// edgevox-onnx/csrc/offline-source-separation-impl.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_IMPL_H_

#include <memory>
#include <vector>

#include "edgevox-onnx/csrc/offline-source-separation.h"

namespace edgevox_onnx {

class OfflineSourceSeparationImpl {
 public:
  static std::unique_ptr<OfflineSourceSeparationImpl> Create(
      const OfflineSourceSeparationConfig &config);

  template <typename Manager>
  static std::unique_ptr<OfflineSourceSeparationImpl> Create(
      Manager *mgr, const OfflineSourceSeparationConfig &config);

  virtual ~OfflineSourceSeparationImpl() = default;

  virtual OfflineSourceSeparationOutput Process(
      const OfflineSourceSeparationInput &input) const = 0;

  virtual int32_t GetOutputSampleRate() const = 0;

  virtual int32_t GetNumberOfStems() const = 0;

  OfflineSourceSeparationInput Resample(
      const OfflineSourceSeparationInput &input, bool debug = false) const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_IMPL_H_
