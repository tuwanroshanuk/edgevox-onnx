// edgevox-onnx/csrc/offline-paraformer-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"
#include "edgevox-onnx/csrc/qnn-config.h"

namespace edgevox_onnx {

struct OfflineParaformerModelConfig {
  // for ascend npu,
  // model is "/path/to/encoder.om,/path/to/predictor.om,/path/to/decoder.om"
  //
  // for rknn,
  // model is
  // "/path/to/encoder.rknn,/path/to/predictor.rknn,/path/to/decoder.rknn"
  //
  // for qnn with shared libs, model is
  // model is
  // "/path/to/libencoder.so,/path/to/libpredictor.so,/path/to/libdecoder.so"
  std::string model;

  QnnConfig qnn_config;

  OfflineParaformerModelConfig() = default;
  explicit OfflineParaformerModelConfig(const std::string &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_MODEL_CONFIG_H_
