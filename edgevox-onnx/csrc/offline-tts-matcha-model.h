// edgevox-onnx/csrc/offline-tts-matcha-model.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_MATCHA_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_MATCHA_MODEL_H_

#include <memory>
#include <string>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-tts-matcha-model-meta-data.h"
#include "edgevox-onnx/csrc/offline-tts-model-config.h"

namespace edgevox_onnx {

class OfflineTtsMatchaModel {
 public:
  ~OfflineTtsMatchaModel();

  explicit OfflineTtsMatchaModel(const OfflineTtsModelConfig &config);

  template <typename Manager>
  OfflineTtsMatchaModel(Manager *mgr, const OfflineTtsModelConfig &config);

  // Return a float32 tensor containing the mel
  // of shape (batch_size, mel_dim, num_frames)
  Ort::Value Run(Ort::Value x, int64_t sid = 0, float speed = 1.0) const;

  const OfflineTtsMatchaModelMetaData &GetMetaData() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_MATCHA_MODEL_H_
