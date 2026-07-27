// edgevox-onnx/csrc/melo-tts-lexicon.h
//
// Copyright (c)  2022-2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_MELO_TTS_LEXICON_H_
#define EDGEVOX_ONNX_CSRC_MELO_TTS_LEXICON_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-frontend.h"
#include "edgevox-onnx/csrc/offline-tts-vits-model-meta-data.h"

namespace edgevox_onnx {

class MeloTtsLexicon : public OfflineTtsFrontend {
 public:
  ~MeloTtsLexicon() override;
  MeloTtsLexicon(const std::string &lexicon, const std::string &tokens,
                 const OfflineTtsVitsModelMetaData &meta_data, bool debug);

  template <typename Manager>
  MeloTtsLexicon(Manager *mgr, const std::string &lexicon,
                 const std::string &tokens,
                 const OfflineTtsVitsModelMetaData &meta_data, bool debug);

  std::vector<TokenIDs> ConvertTextToTokenIds(
      const std::string &text,
      const std::string &unused_voice = "") const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_MELO_TTS_LEXICON_H_
