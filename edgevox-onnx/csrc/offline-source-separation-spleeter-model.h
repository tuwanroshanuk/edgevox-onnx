// edgevox-onnx/csrc/offline-source-separation-spleeter-model.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_H_
#include <memory>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-source-separation-model-config.h"
#include "edgevox-onnx/csrc/offline-source-separation-spleeter-model-meta-data.h"

namespace edgevox_onnx {

class OfflineSourceSeparationSpleeterModel {
 public:
  ~OfflineSourceSeparationSpleeterModel();

  explicit OfflineSourceSeparationSpleeterModel(
      const OfflineSourceSeparationModelConfig &config);

  template <typename Manager>
  OfflineSourceSeparationSpleeterModel(
      Manager *mgr, const OfflineSourceSeparationModelConfig &config);

  Ort::Value RunVocals(Ort::Value x) const;
  Ort::Value RunAccompaniment(Ort::Value x) const;

  const OfflineSourceSeparationSpleeterModelMetaData &GetMetaData() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_H_
