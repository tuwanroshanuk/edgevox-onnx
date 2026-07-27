// edgevox-onnx/csrc/offline-nemo-enc-dec-ctc-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_NEMO_ENC_DEC_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_NEMO_ENC_DEC_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"
#include "edgevox-onnx/csrc/qnn-config.h"

namespace edgevox_onnx {

struct OfflineNemoEncDecCtcModelConfig {
  std::string model;
  QnnConfig qnn_config;

  OfflineNemoEncDecCtcModelConfig() = default;
  explicit OfflineNemoEncDecCtcModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_NEMO_ENC_DEC_CTC_MODEL_CONFIG_H_
