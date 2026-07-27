// edgevox-onnx/csrc/offline-source-separation-spleeter-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-source-separation-spleeter-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineSourceSeparationSpleeterModelConfig::Register(ParseOptions *po) {
  po->Register("spleeter-vocals", &vocals, "Path to the spleeter vocals model");

  po->Register("spleeter-accompaniment", &accompaniment,
               "Path to the spleeter accompaniment model");
}

bool OfflineSourceSeparationSpleeterModelConfig::Validate() const {
  if (vocals.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --spleeter-vocals");
    return false;
  }

  if (!FileExists(vocals)) {
    EDGEVOX_ONNX_LOGE("spleeter vocals '%s' does not exist. ", vocals.c_str());
    return false;
  }

  if (accompaniment.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --spleeter-accompaniment");
    return false;
  }

  if (!FileExists(accompaniment)) {
    EDGEVOX_ONNX_LOGE("spleeter accompaniment '%s' does not exist. ",
                     accompaniment.c_str());
    return false;
  }

  return true;
}

std::string OfflineSourceSeparationSpleeterModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineSourceSeparationSpleeterModelConfig(";
  os << "vocals=\"" << vocals << "\", ";
  os << "accompaniment=\"" << accompaniment << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
