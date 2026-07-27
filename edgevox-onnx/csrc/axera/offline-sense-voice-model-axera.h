// edgevox-onnx/csrc/axera/offline-sense-voice-model-axera.h
//
// Copyright (c)  2025  M5Stack Technology CO LTD

#ifndef EDGEVOX_ONNX_CSRC_AXERA_OFFLINE_SENSE_VOICE_MODEL_AXERA_H_
#define EDGEVOX_ONNX_CSRC_AXERA_OFFLINE_SENSE_VOICE_MODEL_AXERA_H_

#include <memory>
#include <vector>

#include "edgevox-onnx/csrc/offline-model-config.h"
#include "edgevox-onnx/csrc/offline-sense-voice-model-meta-data.h"

namespace edgevox_onnx {

class OfflineSenseVoiceModelAxera {
 public:
  ~OfflineSenseVoiceModelAxera();

  explicit OfflineSenseVoiceModelAxera(const OfflineModelConfig &config);

  template <typename Manager>
  OfflineSenseVoiceModelAxera(Manager *mgr, const OfflineModelConfig &config);

  std::vector<float> Run(std::vector<float> features, int32_t language,
                         int32_t text_norm) const;

  const OfflineSenseVoiceModelMetaData &GetModelMetadata() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_AXERA_OFFLINE_SENSE_VOICE_MODEL_AXERA_H_
