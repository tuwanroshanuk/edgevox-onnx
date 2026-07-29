// edgevox-onnx/csrc/offline-tts-chatterbox-model-config.h
//
// Copyright (c) 2026 Nexus Cloud

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineTtsChatterboxModelConfig {
  std::string speech_encoder;
  std::string embed_tokens;
  std::string language_model;
  std::string conditional_decoder;
  std::string tokenizer;

  void Register(ParseOptions *po);
  bool Validate() const;
  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_CONFIG_H_
