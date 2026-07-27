// edgevox-onnx/csrc/offline-source-separation-uvr-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-source-separation-uvr-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineSourceSeparationUvrModelConfig::Register(ParseOptions *po) {
  po->Register("uvr-model", &model, "Path to the UVR model");
}

bool OfflineSourceSeparationUvrModelConfig::Validate() const {
  if (model.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --uvr-model");
    return false;
  }

  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("UVR model '%s' does not exist. ", model.c_str());
    return false;
  }

  return true;
}

std::string OfflineSourceSeparationUvrModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineSourceSeparationUvrModelConfig(";
  os << "model=\"" << model << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
