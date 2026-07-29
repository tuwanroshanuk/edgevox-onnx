// edgevox-onnx/csrc/offline-tts-chatterbox-impl.h
//
// Copyright (c) 2026 Nexus Cloud

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_IMPL_H_

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/offline-tts-chatterbox-model.h"
#include "edgevox-onnx/csrc/offline-tts-impl.h"
#include "edgevox-onnx/csrc/resample.h"

namespace edgevox_onnx {

class OfflineTtsChatterboxImpl : public OfflineTtsImpl {
 public:
  explicit OfflineTtsChatterboxImpl(const OfflineTtsConfig &config)
      : config_(config),
        model_(std::make_unique<OfflineTtsChatterboxModel>(config.model)) {}

  int32_t SampleRate() const override { return 24000; }
  int32_t NumSpeakers() const override { return 1; }

  GeneratedAudio Generate(
      const std::string &text, const GenerationConfig &gen_config,
      GeneratedAudioCallback callback = nullptr) const override {
    GeneratedAudio result;
    result.sample_rate = SampleRate();
    if (text.empty()) return result;
    if (gen_config.reference_audio.empty() ||
        gen_config.reference_sample_rate <= 0) {
      EDGEVOX_ONNX_LOGE(
          "Chatterbox Turbo requires reference_audio and "
          "reference_sample_rate");
      return result;
    }

    std::vector<float> reference = gen_config.reference_audio;
    if (gen_config.reference_sample_rate != SampleRate()) {
      const float cutoff =
          0.99f * 0.5f *
          std::min(gen_config.reference_sample_rate, SampleRate());
      LinearResample resampler(gen_config.reference_sample_rate, SampleRate(),
                               cutoff, 6);
      resampler.Resample(gen_config.reference_audio.data(),
                         gen_config.reference_audio.size(), true, &reference);
    }

    // Ten seconds is enough for stable cloning and avoids needless encoder
    // latency/memory on long recordings.
    const int32_t max_reference_seconds =
        gen_config.GetExtraInt("max_reference_seconds", 10);
    const size_t max_samples =
        static_cast<size_t>(SampleRate()) * max_reference_seconds;
    if (reference.size() > max_samples) reference.resize(max_samples);

    const int32_t max_new_tokens =
        gen_config.GetExtraInt("max_new_tokens", 1024);
    const float repetition_penalty =
        gen_config.GetExtraFloat("repetition_penalty", 1.2f);
    auto tokens = model_->Tokenize(text);
    result.samples =
        model_->Generate(tokens, reference, max_new_tokens, repetition_penalty);
    if (callback && !result.samples.empty()) {
      callback(result.samples.data(),
               static_cast<int32_t>(result.samples.size()), 1.0f);
    }
    return result;
  }

 private:
  OfflineTtsConfig config_;
  std::unique_ptr<OfflineTtsChatterboxModel> model_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_IMPL_H_
