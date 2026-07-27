// edgevox-onnx/csrc/online-nemo-ctc-model-config.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_ONLINE_NEMO_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_NEMO_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OnlineNeMoCtcModelConfig {
  std::string model;

  OnlineNeMoCtcModelConfig() = default;

  explicit OnlineNeMoCtcModelConfig(const std::string &model) : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_NEMO_CTC_MODEL_CONFIG_H_
