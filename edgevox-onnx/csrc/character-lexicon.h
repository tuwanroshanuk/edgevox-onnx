// edgevox-onnx/csrc/character-lexicon.h
//
// Copyright (c)  2022-2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_CHARACTER_LEXICON_H_
#define EDGEVOX_ONNX_CSRC_CHARACTER_LEXICON_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-frontend.h"

namespace edgevox_onnx {

class CharacterLexicon : public OfflineTtsFrontend {
 public:
  ~CharacterLexicon() override;

  CharacterLexicon(const std::string &lexicon, const std::string &tokens,
                   bool debug, bool use_g2pw = false);

  template <typename Manager>
  CharacterLexicon(Manager *mgr, const std::string &lexicon,
                   const std::string &tokens, bool debug,
                   bool use_g2pw = false);

  std::vector<TokenIDs> ConvertTextToTokenIds(
      const std::string &text,
      const std::string &unused_voice = "") const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_CHARACTER_LEXICON_H_
