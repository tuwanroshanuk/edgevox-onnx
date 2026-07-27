// edgevox-onnx/csrc/offline-speech-denoiser-dpdfnet-model-config.h
//
// Copyright (c)  2026  Ceva Inc
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_DPDFNET_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_DPDFNET_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineSpeechDenoiserDpdfNetModelConfig {
  std::string model;
  OfflineSpeechDenoiserDpdfNetModelConfig() = default;

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_DPDFNET_MODEL_CONFIG_H_
