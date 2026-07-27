// edgevox-onnx/csrc/offline-sense-voice-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-sense-voice-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {

void OfflineSenseVoiceModelConfig::Register(ParseOptions *po) {
  po->Register("sense-voice-model", &model,
               "Path to model.onnx of SenseVoice.");
  po->Register(
      "sense-voice-language", &language,
      "Valid values: auto, zh, en, ja, ko, yue. If left empty, auto is used");
  po->Register(
      "sense-voice-use-itn", &use_itn,
      "True to enable inverse text normalization. False to disable it.");

  std::string prefix = "sense-voice";
  ParseOptions p(prefix, po);

  qnn_config.Register(&p);
}

bool OfflineSenseVoiceModelConfig::Validate() const {
  if (qnn_config.context_binary.empty()) {
    if (model.empty()) {
      EDGEVOX_ONNX_LOGE("Please provide a senseVoice model");
      return false;
    }

    if (!FileExists(model)) {
      EDGEVOX_ONNX_LOGE("SenseVoice model '%s' does not exist", model.c_str());
      return false;
    }
  }

  if (!language.empty()) {
    if (language != "auto" && language != "zh" && language != "en" &&
        language != "ja" && language != "ko" && language != "yue") {
      EDGEVOX_ONNX_LOGE(
          "Invalid sense-voice-language: '%s'. Valid values are: auto, zh, en, "
          "ja, ko, yue. Or you can leave it empty to use 'auto'",
          language.c_str());

      return false;
    }
  }

  if (model.empty() && !qnn_config.context_binary.empty()) {
    // we require that the context_binary exists
    if (!FileExists(qnn_config.context_binary)) {
      EDGEVOX_ONNX_LOGE(
          "Model is empty, but you provide a context binary that does not "
          "exist");
      return false;
    }
  }

  if (EndsWith(model, ".so") || EndsWith(model, ".bin") ||
      (model.empty() && !qnn_config.context_binary.empty())) {
    return qnn_config.Validate();
  }

  return true;
}

std::string OfflineSenseVoiceModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineSenseVoiceModelConfig(";
  os << "model=\"" << model << "\", ";

  if (!qnn_config.backend_lib.empty()) {
    os << "qnn_config=" << qnn_config.ToString() << ", ";
  }

  os << "language=\"" << language << "\", ";
  os << "use_itn=" << (use_itn ? "True" : "False") << ")";

  return os.str();
}

}  // namespace edgevox_onnx
