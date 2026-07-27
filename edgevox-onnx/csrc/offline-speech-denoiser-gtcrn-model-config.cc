// edgevox-onnx/csrc/offline-speech-denoiser-gtcrn-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-speech-denoiser-gtcrn-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineSpeechDenoiserGtcrnModelConfig::Register(ParseOptions *po) {
  po->Register("speech-denoiser-gtcrn-model", &model,
               "Path to the gtcrn model for speech denoising");
}

bool OfflineSpeechDenoiserGtcrnModelConfig::Validate() const {
  if (model.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --speech-denoiser-gtcrn-model");
    return false;
  }

  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("gtcrn model file '%s' does not exist", model.c_str());
    return false;
  }
  return true;
}

std::string OfflineSpeechDenoiserGtcrnModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineSpeechDenoiserGtcrnModelConfig(";
  os << "model=\"" << model << "\")";
  return os.str();
}

}  // namespace edgevox_onnx
