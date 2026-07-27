// edgevox-onnx/csrc/axcl/axcl-manager.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/axcl/axcl-manager.h"

#include <cstdint>

#include "axcl.h"  // NOLINT
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

std::mutex AxclManager::mutex_;

int32_t AxclManager::count_{0};

AxclManager::AxclManager(const char *config /*= nullptr*/) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (count_ == 0) {
    auto ret = axclInit(config);
    if (ret != 0) {
      EDGEVOX_ONNX_LOGE("Failed to call axclInit(). Return code: %d",
                       static_cast<int32_t>(ret));
      EDGEVOX_ONNX_EXIT(-1);
    }
  }

  ++count_;
}

AxclManager::~AxclManager() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (--count_ == 0) {
    auto ret = axclFinalize();

    if (ret != 0) {
      EDGEVOX_ONNX_LOGE("Failed to call axclFinalize(). Return code: %d",
                       static_cast<int32_t>(ret));
      EDGEVOX_ONNX_EXIT(-1);
    }
  }
}

}  // namespace edgevox_onnx
