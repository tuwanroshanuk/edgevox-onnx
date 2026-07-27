// edgevox-onnx/csrc/offline-fire-red-asr-ctc-model-config.cc
//
// Copyright (c)  2026  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-fire-red-asr-ctc-model-config.h"

#include <sstream>
#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineFireRedAsrCtcModelConfig::Register(ParseOptions *po) {
  po->Register(
      "fire-red-asr-ctc", &model,
      "Path to model.onnx from FireRedASR CTC. "
      "Please see "
      "https://k2-fsa.github.io/sherpa/onnx/FireRedAsr/pretrained.html "
      "for available FireRedASR CTC models");
}

bool OfflineFireRedAsrCtcModelConfig::Validate() const {
  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("FireRedASR CTC model: '%s' does not exist",
                     model.c_str());
    return false;
  }

  return true;
}

std::string OfflineFireRedAsrCtcModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineFireRedAsrCtcModelConfig(";
  os << "model=\"" << model << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
