// edgevox-onnx/csrc/offline-zipformer-ctc-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_ZIPFORMER_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_ZIPFORMER_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"
#include "edgevox-onnx/csrc/qnn-config.h"

namespace edgevox_onnx {

// for
// https://github.com/k2-fsa/icefall/blob/master/egs/librispeech/ASR/zipformer/export-onnx-ctc.py
struct OfflineZipformerCtcModelConfig {
  std::string model;
  QnnConfig qnn_config;

  OfflineZipformerCtcModelConfig() = default;

  explicit OfflineZipformerCtcModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);

  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_ZIPFORMER_CTC_MODEL_CONFIG_H_
