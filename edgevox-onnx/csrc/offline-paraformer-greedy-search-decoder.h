// edgevox-onnx/csrc/offline-paraformer-greedy-search-decoder.h
//
// Copyright (c)  2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_GREEDY_SEARCH_DECODER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_GREEDY_SEARCH_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/offline-paraformer-decoder.h"

namespace edgevox_onnx {

class OfflineParaformerGreedySearchDecoder : public OfflineParaformerDecoder {
 public:
  explicit OfflineParaformerGreedySearchDecoder(int32_t eos_id)
      : eos_id_(eos_id) {}

  std::vector<OfflineParaformerDecoderResult> Decode(
      Ort::Value log_probs, Ort::Value token_num,
      Ort::Value us_cif_peak = Ort::Value(nullptr)) override;

 private:
  int32_t eos_id_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_PARAFORMER_GREEDY_SEARCH_DECODER_H_
