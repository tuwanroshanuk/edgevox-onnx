// edgevox-onnx/csrc/offline-tts-openvoice.h
//
// Copyright (c)  2026  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_OPENVOICE_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_OPENVOICE_H_

#include <memory>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-model-config.h"

namespace edgevox_onnx {

class MemoryResourceManager;

// Exposed for deterministic preprocessing parity tests.
std::vector<float> ComputeOpenVoiceLinearSpectrogramForTest(
    const std::vector<float> &audio, int64_t *num_frames);

// Optional OpenVoice V2 tone-color post-converter for generated TTS audio.
class OfflineTtsOpenVoice {
 public:
  explicit OfflineTtsOpenVoice(const OfflineTtsModelConfig &config);
  OfflineTtsOpenVoice(MemoryResourceManager *mgr,
                      const OfflineTtsModelConfig &config);
  ~OfflineTtsOpenVoice();

  std::vector<float> Convert(const std::vector<float> &source,
                             int32_t source_sample_rate,
                             const std::vector<float> &reference,
                             int32_t reference_sample_rate) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_OPENVOICE_H_
