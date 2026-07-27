// edgevox-onnx/csrc/vocos-vocoder.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_VOCOS_VOCODER_H_
#define EDGEVOX_ONNX_CSRC_VOCOS_VOCODER_H_

#include <memory>
#include <string>
#include <vector>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-tts-model-config.h"
#include "edgevox-onnx/csrc/vocoder.h"

namespace edgevox_onnx {

class VocosVocoder : public Vocoder {
 public:
  ~VocosVocoder() override;

  explicit VocosVocoder(const OfflineTtsModelConfig &config);

  template <typename Manager>
  VocosVocoder(Manager *mgr, const OfflineTtsModelConfig &config);

  /** @param mel A float32 tensor of shape (batch_size, feat_dim, num_frames).
   *  @return Return a float32 tensor of shape (batch_size, num_samples).
   */
  std::vector<float> Run(Ort::Value mel) const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_VOCOS_VOCODER_H_
