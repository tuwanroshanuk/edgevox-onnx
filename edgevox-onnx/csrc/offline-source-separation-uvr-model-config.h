// edgevox-onnx/csrc/offline-source-separation-uvr-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/offline-source-separation-uvr-model-config.h"
#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineSourceSeparationUvrModelConfig {
  std::string model;

  OfflineSourceSeparationUvrModelConfig() = default;

  explicit OfflineSourceSeparationUvrModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);

  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_CONFIG_H_
