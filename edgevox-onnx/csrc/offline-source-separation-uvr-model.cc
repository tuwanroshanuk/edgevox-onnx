// edgevox-onnx/csrc/offline-source-separation-uvr-model.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-source-separation-uvr-model.h"
#include "edgevox-onnx/csrc/ort-env.h"
#include "edgevox-onnx/csrc/macros.h"

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
#include "edgevox-onnx/csrc/onnx-utils.h"
#include "edgevox-onnx/csrc/session.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {

class OfflineSourceSeparationUvrModel::Impl {
 public:
  explicit Impl(const OfflineSourceSeparationModelConfig &config)
      : config_(config),
        env_(CreateOrtEnv()),
        sess_opts_(GetSessionOptions(config)),
        allocator_{} {
    sess_ = std::make_unique<Ort::Session>(
        env_, EDGEVOX_ONNX_TO_ORT_PATH(config.uvr.model), sess_opts_);
    Init(nullptr, 0);
  }

  template <typename Manager>
  Impl(Manager *mgr, const OfflineSourceSeparationModelConfig &config)
      : config_(config),
        env_(CreateOrtEnv()),
        sess_opts_(GetSessionOptions(config)),
        allocator_{} {
    auto buf = ReadFile(mgr, config.uvr.model);
    Init(buf.data(), buf.size());
  }

  const OfflineSourceSeparationUvrModelMetaData &GetMetaData() const {
    return meta_;
  }

  Ort::Value Run(Ort::Value x) const {
    auto out = sess_->Run({}, input_names_ptr_.data(), &x, 1,
                          output_names_ptr_.data(), output_names_ptr_.size());
    return std::move(out[0]);
  }

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

    Ort::ModelMetadata meta_data = sess_->GetModelMetadata();
    if (config_.debug) {
      std::ostringstream os;
      os << "---UVR model---\n";
      PrintModelMetadata(os, meta_data);

      os << "----------input names----------\n";
      int32_t i = 0;
      for (const auto &s : input_names_) {
        os << i << " " << s << "\n";
        ++i;
      }
      os << "----------output names----------\n";
      i = 0;
      for (const auto &s : output_names_) {
        os << i << " " << s << "\n";
        ++i;
      }

#if __OHOS__
      EDGEVOX_ONNX_LOGE("%{public}s\n", os.str().c_str());
#else
      EDGEVOX_ONNX_LOGE("%s\n", os.str().c_str());
#endif
    }

    Ort::AllocatorWithDefaultOptions allocator;  // used in the macro below

    std::string model_type;
    EDGEVOX_ONNX_READ_META_DATA_STR(model_type, "model_type");
    if (model_type != "UVR") {
      EDGEVOX_ONNX_LOGE("Expect model type 'UVR'. Given: '%s'",
                       model_type.c_str());
      EDGEVOX_ONNX_EXIT(-1);
    }

    EDGEVOX_ONNX_READ_META_DATA(meta_.num_stems, "stems");
    if (meta_.num_stems != 2) {
      EDGEVOX_ONNX_LOGE("Only 2stems is supported. Given %d stems",
                       meta_.num_stems);
      EDGEVOX_ONNX_EXIT(-1);
    }

    EDGEVOX_ONNX_READ_META_DATA(meta_.sample_rate, "sample_rate");
    EDGEVOX_ONNX_READ_META_DATA(meta_.n_fft, "n_fft");
    EDGEVOX_ONNX_READ_META_DATA(meta_.center, "center");
    EDGEVOX_ONNX_READ_META_DATA(meta_.window_length, "win_length");
    EDGEVOX_ONNX_READ_META_DATA(meta_.hop_length, "hop_length");
    EDGEVOX_ONNX_READ_META_DATA(meta_.dim_t, "dim_t");
    EDGEVOX_ONNX_READ_META_DATA(meta_.dim_f, "dim_f");
    EDGEVOX_ONNX_READ_META_DATA(meta_.dim_c, "dim_c");
    EDGEVOX_ONNX_READ_META_DATA_STR(meta_.window_type, "window_type");

    meta_.margin = meta_.sample_rate;
  }

 private:
  OfflineSourceSeparationModelConfig config_;
  OfflineSourceSeparationUvrModelMetaData meta_;

  Ort::Env env_;
  Ort::SessionOptions sess_opts_;
  Ort::AllocatorWithDefaultOptions allocator_;

  std::unique_ptr<Ort::Session> sess_;

  std::vector<std::string> input_names_;
  std::vector<const char *> input_names_ptr_;

  std::vector<std::string> output_names_;
  std::vector<const char *> output_names_ptr_;
};

OfflineSourceSeparationUvrModel::~OfflineSourceSeparationUvrModel() = default;

OfflineSourceSeparationUvrModel::OfflineSourceSeparationUvrModel(
    const OfflineSourceSeparationModelConfig &config)
    : impl_(std::make_unique<Impl>(config)) {}

template <typename Manager>
OfflineSourceSeparationUvrModel::OfflineSourceSeparationUvrModel(
    Manager *mgr, const OfflineSourceSeparationModelConfig &config)
    : impl_(std::make_unique<Impl>(mgr, config)) {}

Ort::Value OfflineSourceSeparationUvrModel::Run(Ort::Value x) const {
  return impl_->Run(std::move(x));
}

const OfflineSourceSeparationUvrModelMetaData &
OfflineSourceSeparationUvrModel::GetMetaData() const {
  return impl_->GetMetaData();
}

#if __ANDROID_API__ >= 9
template OfflineSourceSeparationUvrModel::OfflineSourceSeparationUvrModel(
    AAssetManager *mgr, const OfflineSourceSeparationModelConfig &config);
#endif

#if __OHOS__
template OfflineSourceSeparationUvrModel::OfflineSourceSeparationUvrModel(
    NativeResourceManager *mgr,
    const OfflineSourceSeparationModelConfig &config);
#endif

}  // namespace edgevox_onnx
