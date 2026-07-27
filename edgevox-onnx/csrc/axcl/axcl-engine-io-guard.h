// edgevox-onnx/csrc/axcl/axcl-engine-io-guard.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_IO_GUARD_H_
#define EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_IO_GUARD_H_
#include "axcl.h"  // NOLINT

namespace edgevox_onnx {

class AxclEngineIOGuard {
 public:
  explicit AxclEngineIOGuard(axclrtEngineIOInfo io_info);
  ~AxclEngineIOGuard();

  AxclEngineIOGuard(const AxclEngineIOGuard &) = delete;
  AxclEngineIOGuard &operator=(const AxclEngineIOGuard &) = delete;
  AxclEngineIOGuard(AxclEngineIOGuard &&) = delete;
  AxclEngineIOGuard &operator=(AxclEngineIOGuard &&) = delete;

  operator axclrtEngineIO() { return io_; }

 private:
  bool initialized_ = false;
  axclrtEngineIO io_ = nullptr;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_AXCL_AXCL_ENGINE_IO_GUARD_H_
