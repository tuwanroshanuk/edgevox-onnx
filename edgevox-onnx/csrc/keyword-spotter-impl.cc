// edgevox-onnx/csrc/keyword-spotter-impl.cc
//
// Copyright (c)  2023-2024  Xiaomi Corporation

#include "edgevox-onnx/csrc/keyword-spotter-impl.h"

#include <memory>

#if __ANDROID_API__ >= 9
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#endif

#if __OHOS__
#include "rawfile/raw_file_manager.h"
#endif

#include "edgevox-onnx/csrc/keyword-spotter-transducer-impl.h"
#include "edgevox-onnx/csrc/macros.h"

#if EDGEVOX_ONNX_ENABLE_RKNN
#include "edgevox-onnx/csrc/rknn/keyword-spotter-transducer-rknn-impl.h"
#endif

namespace edgevox_onnx {

std::unique_ptr<KeywordSpotterImpl> KeywordSpotterImpl::Create(
    const KeywordSpotterConfig &config) {
  if (config.model_config.provider_config.provider == "rknn") {
#if EDGEVOX_ONNX_ENABLE_RKNN
    if (!config.model_config.transducer.encoder.empty()) {
      return std::make_unique<KeywordSpotterTransducerRknnImpl>(config);
    }
#else
    EDGEVOX_ONNX_LOGE(
        "Please rebuild edgevox-onnx with -DEDGEVOX_ONNX_ENABLE_RKNN=ON if you "
        "want to use rknn.");
    EDGEVOX_ONNX_EXIT(-1);
    return nullptr;
#endif
  }

  if (!config.model_config.transducer.encoder.empty()) {
    return std::make_unique<KeywordSpotterTransducerImpl>(config);
  }

  EDGEVOX_ONNX_LOGE("Please specify a model");
  EDGEVOX_ONNX_EXIT(-1);
}

template <typename Manager>
std::unique_ptr<KeywordSpotterImpl> KeywordSpotterImpl::Create(
    Manager *mgr, const KeywordSpotterConfig &config) {
  if (config.model_config.provider_config.provider == "rknn") {
#if EDGEVOX_ONNX_ENABLE_RKNN
    if (!config.model_config.transducer.encoder.empty()) {
      return std::make_unique<KeywordSpotterTransducerRknnImpl>(mgr, config);
    }
#else
    EDGEVOX_ONNX_LOGE(
        "Please rebuild edgevox-onnx with -DEDGEVOX_ONNX_ENABLE_RKNN=ON if you "
        "want to use rknn.");
    EDGEVOX_ONNX_EXIT(-1);
    return nullptr;
#endif
  }

  if (!config.model_config.transducer.encoder.empty()) {
    return std::make_unique<KeywordSpotterTransducerImpl>(mgr, config);
  }

  EDGEVOX_ONNX_LOGE("Please specify a model");
  EDGEVOX_ONNX_EXIT(-1);
}

#if __ANDROID_API__ >= 9
template std::unique_ptr<KeywordSpotterImpl> KeywordSpotterImpl::Create(
    AAssetManager *mgr, const KeywordSpotterConfig &config);
#endif

#if __OHOS__
template std::unique_ptr<KeywordSpotterImpl> KeywordSpotterImpl::Create(
    NativeResourceManager *mgr, const KeywordSpotterConfig &config);
#endif

}  // namespace edgevox_onnx
