// edgevox-onnx/csrc/online-zipformer2-ctc-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_ONLINE_ZIPFORMER2_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_ZIPFORMER2_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OnlineZipformer2CtcModelConfig {
  std::string model;

  OnlineZipformer2CtcModelConfig() = default;

  explicit OnlineZipformer2CtcModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_ZIPFORMER2_CTC_MODEL_CONFIG_H_
