// edgevox-onnx/csrc/offline-tts-chatterbox-model.cc
//
// Native ONNX Runtime implementation of ResembleAI/chatterbox-turbo-ONNX.
// Copyright (c) 2026 Nexus Cloud

#include "edgevox-onnx/csrc/offline-tts-chatterbox-model.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <limits>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/funasr-nano-tokenizer.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/onnx-utils.h"
#include "edgevox-onnx/csrc/ort-env.h"
#include "edgevox-onnx/csrc/session.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {
namespace {

constexpr int64_t kStartSpeechToken = 6561;
constexpr int64_t kStopSpeechToken = 6562;
constexpr int64_t kSilenceToken = 4299;
constexpr int64_t kTextEosToken = 50256;
constexpr int64_t kNumKvHeads = 16;
constexpr int64_t kHeadDim = 64;

void GetNames(Ort::Session *session, std::vector<std::string> *inputs,
              std::vector<const char *> *input_ptrs,
              std::vector<std::string> *outputs,
              std::vector<const char *> *output_ptrs) {
  GetInputNames(session, inputs, input_ptrs);
  GetOutputNames(session, outputs, output_ptrs);
}

}  // namespace

class OfflineTtsChatterboxModel::Impl {
 public:
  explicit Impl(const OfflineTtsModelConfig &config)
      : tokenizer_(config.chatterbox.tokenizer),
        env_(CreateOrtEnv()),
        opts_(GetSessionOptions(config)) {
    speech_encoder_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config.chatterbox.speech_encoder),
        opts_);
    embed_tokens_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config.chatterbox.embed_tokens), opts_);
    language_model_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config.chatterbox.language_model),
        opts_);
    conditional_decoder_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config.chatterbox.conditional_decoder),
        opts_);

    GetNames(speech_encoder_.get(), &speech_in_, &speech_in_ptr_, &speech_out_,
             &speech_out_ptr_);
    GetNames(embed_tokens_.get(), &embed_in_, &embed_in_ptr_, &embed_out_,
             &embed_out_ptr_);
    GetNames(language_model_.get(), &lm_in_, &lm_in_ptr_, &lm_out_,
             &lm_out_ptr_);
    GetNames(conditional_decoder_.get(), &decoder_in_, &decoder_in_ptr_,
             &decoder_out_, &decoder_out_ptr_);
  }

  std::vector<int64_t> Tokenize(const std::string &text) {
    auto ids = tokenizer_.Encode(text);
    // tokenizer.json applies TemplateProcessing with two trailing EOS tokens.
    ids.push_back(kTextEosToken);
    ids.push_back(kTextEosToken);
    return ids;
  }

  Ort::Value Embed(const std::vector<int64_t> &ids) const {
    auto memory = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    int64_t shape[] = {1, static_cast<int64_t>(ids.size())};
    auto input = Ort::Value::CreateTensor<int64_t>(
        memory, const_cast<int64_t *>(ids.data()), ids.size(), shape, 2);
    auto outputs = embed_tokens_->Run({}, embed_in_ptr_.data(), &input, 1,
                                      embed_out_ptr_.data(), 1);
    return std::move(outputs[0]);
  }

  std::vector<float> Generate(const std::vector<int64_t> &text_tokens,
                              const std::vector<float> &audio,
                              int32_t max_new_tokens,
                              float repetition_penalty) const {
    auto memory = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    int64_t audio_shape[] = {1, static_cast<int64_t>(audio.size())};
    auto audio_tensor = Ort::Value::CreateTensor<float>(
        memory, const_cast<float *>(audio.data()), audio.size(), audio_shape, 2);
    auto speech = speech_encoder_->Run(
        {}, speech_in_ptr_.data(), &audio_tensor, 1, speech_out_ptr_.data(),
        speech_out_ptr_.size());

    Ort::Value text_embed = Embed(text_tokens);
    const auto cond_info = speech[0].GetTensorTypeAndShapeInfo();
    const auto text_info = text_embed.GetTensorTypeAndShapeInfo();
    const auto cond_shape = cond_info.GetShape();
    const auto text_shape = text_info.GetShape();
    if (cond_shape.size() != 3 || text_shape.size() != 3 ||
        cond_shape[2] != text_shape[2]) {
      EDGEVOX_ONNX_LOGE("Unexpected Chatterbox embedding shapes");
      return {};
    }

    const int64_t hidden = cond_shape[2];
    const int64_t initial_len = cond_shape[1] + text_shape[1];
    std::vector<float> initial_embeddings(initial_len * hidden);
    const float *cond = speech[0].GetTensorData<float>();
    const float *txt = text_embed.GetTensorData<float>();
    std::copy(cond, cond + cond_shape[1] * hidden,
              initial_embeddings.begin());
    std::copy(txt, txt + text_shape[1] * hidden,
              initial_embeddings.begin() + cond_shape[1] * hidden);

    std::vector<int64_t> generated = {kStartSpeechToken};
    std::vector<Ort::Value> past;
    int64_t total_len = initial_len;

    for (int32_t step = 0; step < max_new_tokens; ++step) {
      std::vector<Ort::Value> inputs;
      inputs.reserve(lm_in_ptr_.size());
      std::vector<float> next_embedding;
      int64_t seq_len = initial_len;

      if (step == 0) {
        int64_t shape[] = {1, initial_len, hidden};
        inputs.push_back(Ort::Value::CreateTensor<float>(
            memory, initial_embeddings.data(), initial_embeddings.size(), shape,
            3));
      } else {
        std::vector<int64_t> id = {generated.back()};
        Ort::Value embedded = Embed(id);
        const float *p = embedded.GetTensorData<float>();
        next_embedding.assign(p, p + hidden);
        int64_t shape[] = {1, 1, hidden};
        inputs.push_back(Ort::Value::CreateTensor<float>(
            memory, next_embedding.data(), next_embedding.size(), shape, 3));
        seq_len = 1;
      }

      std::vector<int64_t> attention(total_len, 1);
      int64_t attention_shape[] = {1, total_len};
      inputs.push_back(Ort::Value::CreateTensor<int64_t>(
          memory, attention.data(), attention.size(), attention_shape, 2));

      std::vector<int64_t> positions(seq_len);
      const int64_t position_start = total_len - seq_len;
      for (int64_t i = 0; i < seq_len; ++i) positions[i] = position_start + i;
      int64_t position_shape[] = {1, seq_len};
      inputs.push_back(Ort::Value::CreateTensor<int64_t>(
          memory, positions.data(), positions.size(), position_shape, 2));

      if (step == 0) {
        int64_t cache_shape[] = {1, kNumKvHeads, 0, kHeadDim};
        for (size_t i = 3; i < lm_in_ptr_.size(); ++i) {
          inputs.push_back(Ort::Value::CreateTensor<float>(
              memory, nullptr, 0, cache_shape, 4));
        }
      } else {
        for (auto &value : past) inputs.push_back(std::move(value));
      }

      auto outputs = language_model_->Run(
          {}, lm_in_ptr_.data(), inputs.data(), inputs.size(),
          lm_out_ptr_.data(), lm_out_ptr_.size());
      const auto logits_shape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
      const int64_t vocab = logits_shape.back();
      const float *logits = outputs[0].GetTensorData<float>() +
                            (logits_shape[1] - 1) * vocab;

      std::unordered_set<int64_t> seen(generated.begin(), generated.end());
      int64_t best = 0;
      float best_score = -std::numeric_limits<float>::infinity();
      for (int64_t id = 0; id < vocab; ++id) {
        float score = logits[id];
        if (seen.count(id)) {
          score = score < 0 ? score * repetition_penalty
                            : score / repetition_penalty;
        }
        if (score > best_score) {
          best_score = score;
          best = id;
        }
      }
      generated.push_back(best);
      if (best == kStopSpeechToken) break;

      past.clear();
      past.reserve(outputs.size() - 1);
      for (size_t i = 1; i < outputs.size(); ++i) {
        past.push_back(std::move(outputs[i]));
      }
      ++total_len;
    }

    // Exclude start/stop, prepend the reference prompt tokens, then add the
    // three silence tokens used by the official decoder example.
    const auto prompt_shape = speech[1].GetTensorTypeAndShapeInfo().GetShape();
    const int64_t prompt_len = prompt_shape[1];
    const int64_t *prompt = speech[1].GetTensorData<int64_t>();
    std::vector<int64_t> speech_tokens(prompt, prompt + prompt_len);
    size_t end = generated.size();
    if (end > 1 && generated.back() == kStopSpeechToken) --end;
    speech_tokens.insert(speech_tokens.end(), generated.begin() + 1,
                         generated.begin() + end);
    speech_tokens.insert(speech_tokens.end(), 3, kSilenceToken);

    int64_t token_shape[] = {1, static_cast<int64_t>(speech_tokens.size())};
    auto token_tensor = Ort::Value::CreateTensor<int64_t>(
        memory, speech_tokens.data(), speech_tokens.size(), token_shape, 2);

    const auto speaker_shape =
        speech[2].GetTensorTypeAndShapeInfo().GetShape();
    const auto feature_shape =
        speech[3].GetTensorTypeAndShapeInfo().GetShape();
    const size_t speaker_size =
        speech[2].GetTensorTypeAndShapeInfo().GetElementCount();
    const size_t feature_size =
        speech[3].GetTensorTypeAndShapeInfo().GetElementCount();
    auto speaker_tensor = Ort::Value::CreateTensor<float>(
        memory, speech[2].GetTensorMutableData<float>(), speaker_size,
        speaker_shape.data(), speaker_shape.size());
    auto feature_tensor = Ort::Value::CreateTensor<float>(
        memory, speech[3].GetTensorMutableData<float>(), feature_size,
        feature_shape.data(), feature_shape.size());

    Ort::Value decoder_inputs[] = {std::move(token_tensor),
                                   std::move(speaker_tensor),
                                   std::move(feature_tensor)};
    auto decoded = conditional_decoder_->Run(
        {}, decoder_in_ptr_.data(), decoder_inputs, 3, decoder_out_ptr_.data(),
        decoder_out_ptr_.size());
    const size_t n =
        decoded[0].GetTensorTypeAndShapeInfo().GetElementCount();
    const float *wave = decoded[0].GetTensorData<float>();
    return {wave, wave + n};
  }

 private:
  FunASRNanoTokenizer tokenizer_;
  Ort::Env env_;
  Ort::SessionOptions opts_;
  std::unique_ptr<Ort::Session> speech_encoder_;
  std::unique_ptr<Ort::Session> embed_tokens_;
  std::unique_ptr<Ort::Session> language_model_;
  std::unique_ptr<Ort::Session> conditional_decoder_;
  std::vector<std::string> speech_in_, speech_out_, embed_in_, embed_out_,
      lm_in_, lm_out_, decoder_in_, decoder_out_;
  std::vector<const char *> speech_in_ptr_, speech_out_ptr_, embed_in_ptr_,
      embed_out_ptr_, lm_in_ptr_, lm_out_ptr_, decoder_in_ptr_,
      decoder_out_ptr_;
};

OfflineTtsChatterboxModel::OfflineTtsChatterboxModel(
    const OfflineTtsModelConfig &config)
    : impl_(std::make_unique<Impl>(config)) {}

OfflineTtsChatterboxModel::~OfflineTtsChatterboxModel() = default;

std::vector<int64_t> OfflineTtsChatterboxModel::Tokenize(
    const std::string &text) {
  return impl_->Tokenize(text);
}

std::vector<float> OfflineTtsChatterboxModel::Generate(
    const std::vector<int64_t> &text_tokens,
    const std::vector<float> &reference_audio, int32_t max_new_tokens,
    float repetition_penalty) const {
  return impl_->Generate(text_tokens, reference_audio, max_new_tokens,
                         repetition_penalty);
}

}  // namespace edgevox_onnx
