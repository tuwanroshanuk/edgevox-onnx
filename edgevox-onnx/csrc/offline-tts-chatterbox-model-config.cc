// edgevox-onnx/csrc/offline-tts-chatterbox-model-config.cc
//
// Copyright (c) 2026 Nexus Cloud

#include "edgevox-onnx/csrc/offline-tts-chatterbox-model-config.h"

#include <sstream>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineTtsChatterboxModelConfig::Register(ParseOptions *po) {
  po->Register("chatterbox-speech-encoder", &speech_encoder,
               "Path to Chatterbox Turbo speech_encoder ONNX model");
  po->Register("chatterbox-embed-tokens", &embed_tokens,
               "Path to Chatterbox Turbo embed_tokens ONNX model");
  po->Register("chatterbox-language-model", &language_model,
               "Path to Chatterbox Turbo language_model ONNX model");
  po->Register("chatterbox-conditional-decoder", &conditional_decoder,
               "Path to Chatterbox Turbo conditional_decoder ONNX model");
  po->Register("chatterbox-tokenizer", &tokenizer,
               "Directory containing Chatterbox Turbo tokenizer.json");
}

bool OfflineTtsChatterboxModelConfig::Validate() const {
  const std::pair<const char *, const std::string *> files[] = {
      {"--chatterbox-speech-encoder", &speech_encoder},
      {"--chatterbox-embed-tokens", &embed_tokens},
      {"--chatterbox-language-model", &language_model},
      {"--chatterbox-conditional-decoder", &conditional_decoder},
  };
  for (const auto &item : files) {
    if (item.second->empty() || !FileExists(*item.second)) {
      EDGEVOX_ONNX_LOGE("%s '%s' does not exist", item.first,
                       item.second->c_str());
      return false;
    }
  }
  if (tokenizer.empty() || !FileExists(tokenizer + "/tokenizer.json")) {
    EDGEVOX_ONNX_LOGE(
        "--chatterbox-tokenizer '%s' must contain tokenizer.json",
        tokenizer.c_str());
    return false;
  }
  return true;
}

std::string OfflineTtsChatterboxModelConfig::ToString() const {
  std::ostringstream os;
  os << "OfflineTtsChatterboxModelConfig("
     << "speech_encoder=\"" << speech_encoder << "\", "
     << "embed_tokens=\"" << embed_tokens << "\", "
     << "language_model=\"" << language_model << "\", "
     << "conditional_decoder=\"" << conditional_decoder << "\", "
     << "tokenizer=\"" << tokenizer << "\")";
  return os.str();
}

}  // namespace edgevox_onnx
