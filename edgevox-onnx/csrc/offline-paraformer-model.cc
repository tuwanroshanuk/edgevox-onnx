// edgevox-onnx/csrc/offline-paraformer-model.cc
//
// Copyright (c)  2022-2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-paraformer-model.h"
#include "edgevox-onnx/csrc/ort-env.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#if __ANDROID_API__ >= 9
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#endif

#if __OHOS__
#include "rawfile/raw_file_manager.h"
#endif

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/onnx-utils.h"
#include "edgevox-onnx/csrc/session.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {

class OfflineParaformerModel::Impl {
 public:
  explicit Impl(const OfflineModelConfig &config)
      : config_(config),
        env_(CreateOrtEnv()),
        sess_opts_(GetSessionOptions(config)),
        allocator_{} {
    sess_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config_.paraformer.model), sess_opts_);
    Init(nullptr, 0);
  }

  template <typename Manager>
  Impl(Manager *mgr, const OfflineModelConfig &config)
      : config_(config),
        env_(CreateOrtEnv()),
        sess_opts_(GetSessionOptions(config)),
        allocator_{} {
    auto buf = ReadFile(mgr, config_.paraformer.model);
    Init(buf.data(), buf.size());
  }

  std::vector<Ort::Value> Forward(Ort::Value features,
                                  Ort::Value features_length) {
    std::array<Ort::Value, 2> inputs = {std::move(features),
                                        std::move(features_length)};

    return sess_->Run({}, input_names_ptr_.data(), inputs.data(), inputs.size(),
                      output_names_ptr_.data(), output_names_ptr_.size());
  }

  int32_t VocabSize() const { return vocab_size_; }

  int32_t LfrWindowSize() const { return lfr_window_size_; }

  int32_t LfrWindowShift() const { return lfr_window_shift_; }

  const std::vector<float> &NegativeMean() const { return neg_mean_; }

  const std::vector<float> &InverseStdDev() const { return inv_stddev_; }

  OrtAllocator *Allocator() { return allocator_; }

 private:
  void Init(void *model_data, size_t model_data_length) {
    if (model_data) {
      sess_ = std::make_unique<Ort::Session>(
          env_, model_data, model_data_length, sess_opts_);
    } else if (!sess_) {
      EDGEVOX_ONNX_LOGE(
          "Please pass model data or initialize the session outside of "
          "this function");
      EDGEVOX_ONNX_EXIT(-1);
    }

    GetInputNames(sess_.get(), &input_names_, &input_names_ptr_);

    GetOutputNames(sess_.get(), &output_names_, &output_names_ptr_);

    // get meta data
    Ort::ModelMetadata meta_data = sess_->GetModelMetadata();
    if (config_.debug) {
      std::ostringstream os;
      PrintModelMetadata(os, meta_data);
#if __OHOS__
      EDGEVOX_ONNX_LOGE("%{public}s\n", os.str().c_str());
#else
      EDGEVOX_ONNX_LOGE("%s\n", os.str().c_str());
#endif
    }

    Ort::AllocatorWithDefaultOptions allocator;  // used in the macro below
    EDGEVOX_ONNX_READ_META_DATA(vocab_size_, "vocab_size");
    EDGEVOX_ONNX_READ_META_DATA(lfr_window_size_, "lfr_window_size");
    EDGEVOX_ONNX_READ_META_DATA(lfr_window_shift_, "lfr_window_shift");

    EDGEVOX_ONNX_READ_META_DATA_VEC_FLOAT(neg_mean_, "neg_mean");
    EDGEVOX_ONNX_READ_META_DATA_VEC_FLOAT(inv_stddev_, "inv_stddev");
  }

 private:
  OfflineModelConfig config_;
  Ort::Env env_;
  Ort::SessionOptions sess_opts_;
  Ort::AllocatorWithDefaultOptions allocator_;

  std::unique_ptr<Ort::Session> sess_;

  std::vector<std::string> input_names_;
  std::vector<const char *> input_names_ptr_;

  std::vector<std::string> output_names_;
  std::vector<const char *> output_names_ptr_;

  std::vector<float> neg_mean_;
  std::vector<float> inv_stddev_;

  int32_t vocab_size_ = 0;  // initialized in Init
  int32_t lfr_window_size_ = 0;
  int32_t lfr_window_shift_ = 0;
};

OfflineParaformerModel::OfflineParaformerModel(const OfflineModelConfig &config)
    : impl_(std::make_unique<Impl>(config)) {}

template <typename Manager>
OfflineParaformerModel::OfflineParaformerModel(Manager *mgr,
                                               const OfflineModelConfig &config)
    : impl_(std::make_unique<Impl>(mgr, config)) {}

OfflineParaformerModel::~OfflineParaformerModel() = default;

std::vector<Ort::Value> OfflineParaformerModel::Forward(
    Ort::Value features, Ort::Value features_length) {
  return impl_->Forward(std::move(features), std::move(features_length));
}

int32_t OfflineParaformerModel::VocabSize() const { return impl_->VocabSize(); }

int32_t OfflineParaformerModel::LfrWindowSize() const {
  return impl_->LfrWindowSize();
}
int32_t OfflineParaformerModel::LfrWindowShift() const {
  return impl_->LfrWindowShift();
}
const std::vector<float> &OfflineParaformerModel::NegativeMean() const {
  return impl_->NegativeMean();
}
const std::vector<float> &OfflineParaformerModel::InverseStdDev() const {
  return impl_->InverseStdDev();
}

OrtAllocator *OfflineParaformerModel::Allocator() const {
  return impl_->Allocator();
}

#if __ANDROID_API__ >= 9
template OfflineParaformerModel::OfflineParaformerModel(
    AAssetManager *mgr, const OfflineModelConfig &config);
#endif

#if __OHOS__
template OfflineParaformerModel::OfflineParaformerModel(
    NativeResourceManager *mgr, const OfflineModelConfig &config);
#endif

}  // namespace edgevox_onnx
