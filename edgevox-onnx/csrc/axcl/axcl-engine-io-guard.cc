// edgevox-onnx/csrc/axcl/axcl-engine-io-guard.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/axcl/axcl-engine-io-guard.h"

#include <cstdint>

#include "axcl.h"  // NOLINT
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

AxclEngineIOGuard::AxclEngineIOGuard(axclrtEngineIOInfo io_info) {
  axclError ret = axclrtEngineCreateIO(io_info, &io_);
  if (ret != 0) {
    EDGEVOX_ONNX_LOGE(
        "Failed to call axclrtEngineCreateIO(). Return code is: %d",
        static_cast<int32_t>(ret));
    EDGEVOX_ONNX_EXIT(-1);
  }

  initialized_ = true;
}

AxclEngineIOGuard::~AxclEngineIOGuard() {
  if (initialized_) {
    auto ret = axclrtEngineDestroyIO(io_);

    if (ret != 0) {
      EDGEVOX_ONNX_LOGE(
          "Failed to call axclrtEngineDestroyIO(). Return code is: %d",
          static_cast<int32_t>(ret));
      EDGEVOX_ONNX_EXIT(-1);
    }
  }
}

}  // namespace edgevox_onnx
