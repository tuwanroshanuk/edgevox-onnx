// edgevox-onnx/csrc/offline-tts-kokoro-model.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KOKORO_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KOKORO_MODEL_H_

#include <memory>
#include <string>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-tts-kokoro-model-meta-data.h"
#include "edgevox-onnx/csrc/offline-tts-model-config.h"

namespace edgevox_onnx {

class OfflineTtsKokoroModel {
 public:
  ~OfflineTtsKokoroModel();

  explicit OfflineTtsKokoroModel(const OfflineTtsModelConfig &config);

  template <typename Manager>
  OfflineTtsKokoroModel(Manager *mgr, const OfflineTtsModelConfig &config);

  // Return a float32 tensor containing the samples
  // of shape (batch_size, num_samples)
  Ort::Value Run(Ort::Value x, int64_t sid = 0, float speed = 1.0) const;

  const OfflineTtsKokoroModelMetaData &GetMetaData() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_KOKORO_MODEL_H_
