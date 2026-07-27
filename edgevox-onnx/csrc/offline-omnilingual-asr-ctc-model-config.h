// edgevox-onnx/csrc/offline-omnilingual-asr-ctc-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_OMNILINGUAL_ASR_CTC_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_OMNILINGUAL_ASR_CTC_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

// for
// https://github.com/k2-fsa/sherpa-onnx/blob/master/scripts/omnilingual-asr/test.py
struct OfflineOmnilingualAsrCtcModelConfig {
  std::string model;

  OfflineOmnilingualAsrCtcModelConfig() = default;

  explicit OfflineOmnilingualAsrCtcModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);

  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_OMNILINGUAL_ASR_CTC_MODEL_CONFIG_H_
