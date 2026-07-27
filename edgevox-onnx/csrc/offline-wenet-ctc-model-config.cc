// edgevox-onnx/csrc/offline-wenet-ctc-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-wenet-ctc-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineWenetCtcModelConfig::Register(ParseOptions *po) {
  po->Register(
      "wenet-ctc-model", &model,
      "Path to model.onnx from WeNet. Please see "
      "https://github.com/k2-fsa/edgevox-onnx/pull/425 for available models");
}

bool OfflineWenetCtcModelConfig::Validate() const {
  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("WeNet model: '%s' does not exist", model.c_str());
    return false;
  }

  return true;
}

std::string OfflineWenetCtcModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineWenetCtcModelConfig(";
  os << "model=\"" << model << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
