// edgevox-onnx/csrc/offline-tts-chatterbox-model.h
//
// Copyright (c) 2026 Nexus Cloud

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-model-config.h"

namespace edgevox_onnx {

class OfflineTtsChatterboxModel {
 public:
  explicit OfflineTtsChatterboxModel(const OfflineTtsModelConfig &config);
  template <typename Manager>
  OfflineTtsChatterboxModel(Manager *mgr,
                            const OfflineTtsModelConfig &config);
  ~OfflineTtsChatterboxModel();

  std::vector<int64_t> Tokenize(const std::string &text);

  std::vector<float> Generate(const std::vector<int64_t> &text_tokens,
                              const std::vector<float> &reference_audio,
                              int32_t max_new_tokens,
                              float repetition_penalty) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_H_
