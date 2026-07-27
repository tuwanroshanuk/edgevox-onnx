// edgevox-onnx/csrc/sentence-piece-tokenizer.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_SENTENCE_PIECE_TOKENIZER_H_
#define EDGEVOX_ONNX_CSRC_SENTENCE_PIECE_TOKENIZER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace edgevox_onnx {

class SentencePieceTokenizer {
 public:
  SentencePieceTokenizer(const std::string &vocab_json,
                         const std::string &token_scores_json);

  template <typename Manager>
  SentencePieceTokenizer(Manager *mgr, const std::string &vocab_json,
                         const std::string &token_scores_json);

  ~SentencePieceTokenizer();

  std::vector<int32_t> EncodeIds(const std::string &text) const;
  std::vector<std::string> EncodeTokens(const std::string &text) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_SENTENCE_PIECE_TOKENIZER_H_
