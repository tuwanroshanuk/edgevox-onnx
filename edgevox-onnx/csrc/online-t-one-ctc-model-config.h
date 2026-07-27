// edgevox-onnx/csrc/online-t-one-ctc-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_ONLINE_T_ONE_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_T_ONE_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OnlineToneCtcModelConfig {
  std::string model;

  OnlineToneCtcModelConfig() = default;

  explicit OnlineToneCtcModelConfig(const std::string &model) : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_T_ONE_CTC_MODEL_CONFIG_H_
