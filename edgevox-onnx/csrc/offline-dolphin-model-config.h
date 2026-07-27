// edgevox-onnx/csrc/offline-dolphin-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineDolphinModelConfig {
  std::string model;

  OfflineDolphinModelConfig() = default;
  explicit OfflineDolphinModelConfig(const std::string &model) : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_CONFIG_H_
