// edgevox-onnx/csrc/vad-model.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/vad-model.h"

#include <memory>

#if __ANDROID_API__ >= 9
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#endif

#if __OHOS__
#include "rawfile/raw_file_manager.h"
#endif

#if EDGEVOX_ONNX_ENABLE_RKNN
#include "edgevox-onnx/csrc/rknn/silero-vad-model-rknn.h"
#endif

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/silero-vad-model.h"
#include "edgevox-onnx/csrc/ten-vad-model.h"

namespace edgevox_onnx {

std::unique_ptr<VadModel> VadModel::Create(const VadModelConfig &config) {
  if (config.provider == "rknn") {
#if EDGEVOX_ONNX_ENABLE_RKNN
    if (!config.silero_vad.model.empty()) {
      return std::make_unique<SileroVadModelRknn>(config);
    } else {
      EDGEVOX_ONNX_LOGE("Only silero-vad is supported for RKNN at present");
      EDGEVOX_ONNX_EXIT(-1);
    }
#else
    EDGEVOX_ONNX_LOGE(
        "Please rebuild edgevox-onnx with -DEDGEVOX_ONNX_ENABLE_RKNN=ON if you "
        "want to use rknn.");
    EDGEVOX_ONNX_EXIT(-1);
    return nullptr;
#endif
  }

  if (!config.silero_vad.model.empty()) {
    return std::make_unique<SileroVadModel>(config);
  }

  if (!config.ten_vad.model.empty()) {
    return std::make_unique<TenVadModel>(config);
  }

  EDGEVOX_ONNX_LOGE("Please provide a vad model");
  return nullptr;
}

template <typename Manager>
std::unique_ptr<VadModel> VadModel::Create(Manager *mgr,
                                           const VadModelConfig &config) {
  if (config.provider == "rknn") {
#if EDGEVOX_ONNX_ENABLE_RKNN
    if (!config.silero_vad.model.empty()) {
      return std::make_unique<SileroVadModelRknn>(mgr, config);
    } else {
      EDGEVOX_ONNX_LOGE("Only silero-vad is supported for RKNN at present");
      EDGEVOX_ONNX_EXIT(-1);
    }
#else
    EDGEVOX_ONNX_LOGE(
        "Please rebuild edgevox-onnx with -DEDGEVOX_ONNX_ENABLE_RKNN=ON if you "
        "want to use rknn.");
    EDGEVOX_ONNX_EXIT(-1);
    return nullptr;
#endif
  }
  if (!config.silero_vad.model.empty()) {
    return std::make_unique<SileroVadModel>(mgr, config);
  }

  if (!config.ten_vad.model.empty()) {
    return std::make_unique<TenVadModel>(mgr, config);
  }

  EDGEVOX_ONNX_LOGE("Please provide a vad model");
  return nullptr;
}

#if __ANDROID_API__ >= 9
template std::unique_ptr<VadModel> VadModel::Create(
    AAssetManager *mgr, const VadModelConfig &config);
#endif

#if __OHOS__
template std::unique_ptr<VadModel> VadModel::Create(
    NativeResourceManager *mgr, const VadModelConfig &config);
#endif
}  // namespace edgevox_onnx
