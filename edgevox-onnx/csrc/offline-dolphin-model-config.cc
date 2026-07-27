// edgevox-onnx/csrc/offline-dolphin-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-dolphin-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineDolphinModelConfig::Register(ParseOptions *po) {
  po->Register("dolphin-model", &model,
               "Path to model.onnx of Dolphin CTC branch.");
}

bool OfflineDolphinModelConfig::Validate() const {
  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("Dolphin model '%s' does not exist", model.c_str());
    return false;
  }

  return true;
}

std::string OfflineDolphinModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineDolphinModelConfig(";
  os << "model=\"" << model << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
