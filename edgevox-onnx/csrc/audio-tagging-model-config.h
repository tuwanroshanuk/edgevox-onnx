// edgevox-onnx/csrc/audio-tagging-model-config.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_AUDIO_TAGGING_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_AUDIO_TAGGING_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/offline-zipformer-audio-tagging-model-config.h"
#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct AudioTaggingModelConfig {
  struct OfflineZipformerAudioTaggingModelConfig zipformer;
  std::string ced;

  int32_t num_threads = 1;
  bool debug = false;
  std::string provider = "cpu";

  AudioTaggingModelConfig() = default;

  AudioTaggingModelConfig(
      const OfflineZipformerAudioTaggingModelConfig &zipformer,
      const std::string &ced, int32_t num_threads, bool debug,
      const std::string &provider)
      : zipformer(zipformer),
        ced(ced),
        num_threads(num_threads),
        debug(debug),
        provider(provider) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_AUDIO_TAGGING_MODEL_CONFIG_H_
