// edgevox-onnx/csrc/offline-speech-denoiser-gtcrn-model-config.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_GTCRN_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_GTCRN_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineSpeechDenoiserGtcrnModelConfig {
  std::string model;
  OfflineSpeechDenoiserGtcrnModelConfig() = default;

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_GTCRN_MODEL_CONFIG_H_
