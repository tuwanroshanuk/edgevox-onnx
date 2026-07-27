// edgevox-onnx/csrc/offline-sense-voice-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SENSE_VOICE_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SENSE_VOICE_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"
#include "edgevox-onnx/csrc/qnn-config.h"

namespace edgevox_onnx {

struct OfflineSenseVoiceModelConfig {
  std::string model;

  // "" or "auto" to let the model recognize the language
  // valid values:
  //  zh, en, ja, ko, yue, auto
  std::string language = "auto";

  // true to use inverse text normalization
  // false to not use inverse text normalization
  bool use_itn = false;

  QnnConfig qnn_config;

  OfflineSenseVoiceModelConfig() = default;
  OfflineSenseVoiceModelConfig(const std::string &model,
                               const std::string &language, bool use_itn)
      : model(model), language(language), use_itn(use_itn) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SENSE_VOICE_MODEL_CONFIG_H_
