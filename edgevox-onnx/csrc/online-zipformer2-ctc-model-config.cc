// edgevox-onnx/csrc/online-zipformer2-ctc-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/online-zipformer2-ctc-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OnlineZipformer2CtcModelConfig::Register(ParseOptions *po) {
  po->Register("zipformer2-ctc-model", &model,
               "Path to CTC model.onnx. See also "
               "https://github.com/k2-fsa/icefall/pull/1413");
}

bool OnlineZipformer2CtcModelConfig::Validate() const {
  if (model.empty()) {
    EDGEVOX_ONNX_LOGE("--zipformer2-ctc-model is empty!");
    return false;
  }

  if (!FileExists(model)) {
    EDGEVOX_ONNX_LOGE("--zipformer2-ctc-model '%s' does not exist",
                     model.c_str());
    return false;
  }

  return true;
}

std::string OnlineZipformer2CtcModelConfig::ToString() const {
  std::ostringstream os;

  os << "OnlineZipformer2CtcModelConfig(";
  os << "model=\"" << model << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
