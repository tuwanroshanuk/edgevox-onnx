// edgevox-onnx/csrc/matcha-tts-lexicon.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_MATCHA_TTS_LEXICON_H_
#define EDGEVOX_ONNX_CSRC_MATCHA_TTS_LEXICON_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-frontend.h"

namespace edgevox_onnx {

// For Chinese+English matcha tts
class MatchaTtsLexicon : public OfflineTtsFrontend {
 public:
  ~MatchaTtsLexicon() override;

  MatchaTtsLexicon(const std::string &lexicon, const std::string &tokens,
                   const std::string &data_dir, bool debug,
                   bool skip_replacement,
                   const std::string &espeak_voice = "en-us");

  template <typename Manager>
  MatchaTtsLexicon(Manager *mgr, const std::string &lexicon,
                   const std::string &tokens, const std::string &data_dir,
                   bool debug, bool skip_replacement,
                   const std::string &espeak_voice = "en-us");

  std::vector<TokenIDs> ConvertTextToTokenIds(
      const std::string &text,
      const std::string &unused_voice = "") const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_MATCHA_TTS_LEXICON_H_
