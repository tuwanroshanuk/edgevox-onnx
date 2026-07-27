// edgevox-onnx/csrc/offline-cohere-transcribe-model-config.h
//
// Copyright (c)  2026  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_COHERE_TRANSCRIBE_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_COHERE_TRANSCRIBE_MODEL_CONFIG_H_

#include <string>
#include <vector>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineCohereTranscribeModelConfig {
  std::string encoder;
  std::string decoder;

  // This model supports 14 languages:
  // ar, de, el, en, es, fr, it
  // ja, ko, nl, pl, pt, vi, zh
  std::string language;

  // true to enable punctuations. false to disable punctuations
  bool use_punct = true;

  // true to enable inverse text normalization
  // false to disable inverse text normalization
  bool use_itn = true;

  OfflineCohereTranscribeModelConfig() = default;
  OfflineCohereTranscribeModelConfig(const std::string &encoder,
                                     const std::string &decoder,
                                     const std::string &language,
                                     bool use_punct, bool use_itn)
      : encoder(encoder),
        decoder(decoder),
        language(language),
        use_punct(use_punct),
        use_itn(use_itn) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

bool IsValidCohereTranscribeLanguage(const std::string &language);

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_COHERE_TRANSCRIBE_MODEL_CONFIG_H_
