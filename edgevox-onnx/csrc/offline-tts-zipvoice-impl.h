// edgevox-onnx/csrc/offline-tts-zipvoice-impl.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_IMPL_H_

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "kaldi-native-fbank/csrc/mel-computations.h"
#include "kaldi-native-fbank/csrc/stft.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/matcha-tts-lexicon.h"
#include "edgevox-onnx/csrc/math.h"
#include "edgevox-onnx/csrc/offline-tts-frontend.h"
#include "edgevox-onnx/csrc/offline-tts-impl.h"
#include "edgevox-onnx/csrc/offline-tts-zipvoice-model-config.h"
#include "edgevox-onnx/csrc/offline-tts-zipvoice-model.h"
#include "edgevox-onnx/csrc/onnx-utils.h"
#include "edgevox-onnx/csrc/resample.h"
#include "edgevox-onnx/csrc/text-utils.h"
#include "edgevox-onnx/csrc/vocoder.h"

namespace edgevox_onnx {

class OfflineTtsZipvoiceImpl : public OfflineTtsImpl {
 public:
  explicit OfflineTtsZipvoiceImpl(const OfflineTtsConfig &config)
      : config_(config),
        memory_info_(
            Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault)),
        model_(std::make_unique<OfflineTtsZipvoiceModel>(config.model)),
        vocoder_(Vocoder::Create(config.model)) {
    InitFrontend();

    PostInit();
  }

  template <typename Manager>
  OfflineTtsZipvoiceImpl(Manager *mgr, const OfflineTtsConfig &config)
      : config_(config),
        memory_info_(
            Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault)),
        model_(std::make_unique<OfflineTtsZipvoiceModel>(mgr, config.model)),
        vocoder_(Vocoder::Create(mgr, config.model)) {
    InitFrontend(mgr);

    PostInit();
  }

  int32_t SampleRate() const override {
    return model_->GetMetaData().sample_rate;
  }

  GeneratedAudio Generate(
      const std::string &text, const GenerationConfig &config,
      GeneratedAudioCallback callback = nullptr) const override {
    // Supported extra options in config.extra:
    //   - "speed" (float): Speech speed factor (default: 1.0)
    //   - "num_steps" (int): Number of flow-matching steps (default: 4)
    //   - "max_char_in_sentence" (int): Max characters per chunk (default: 200)
    //   - "min_char_in_sentence" (int): Merge shorter chunks until this size
    //     (default: 30)
    //   - "feat_scale" (float): Prompt mel log scaling factor (default:
    //     config.model.zipvoice.feat_scale)
    //   - "t_shift" (float): Timestep shift used by the decoder schedule
    //     (default: config.model.zipvoice.t_shift)
    //   - "target_rms" (float): Prompt RMS normalization target (default:
    //     config.model.zipvoice.target_rms)
    //   - "guidance_scale" (float): Classifier-free guidance scale for the
    //     decoder (default: config.model.zipvoice.guidance_scale)
    //   - "seed" (int): Optional deterministic noise seed. The default -1
    //     preserves random generation.
    if (config_.model.debug) {
      EDGEVOX_ONNX_LOGE("%s", config.ToString().c_str());
    }

    if (config.reference_sample_rate <= 0) {
      EDGEVOX_ONNX_LOGE("reference_sample_rate %d is invalid.",
                       config.reference_sample_rate);
      return {};
    }

    if (config.reference_audio.empty()) {
      EDGEVOX_ONNX_LOGE("reference_audio is empty.");
      return {};
    }

    if (config.reference_text.empty()) {
      EDGEVOX_ONNX_LOGE("reference_text is empty.");
      return {};
    }

    float speed =
        config.GetExtraFloat("speed", config.speed > 0 ? config.speed : 1.0f);
    if (speed <= 0) {
      EDGEVOX_ONNX_LOGE("Speed must be > 0. Given: %f", speed);
      return {};
    }

    int32_t num_steps = config.GetExtraInt(
        "num_steps", config.num_steps > 0 ? config.num_steps : 4);
    if (num_steps <= 0) {
      EDGEVOX_ONNX_LOGE("Num steps must be > 0. Given: %d", num_steps);
      return {};
    }

    float feat_scale =
        config.GetExtraFloat("feat_scale", config_.model.zipvoice.feat_scale);
    if (feat_scale <= 0) {
      EDGEVOX_ONNX_LOGE("feat_scale must be > 0. Given: %f", feat_scale);
      return {};
    }

    float t_shift =
        config.GetExtraFloat("t_shift", config_.model.zipvoice.t_shift);
    if (t_shift < 0) {
      EDGEVOX_ONNX_LOGE("t_shift must be >= 0. Given: %f", t_shift);
      return {};
    }

    float target_rms =
        config.GetExtraFloat("target_rms", config_.model.zipvoice.target_rms);
    if (target_rms <= 0) {
      EDGEVOX_ONNX_LOGE("target_rms must be > 0. Given: %f", target_rms);
      return {};
    }

    float guidance_scale = config.GetExtraFloat(
        "guidance_scale", config_.model.zipvoice.guidance_scale);
    if (guidance_scale <= 0) {
      EDGEVOX_ONNX_LOGE("guidance_scale must be > 0. Given: %f", guidance_scale);
      return {};
    }

    int32_t seed = config.GetExtraInt("seed", -1);
    if (seed < -1) {
      EDGEVOX_ONNX_LOGE("seed must be >= -1. Given: %d", seed);
      return {};
    }

    auto prompt = PreparePrompt(config.reference_text, config.reference_audio,
                                config.reference_sample_rate, feat_scale,
                                target_rms);
    if (!prompt) {
#if __OHOS__
      EDGEVOX_ONNX_LOGE(
          "Failed to prepare ZipVoice prompt '%{public}s'",
          config.reference_text.c_str());
#else
      EDGEVOX_ONNX_LOGE("Failed to prepare ZipVoice prompt '%s'",
                       config.reference_text.c_str());
#endif
      return {};
    }

    auto sentences = SplitByPunctuation(text);
    if (sentences.empty()) {
      return {};
    }

    int32_t max_char_in_sentence =
        config.GetExtraInt("max_char_in_sentence", 200);
    int32_t min_char_in_sentence =
        config.GetExtraInt("min_char_in_sentence", 30);

    if (max_char_in_sentence <= 0) {
      EDGEVOX_ONNX_LOGE("max_char_in_sentence must be > 0. Given: %d",
                       max_char_in_sentence);
      return {};
    }

    if (min_char_in_sentence <= 0) {
      EDGEVOX_ONNX_LOGE("min_char_in_sentence must be > 0. Given: %d",
                       min_char_in_sentence);
      return {};
    }

    sentences = MergeShortSentences(sentences, min_char_in_sentence);

    std::vector<std::string> final_chunks;
    for (const auto &s : sentences) {
      auto pieces = SplitLongSentence(s, max_char_in_sentence);
      final_chunks.insert(final_chunks.end(), pieces.begin(), pieces.end());
    }

    sentences = std::move(final_chunks);
    if (sentences.empty()) {
      return {};
    }

    GeneratedAudio result;
    result.sample_rate = SampleRate();

    const int32_t total = static_cast<int32_t>(sentences.size());

    for (int32_t i = 0; i < total; ++i) {
      if (config_.model.debug) {
#if __OHOS__
        EDGEVOX_ONNX_LOGE("Processing %{public}d/%{public}d: %{public}s", i + 1,
                         total, sentences[i].c_str());
#else
        EDGEVOX_ONNX_LOGE("Processing %d/%d: %s", i + 1, total,
                         sentences[i].c_str());
#endif
      }

      const int32_t sentence_seed =
          seed < 0
              ? -1
              : static_cast<int32_t>(
                    (static_cast<uint32_t>(seed) + static_cast<uint32_t>(i)) &
                    0x7fffffffU);
      GeneratedAudio cur = GenerateChunk(
          sentences[i], prompt->tokens, prompt->features, speed, num_steps,
          feat_scale, t_shift, guidance_scale, sentence_seed);

      if (cur.samples.empty()) {
        continue;
      }

      result.samples.insert(result.samples.end(), cur.samples.begin(),
                            cur.samples.end());

      if (callback) {
        if (!callback(cur.samples.data(),
                      static_cast<int32_t>(cur.samples.size()),
                      (i + 1) * 1.0f / total)) {
          break;
        }
      }
    }

    if (config.silence_scale != 1) {
      result = result.ScaleSilence(config.silence_scale);
    }

    return result;
  }

  GeneratedAudio Generate(
      const std::string &text, const std::string &prompt_text,
      const std::vector<float> &prompt_samples, int32_t sample_rate,
      float speed, int32_t num_steps,
      GeneratedAudioCallback callback = nullptr) const override {
    GenerationConfig config;
    config.speed = speed;
    config.num_steps = num_steps;
    config.reference_text = prompt_text;
    config.reference_audio = prompt_samples;
    config.reference_sample_rate = sample_rate;
    return Generate(text, config, std::move(callback));
  }

 private:
  void PostInit() {
    InitMelBanks();

    const auto &meta = model_->GetMetaData();
    knf::StftConfig stft_config;
    stft_config.n_fft = meta.n_fft;
    stft_config.hop_length = meta.hop_length;
    stft_config.win_length = meta.window_length;
    stft_config.window_type = "hann";
    stft_config.center = true;
    stft_ = std::make_unique<knf::Stft>(stft_config);
    magnitude_spectrum_.resize(meta.n_fft / 2 + 1);
  }

  void InitMelBanks() {
    const auto &meta = model_->GetMetaData();
    int32_t sample_rate = meta.sample_rate;
    int32_t n_fft = meta.n_fft;
    int32_t hop_length = meta.hop_length;
    int32_t win_length = meta.window_length;
    int32_t num_mels = meta.num_mels;

    knf::FrameExtractionOptions frame_opts;
    frame_opts.samp_freq = sample_rate;
    frame_opts.frame_length_ms = win_length * 1000 / sample_rate;
    frame_opts.frame_shift_ms = hop_length * 1000 / sample_rate;
    frame_opts.window_type = "hanning";

    knf::MelBanksOptions mel_opts;
    mel_opts.num_bins = num_mels;
    mel_opts.low_freq = 0;
    mel_opts.high_freq = sample_rate / 2;
    mel_opts.is_librosa = true;
    mel_opts.use_slaney_mel_scale = false;
    mel_opts.norm = "";

    mel_banks_ = std::make_unique<knf::MelBanks>(mel_opts, frame_opts, 1.0f);
  }

  template <typename Manager>
  void InitFrontend(Manager *mgr) {
    frontend_ = std::make_unique<MatchaTtsLexicon>(
        mgr, config_.model.zipvoice.lexicon, config_.model.zipvoice.tokens,
        config_.model.zipvoice.data_dir, config_.model.debug, true,
        config_.model.zipvoice.espeak_voice);
  }

  void InitFrontend() {
    frontend_ = std::make_unique<MatchaTtsLexicon>(
        config_.model.zipvoice.lexicon, config_.model.zipvoice.tokens,
        config_.model.zipvoice.data_dir, config_.model.debug, true,
        config_.model.zipvoice.espeak_voice);
  }

  void ComputeMelSpectrogram(const std::vector<float> &_samples,
                             int32_t sample_rate, float feat_scale,
                             std::vector<float> *prompt_features) const {
    const auto &meta = model_->GetMetaData();
    if (sample_rate != meta.sample_rate) {
      EDGEVOX_ONNX_LOGE(
          "Creating a resampler:\n"
          "   in_sample_rate: %d\n"
          "   output_sample_rate: %d\n",
          sample_rate, static_cast<int32_t>(meta.sample_rate));

      float min_freq = std::min<int32_t>(sample_rate, meta.sample_rate);
      float lowpass_cutoff = 0.99 * 0.5 * min_freq;

      int32_t lowpass_filter_width = 6;
      auto resampler = std::make_unique<LinearResample>(
          sample_rate, meta.sample_rate, lowpass_cutoff, lowpass_filter_width);
      std::vector<float> samples;
      resampler->Resample(_samples.data(), _samples.size(), true, &samples);
      ComputeMelSpectrogram(samples, feat_scale, prompt_features);
      return;
    }

    ComputeMelSpectrogram(_samples, feat_scale, prompt_features);
  }

  void ComputeMelSpectrogram(const std::vector<float> &samples,
                             float feat_scale,
                             std::vector<float> *prompt_features) const {
    const auto &meta = model_->GetMetaData();

    int32_t n_fft = meta.n_fft;
    int32_t num_mels = meta.num_mels;

    std::lock_guard<std::mutex> feature_lock(feature_mutex_);
    auto stft_result = stft_->Compute(samples.data(), samples.size());
    int32_t num_frames = stft_result.num_frames;
    int32_t fft_bins = n_fft / 2 + 1;

    prompt_features->resize(num_frames * num_mels);
    float *p = prompt_features->data();

    for (int32_t i = 0; i < num_frames; ++i, p += num_mels) {
      for (int32_t k = 0; k < fft_bins; ++k) {
        float real = stft_result.real[i * fft_bins + k];
        float imag = stft_result.imag[i * fft_bins + k];
        magnitude_spectrum_[k] = std::sqrt(real * real + imag * imag);
      }

      mel_banks_->Compute(magnitude_spectrum_.data(), p);

      for (int32_t j = 0; j < num_mels; ++j) {
        p[j] = std::log(p[j] + 1e-10f) * feat_scale;
      }
    }
  }

  GeneratedAudio GenerateChunk(const std::string &text,
                               const std::vector<int64_t> &prompt_tokens,
                               const std::vector<float> &prompt_features,
                               float speed, int32_t num_steps, float feat_scale,
                               float t_shift, float guidance_scale,
                               int32_t seed) const {
    std::vector<TokenIDs> text_token_ids =
        frontend_->ConvertTextToTokenIds(text);

    if (text_token_ids.empty() ||
        (text_token_ids.size() == 1 && text_token_ids[0].tokens.empty())) {
#if __OHOS__
      EDGEVOX_ONNX_LOGE("Failed to convert '%{public}s' to token IDs",
                       text.c_str());
#else
      EDGEVOX_ONNX_LOGE("Failed to convert '%s' to token IDs", text.c_str());
#endif
      return {};
    }

    std::vector<int64_t> tokens;
    for (const auto &t : text_token_ids) {
      tokens.insert(tokens.end(), t.tokens.begin(), t.tokens.end());
    }

    return Process(tokens, prompt_tokens, prompt_features, speed, num_steps,
                   feat_scale, t_shift, guidance_scale, seed);
  }

  struct PreparedPrompt {
    std::vector<int64_t> tokens;
    std::vector<float> features;
  };

  static uint64_t HashBytes(uint64_t hash, const void *data, size_t size) {
    const auto *p = static_cast<const uint8_t *>(data);
    for (size_t i = 0; i < size; ++i) {
      hash ^= p[i];
      hash *= 1099511628211ULL;
    }
    return hash;
  }

  static uint64_t PromptCacheKey(const std::string &text,
                                 const std::vector<float> &samples,
                                 int32_t sample_rate, float feat_scale,
                                 float target_rms) {
    uint64_t hash = 1469598103934665603ULL;
    hash = HashBytes(hash, text.data(), text.size());
    hash = HashBytes(hash, &sample_rate, sizeof(sample_rate));
    hash = HashBytes(hash, &feat_scale, sizeof(feat_scale));
    hash = HashBytes(hash, &target_rms, sizeof(target_rms));
    if (!samples.empty()) {
      hash = HashBytes(hash, samples.data(), samples.size() * sizeof(float));
    }
    return hash;
  }

  std::shared_ptr<const PreparedPrompt> PreparePrompt(
      const std::string &text, const std::vector<float> &samples,
      int32_t sample_rate, float feat_scale, float target_rms) const {
    const uint64_t key =
        PromptCacheKey(text, samples, sample_rate, feat_scale, target_rms);
    {
      std::lock_guard<std::mutex> lock(prompt_cache_mutex_);
      for (auto it = prompt_cache_.begin(); it != prompt_cache_.end(); ++it) {
        if (it->first == key) {
          auto prepared = it->second;
          prompt_cache_.splice(prompt_cache_.begin(), prompt_cache_, it);
          return prepared;
        }
      }
    }

    std::vector<TokenIDs> prompt_token_ids =
        frontend_->ConvertTextToTokenIds(text);
    if (prompt_token_ids.empty() ||
        (prompt_token_ids.size() == 1 && prompt_token_ids[0].tokens.empty())) {
      return nullptr;
    }

    auto prepared = std::make_shared<PreparedPrompt>();
    for (const auto &t : prompt_token_ids) {
      prepared->tokens.insert(prepared->tokens.end(), t.tokens.begin(),
                              t.tokens.end());
    }
    prepared->features =
        ComputePromptFeatures(samples, sample_rate, feat_scale, target_rms);
    if (prepared->features.empty()) {
      return nullptr;
    }

    {
      std::lock_guard<std::mutex> lock(prompt_cache_mutex_);
      prompt_cache_.emplace_front(key, prepared);
      if (prompt_cache_.size() > 2) {
        prompt_cache_.pop_back();
      }
    }
    return prepared;
  }

  std::vector<float> ComputePromptFeatures(
      const std::vector<float> &prompt_samples, int32_t sample_rate,
      float feat_scale, float target_rms) const {
    std::vector<float> prompt_samples_scaled = prompt_samples;
    double prompt_rms = 0.0;
    double sum_sq = 0.0;
    for (float s : prompt_samples_scaled) {
      sum_sq += s * s;
    }
    prompt_rms = std::sqrt(sum_sq / prompt_samples_scaled.size());
    if (prompt_rms < target_rms && prompt_rms > 0.0f) {
      float scale = target_rms / prompt_rms;
      for (auto &s : prompt_samples_scaled) {
        s *= scale;
      }
    }

    std::vector<float> prompt_features;
    ComputeMelSpectrogram(prompt_samples_scaled, sample_rate, feat_scale,
                          &prompt_features);

    return prompt_features;
  }

  GeneratedAudio Process(const std::vector<int64_t> &tokens,
                         const std::vector<int64_t> &prompt_tokens,
                         const std::vector<float> &prompt_features, float speed,
                         int32_t num_steps, float feat_scale, float t_shift,
                         float guidance_scale, int32_t seed) const {
    std::array<int64_t, 2> tokens_shape = {1,
                                           static_cast<int64_t>(tokens.size())};

    Ort::Value tokens_tensor = Ort::Value::CreateTensor(
        memory_info_, const_cast<int64_t *>(tokens.data()), tokens.size(),
        tokens_shape.data(), tokens_shape.size());

    std::array<int64_t, 2> prompt_tokens_shape = {
        1, static_cast<int64_t>(prompt_tokens.size())};

    Ort::Value prompt_tokens_tensor = Ort::Value::CreateTensor(
        memory_info_, const_cast<int64_t *>(prompt_tokens.data()),
        prompt_tokens.size(), prompt_tokens_shape.data(),
        prompt_tokens_shape.size());

    int32_t mel_dim = model_->GetMetaData().num_mels;

    int32_t num_frames = prompt_features.size() / mel_dim;

    std::array<int64_t, 3> shape = {1, num_frames, mel_dim};
    auto prompt_features_tensor = Ort::Value::CreateTensor(
        memory_info_, const_cast<float *>(prompt_features.data()),
        prompt_features.size(), shape.data(), shape.size());

    Ort::Value mel =
        model_->Run(std::move(tokens_tensor), std::move(prompt_tokens_tensor),
                    std::move(prompt_features_tensor), speed, num_steps,
                    t_shift, guidance_scale, seed);

    // Assume mel_shape = {1, T, C}
    std::vector<int64_t> mel_shape = mel.GetTensorTypeAndShapeInfo().GetShape();
    int64_t T = mel_shape[1];
    int64_t C = mel_shape[2];

    const float *mel_data = mel.GetTensorData<float>();

    float inv_feat_scale = 1 / feat_scale;

    // mel_permuted is (C, T)
    std::vector<float> mel_permuted = Transpose(mel_data, T, C);

    Scale(mel_permuted.data(), inv_feat_scale, mel_permuted.size(),
          mel_permuted.data());

    std::array<int64_t, 3> new_shape = {1, C, T};
    Ort::Value mel_new = Ort::Value::CreateTensor<float>(
        memory_info_, mel_permuted.data(), mel_permuted.size(), new_shape.data(),
        new_shape.size());

    GeneratedAudio ans;
    ans.samples = vocoder_->Run(std::move(mel_new));
    ans.sample_rate = model_->GetMetaData().sample_rate;
    return ans;
  }

 private:
  OfflineTtsConfig config_;
  Ort::MemoryInfo memory_info_;
  std::unique_ptr<OfflineTtsZipvoiceModel> model_;
  std::unique_ptr<Vocoder> vocoder_;
  std::unique_ptr<OfflineTtsFrontend> frontend_;

  std::unique_ptr<knf::MelBanks> mel_banks_;
  mutable std::unique_ptr<knf::Stft> stft_;
  mutable std::vector<float> magnitude_spectrum_;
  mutable std::mutex feature_mutex_;
  mutable std::mutex prompt_cache_mutex_;
  mutable std::list<
      std::pair<uint64_t, std::shared_ptr<const PreparedPrompt>>>
      prompt_cache_;
};

}  // namespace edgevox_onnx
#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_IMPL_H_
