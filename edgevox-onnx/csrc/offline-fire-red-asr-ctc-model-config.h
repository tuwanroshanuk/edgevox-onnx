// edgevox-onnx/csrc/offline-fire-red-asr-ctc-model-config.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineFireRedAsrCtcModelConfig {
  std::string model;

  OfflineFireRedAsrCtcModelConfig() = default;
  explicit OfflineFireRedAsrCtcModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_CTC_MODEL_CONFIG_H_
