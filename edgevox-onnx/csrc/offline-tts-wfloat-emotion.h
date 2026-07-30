// edgevox-onnx/csrc/offline-tts-wfloat-emotion.h
//
// Copyright (c)  2026  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_WFLOAT_EMOTION_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_WFLOAT_EMOTION_H_

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

namespace edgevox_onnx {

struct WfloatEmotionControl {
  int64_t emotion_token = 159;
  int64_t intensity_token = 178;
};

inline void AppendWfloatEmotionControl(
    std::vector<int64_t> *tokens, const WfloatEmotionControl &control) {
  // Wfloat is trained with Piper's complete sequence, including BOS/EOS and
  // blank tokens. Its two style tokens belong immediately before the final
  // padding token (when present), exactly as in the official Wfloat frontend.
  auto insert_at = tokens->end();
  if (!tokens->empty() && tokens->back() == 0) {
    insert_at = tokens->end() - 1;
  }
  tokens->insert(insert_at, {control.emotion_token, control.intensity_token});
}

inline bool GetWfloatEmotionControl(const std::string &value, float intensity,
                                    WfloatEmotionControl *control) {
  std::string emotion = value;
  emotion.erase(emotion.begin(),
                std::find_if(emotion.begin(), emotion.end(), [](char c) {
                  return !std::isspace(static_cast<unsigned char>(c));
                }));
  emotion.erase(
      std::find_if(emotion.rbegin(), emotion.rend(),
                   [](char c) {
                     return !std::isspace(static_cast<unsigned char>(c));
                   })
          .base(),
      emotion.end());
  std::transform(emotion.begin(), emotion.end(), emotion.begin(), [](char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  });

  static const std::array<const char *, 8> kEmotions = {
      "neutral", "joy",     "sadness",   "anger",
      "fear",    "surprise", "dismissive", "confusion"};
  auto it = std::find_if(
      kEmotions.begin(), kEmotions.end(),
      [&emotion](const char *candidate) { return emotion == candidate; });
  if (it == kEmotions.end()) {
    return false;
  }

  if (!std::isfinite(intensity)) {
    intensity = 0;
  }
  intensity = std::max(0.0f, std::min(1.0f, intensity));
  int32_t intensity_index =
      std::min(9, static_cast<int32_t>(intensity * 10));

  control->emotion_token = 159 + std::distance(kEmotions.begin(), it);
  control->intensity_token = 173 + intensity_index;
  return true;
}

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_WFLOAT_EMOTION_H_
