// edgevox-onnx/csrc/axcl/axcl-engine-guard.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_GUARD_H_
#define EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_GUARD_H_
#include "axcl.h"  // NOLINT

namespace edgevox_onnx {

class AxclEngineGuard {
 public:
  explicit AxclEngineGuard(axclrtEngineVNpuKind npuKind = AXCL_VNPU_DISABLE);
  ~AxclEngineGuard();

  AxclEngineGuard(const AxclEngineGuard &) = delete;
  AxclEngineGuard &operator=(const AxclEngineGuard &) = delete;
  AxclEngineGuard(AxclEngineGuard &&) = delete;
  AxclEngineGuard &operator=(AxclEngineGuard &&) = delete;

 private:
  bool initialized_ = false;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_GUARD_H_
