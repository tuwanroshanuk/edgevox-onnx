// edgevox-onnx/csrc/kokoro-multi-lang-lexicon.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_KOKORO_MULTI_LANG_LEXICON_H_
#define EDGEVOX_ONNX_CSRC_KOKORO_MULTI_LANG_LEXICON_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-frontend.h"
#include "edgevox-onnx/csrc/offline-tts-kokoro-model-meta-data.h"

namespace edgevox_onnx {

class KokoroMultiLangLexicon : public OfflineTtsFrontend {
 public:
  ~KokoroMultiLangLexicon() override;

  KokoroMultiLangLexicon(const std::string &tokens, const std::string &lexicon,
                         const std::string &data_dir,
                         const OfflineTtsKokoroModelMetaData &meta_data,
                         bool debug);

  template <typename Manager>
  KokoroMultiLangLexicon(Manager *mgr, const std::string &tokens,
                         const std::string &lexicon,
                         const std::string &data_dir,
                         const OfflineTtsKokoroModelMetaData &meta_data,
                         bool debug);

  std::vector<TokenIDs> ConvertTextToTokenIds(
      const std::string &text, const std::string &voice = "") const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_KOKORO_MULTI_LANG_LEXICON_H_
