// edgevox-onnx/csrc/offline-tdnn-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TDNN_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TDNN_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

// for https://github.com/k2-fsa/icefall/tree/master/egs/yesno/ASR/tdnn
struct OfflineTdnnModelConfig {
  std::string model;

  OfflineTdnnModelConfig() = default;
  explicit OfflineTdnnModelConfig(const std::string &model) : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TDNN_MODEL_CONFIG_H_
