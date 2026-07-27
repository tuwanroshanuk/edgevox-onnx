// edgevox-onnx/csrc/axcl/axcl-engine-io-info-guard.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/axcl/axcl-engine-io-info-guard.h"

#include <cstdint>

#include "axcl.h"  // NOLINT
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

AxclEngineIOInfoGuard::AxclEngineIOInfoGuard(uint64_t model_id) {
  axclError ret = axclrtEngineGetIOInfo(model_id, &io_info_);
  if (ret != 0) {
    EDGEVOX_ONNX_LOGE(
        "Failed to call axclrtEngineGetIOInfo(). Return code is: %d",
        static_cast<int32_t>(ret));
    EDGEVOX_ONNX_EXIT(-1);
  }

  initialized_ = true;
}

AxclEngineIOInfoGuard::~AxclEngineIOInfoGuard() {
  if (initialized_) {
    auto ret = axclrtEngineDestroyIOInfo(io_info_);

    if (ret != 0) {
      EDGEVOX_ONNX_LOGE(
          "Failed to call axclrtEngineDestroyIOInfo(). Return code is: %d",
          static_cast<int32_t>(ret));
      EDGEVOX_ONNX_EXIT(-1);
    }
  }
}

}  // namespace edgevox_onnx
