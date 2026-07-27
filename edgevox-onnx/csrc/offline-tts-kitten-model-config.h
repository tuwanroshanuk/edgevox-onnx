// edgevox-onnx/csrc/offline-tts-kitten-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KITTEN_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KITTEN_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineTtsKittenModelConfig {
  std::string model;
  std::string voices;
  std::string tokens;

  std::string data_dir;
  // speed = 1 / length_scale
  float length_scale = 1.0;

  OfflineTtsKittenModelConfig() = default;

  OfflineTtsKittenModelConfig(const std::string &model,
                              const std::string &voices,
                              const std::string &tokens,
                              const std::string &data_dir, float length_scale)
      : model(model),
        voices(voices),
        tokens(tokens),
        data_dir(data_dir),
        length_scale(length_scale) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KITTEN_MODEL_CONFIG_H_
