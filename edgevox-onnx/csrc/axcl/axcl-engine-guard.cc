// edgevox-onnx/csrc/axcl/axcl-engine-guard.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/axcl/axcl-engine-guard.h"

#include <cstdint>

#include "axcl.h"  // NOLINT
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

AxclEngineGuard::AxclEngineGuard(
    axclrtEngineVNpuKind npuKind /*= AXCL_VNPU_DISABLE*/) {
  axclError ret = axclrtEngineInit(npuKind);
  if (ret != 0) {
    EDGEVOX_ONNX_LOGE("Failed to call axclrtEngineInit(). Return code is: %d",
                     static_cast<int32_t>(ret));
    EDGEVOX_ONNX_EXIT(-1);
  }

  initialized_ = true;
}

AxclEngineGuard::~AxclEngineGuard() {
  if (initialized_) {
    auto ret = axclrtEngineFinalize();

    if (ret != 0) {
      EDGEVOX_ONNX_LOGE(
          "Failed to call axclrtEngineFinalize(). Return code is: %d",
          static_cast<int32_t>(ret));
      EDGEVOX_ONNX_EXIT(-1);
    }
  }
}

}  // namespace edgevox_onnx
