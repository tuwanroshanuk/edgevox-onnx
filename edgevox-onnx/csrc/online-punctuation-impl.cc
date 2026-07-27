// edgevox-onnx/csrc/online-punctuation-impl.cc
//
// Copyright (c) 2024 Jian You (jianyou@cisco.com, Cisco Systems)

#include "edgevox-onnx/csrc/online-punctuation-impl.h"

#include <memory>

#if __ANDROID_API__ >= 9
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#endif

#if __OHOS__
#include "rawfile/raw_file_manager.h"
#endif

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/online-punctuation-cnn-bilstm-impl.h"

namespace edgevox_onnx {

std::unique_ptr<OnlinePunctuationImpl> OnlinePunctuationImpl::Create(
    const OnlinePunctuationConfig &config) {
  if (!config.model.cnn_bilstm.empty() && !config.model.bpe_vocab.empty()) {
    return std::make_unique<OnlinePunctuationCNNBiLSTMImpl>(config);
  }

  EDGEVOX_ONNX_LOGE(
      "Please specify a punctuation model and bpe vocab! Return a null "
      "pointer");
  return nullptr;
}

template <typename Manager>
std::unique_ptr<OnlinePunctuationImpl> OnlinePunctuationImpl::Create(
    Manager *mgr, const OnlinePunctuationConfig &config) {
  if (!config.model.cnn_bilstm.empty() && !config.model.bpe_vocab.empty()) {
    return std::make_unique<OnlinePunctuationCNNBiLSTMImpl>(mgr, config);
  }

  EDGEVOX_ONNX_LOGE(
      "Please specify a punctuation model and bpe vocab! Return a null "
      "pointer");
  return nullptr;
}

#if __ANDROID_API__ >= 9
template std::unique_ptr<OnlinePunctuationImpl> OnlinePunctuationImpl::Create(
    AAssetManager *mgr, const OnlinePunctuationConfig &config);
#endif

#if __OHOS__
template std::unique_ptr<OnlinePunctuationImpl> OnlinePunctuationImpl::Create(
    NativeResourceManager *mgr, const OnlinePunctuationConfig &config);
#endif

}  // namespace edgevox_onnx
