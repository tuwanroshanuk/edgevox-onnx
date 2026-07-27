// edgevox-onnx/csrc/ascend/offline-sense-voice-model-ascend.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_ASCEND_OFFLINE_SENSE_VOICE_MODEL_ASCEND_H_
#define EDGEVOX_ONNX_CSRC_ASCEND_OFFLINE_SENSE_VOICE_MODEL_ASCEND_H_

#include <memory>
#include <vector>

#include "edgevox-onnx/csrc/offline-model-config.h"
#include "edgevox-onnx/csrc/offline-sense-voice-model-meta-data.h"

namespace edgevox_onnx {

class OfflineSenseVoiceModelAscend {
 public:
  ~OfflineSenseVoiceModelAscend();

  explicit OfflineSenseVoiceModelAscend(const OfflineModelConfig &config);

  template <typename Manager>
  OfflineSenseVoiceModelAscend(Manager *mgr, const OfflineModelConfig &config);

  /**
   * @param features A tensor of shape (num_frames, feature_dim)
   *                 before applying LFR.
   * @param language
   * @param text_norm
   * @returns Return a tensor of shape (num_output_frames, vocab_size)
   */
  std::vector<float> Run(std::vector<float> features, int32_t language,
                         int32_t text_norm) const;

  const OfflineSenseVoiceModelMetaData &GetModelMetadata() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ASCEND_OFFLINE_SENSE_VOICE_MODEL_ASCEND_H_
