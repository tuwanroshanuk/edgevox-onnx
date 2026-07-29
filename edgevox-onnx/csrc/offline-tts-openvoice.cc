// edgevox-onnx/csrc/offline-tts-openvoice.cc
//
// Copyright (c)  2026  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-tts-openvoice.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "kaldi-native-fbank/csrc/rfft.h"
#include "edgevox-onnx/csrc/ort-env.h"
#include "edgevox-onnx/csrc/resample.h"
#include "edgevox-onnx/csrc/session.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {

namespace {

constexpr int32_t kSampleRate = 22050;
constexpr int32_t kFftSize = 1024;
constexpr int32_t kHopSize = 256;
constexpr int32_t kPad = (kFftSize - kHopSize) / 2;
constexpr int32_t kSpecBins = kFftSize / 2 + 1;
constexpr int32_t kToneDim = 256;
constexpr float kPi = 3.14159265358979323846f;

std::vector<float> ResampleToOpenVoice(const std::vector<float> &samples,
                                       int32_t sample_rate) {
  if (sample_rate == kSampleRate) {
    return samples;
  }
  float cutoff = 0.99f * 0.5f * std::min(sample_rate, kSampleRate);
  LinearResample resampler(sample_rate, kSampleRate, cutoff, 6);
  std::vector<float> output;
  resampler.Resample(samples.data(), static_cast<int32_t>(samples.size()), true,
                     &output);
  return output;
}

int64_t ReflectIndex(int64_t index, int64_t size) {
  if (size <= 1) {
    return 0;
  }
  while (index < 0 || index >= size) {
    if (index < 0) {
      index = -index;
    } else {
      index = 2 * size - 2 - index;
    }
  }
  return index;
}

std::vector<float> ComputeLinearSpectrogram(const std::vector<float> &audio,
                                            int64_t *num_frames) {
  int64_t padded_size = static_cast<int64_t>(audio.size()) + 2 * kPad;
  *num_frames = 1 + (padded_size - kFftSize) / kHopSize;
  if (audio.empty() || *num_frames <= 0) {
    *num_frames = 0;
    return {};
  }

  std::vector<float> output(kSpecBins * *num_frames);
  std::vector<float> frame(kFftSize);
  knf::Rfft rfft(kFftSize);

  for (int64_t t = 0; t != *num_frames; ++t) {
    int64_t start = t * kHopSize - kPad;
    for (int32_t i = 0; i != kFftSize; ++i) {
      int64_t source = ReflectIndex(start + i, audio.size());
      float window =
          0.5f - 0.5f * std::cos(2 * kPi * i / (kFftSize - 1));
      frame[i] = audio[source] * window;
    }
    rfft.Compute(frame.data());

    output[t] = std::sqrt(frame[0] * frame[0] + 1e-6f);
    for (int32_t k = 1; k != kFftSize / 2; ++k) {
      float real = frame[2 * k];
      float imag = frame[2 * k + 1];
      output[k * *num_frames + t] =
          std::sqrt(real * real + imag * imag + 1e-6f);
    }
    output[(kSpecBins - 1) * *num_frames + t] =
        std::sqrt(frame[1] * frame[1] + 1e-6f);
  }
  return output;
}

uint64_t AudioFingerprint(const std::vector<float> &audio,
                          int32_t sample_rate) {
  uint64_t hash = 1469598103934665603ULL;
  auto mix = [&hash](uint64_t value) {
    hash ^= value;
    hash *= 1099511628211ULL;
  };
  mix(audio.size());
  mix(sample_rate);
  if (!audio.empty()) {
    constexpr int32_t kPoints = 128;
    for (int32_t i = 0; i != kPoints; ++i) {
      size_t index =
          static_cast<size_t>(i) * (audio.size() - 1) / (kPoints - 1);
      int32_t quantized = static_cast<int32_t>(audio[index] * 1000000);
      mix(static_cast<uint32_t>(quantized));
    }
  }
  return hash;
}

}  // namespace

std::vector<float> ComputeOpenVoiceLinearSpectrogramForTest(
    const std::vector<float> &audio, int64_t *num_frames) {
  return ComputeLinearSpectrogram(audio, num_frames);
}

class OfflineTtsOpenVoice::Impl {
 public:
  explicit Impl(const OfflineTtsModelConfig &config)
      : config_(config),
        env_(CreateOrtEnv()),
        session_options_(GetSessionOptions(config)) {}

  std::vector<float> Convert(const std::vector<float> &source,
                             int32_t source_sample_rate,
                             const std::vector<float> &reference,
                             int32_t reference_sample_rate) const {
    EnsureModels();
    auto source_22k = ResampleToOpenVoice(source, source_sample_rate);
    auto reference_22k =
        ResampleToOpenVoice(reference, reference_sample_rate);
    if (source_22k.empty() || reference_22k.empty()) {
      return {};
    }

    int64_t source_frames = 0;
    auto source_spec =
        ComputeLinearSpectrogram(source_22k, &source_frames);
    if (source_frames <= 0) {
      return {};
    }
    auto source_embedding = ExtractEmbedding(source_spec, source_frames);
    auto target_embedding =
        GetReferenceEmbedding(reference_22k, reference_sample_rate);

    auto memory_info =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault);
    std::array<int64_t, 3> spec_shape = {1, kSpecBins, source_frames};
    Ort::Value spec_tensor = Ort::Value::CreateTensor(
        memory_info, source_spec.data(), source_spec.size(), spec_shape.data(),
        spec_shape.size());
    std::array<int64_t, 1> lengths_shape = {1};
    Ort::Value lengths_tensor = Ort::Value::CreateTensor(
        memory_info, &source_frames, 1, lengths_shape.data(),
        lengths_shape.size());
    std::array<int64_t, 3> embedding_shape = {1, kToneDim, 1};
    Ort::Value source_tensor = Ort::Value::CreateTensor(
        memory_info, source_embedding.data(), source_embedding.size(),
        embedding_shape.data(), embedding_shape.size());
    Ort::Value target_tensor = Ort::Value::CreateTensor(
        memory_info, target_embedding.data(), target_embedding.size(),
        embedding_shape.data(), embedding_shape.size());

    const char *input_names[] = {"spec", "spec_lengths", "src_g", "tgt_g"};
    const char *output_names[] = {"audio"};
    std::array<Ort::Value, 4> inputs = {
        std::move(spec_tensor), std::move(lengths_tensor),
        std::move(source_tensor), std::move(target_tensor)};
    auto outputs = converter_->Run({}, input_names, inputs.data(), inputs.size(),
                                   output_names, 1);
    auto shape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
    int64_t size = 1;
    for (auto dim : shape) {
      size *= dim;
    }
    const float *data = outputs[0].GetTensorData<float>();
    return {data, data + size};
  }

 private:
  void EnsureModels() const {
    std::lock_guard<std::mutex> lock(models_mutex_);
    if (encoder_) {
      return;
    }
    encoder_ = std::make_unique<Ort::Session>(
        env_,
        EDGEVOX_ONNX_TO_ORT_PATH(config_.vits.openvoice_tone_encoder),
        session_options_);
    converter_ = std::make_unique<Ort::Session>(
        env_,
        EDGEVOX_ONNX_TO_ORT_PATH(config_.vits.openvoice_tone_converter),
        session_options_);
  }

  std::vector<float> ExtractEmbedding(const std::vector<float> &spec,
                                      int64_t frames) const {
    std::vector<float> transposed(frames * kSpecBins);
    for (int64_t t = 0; t != frames; ++t) {
      for (int32_t k = 0; k != kSpecBins; ++k) {
        transposed[t * kSpecBins + k] = spec[k * frames + t];
      }
    }

    auto memory_info =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault);
    std::array<int64_t, 3> shape = {1, frames, kSpecBins};
    Ort::Value input = Ort::Value::CreateTensor(
        memory_info, transposed.data(), transposed.size(), shape.data(),
        shape.size());
    const char *input_names[] = {"spec"};
    const char *output_names[] = {"tone_embedding"};
    auto outputs =
        encoder_->Run({}, input_names, &input, 1, output_names, 1);
    const float *data = outputs[0].GetTensorData<float>();
    return {data, data + kToneDim};
  }

  std::vector<float> GetReferenceEmbedding(
      const std::vector<float> &reference, int32_t original_sample_rate) const {
    uint64_t fingerprint = AudioFingerprint(reference, original_sample_rate);
    {
      std::lock_guard<std::mutex> lock(cache_mutex_);
      if (fingerprint == cached_fingerprint_ &&
          cached_embedding_.size() == kToneDim) {
        return cached_embedding_;
      }
    }

    int64_t frames = 0;
    auto spec = ComputeLinearSpectrogram(reference, &frames);
    auto embedding = ExtractEmbedding(spec, frames);
    {
      std::lock_guard<std::mutex> lock(cache_mutex_);
      cached_fingerprint_ = fingerprint;
      cached_embedding_ = embedding;
    }
    return embedding;
  }

 private:
  OfflineTtsModelConfig config_;
  Ort::Env env_;
  Ort::SessionOptions session_options_;
  mutable std::mutex models_mutex_;
  mutable std::unique_ptr<Ort::Session> encoder_;
  mutable std::unique_ptr<Ort::Session> converter_;
  mutable std::mutex cache_mutex_;
  mutable uint64_t cached_fingerprint_ = 0;
  mutable std::vector<float> cached_embedding_;
};

OfflineTtsOpenVoice::OfflineTtsOpenVoice(const OfflineTtsModelConfig &config)
    : impl_(std::make_unique<Impl>(config)) {}

OfflineTtsOpenVoice::~OfflineTtsOpenVoice() = default;

std::vector<float> OfflineTtsOpenVoice::Convert(
    const std::vector<float> &source, int32_t source_sample_rate,
    const std::vector<float> &reference,
    int32_t reference_sample_rate) const {
  return impl_->Convert(source, source_sample_rate, reference,
                        reference_sample_rate);
}

}  // namespace edgevox_onnx
