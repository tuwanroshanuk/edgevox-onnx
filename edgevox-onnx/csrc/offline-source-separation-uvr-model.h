// edgevox-onnx/csrc/offline-source-separation-uvr-model.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_H_
#include <memory>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-source-separation-model-config.h"
#include "edgevox-onnx/csrc/offline-source-separation-uvr-model-meta-data.h"

namespace edgevox_onnx {

class OfflineSourceSeparationUvrModel {
 public:
  ~OfflineSourceSeparationUvrModel();

  explicit OfflineSourceSeparationUvrModel(
      const OfflineSourceSeparationModelConfig &config);

  template <typename Manager>
  OfflineSourceSeparationUvrModel(
      Manager *mgr, const OfflineSourceSeparationModelConfig &config);

  Ort::Value Run(Ort::Value x) const;

  const OfflineSourceSeparationUvrModelMetaData &GetMetaData() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_UVR_MODEL_H_
