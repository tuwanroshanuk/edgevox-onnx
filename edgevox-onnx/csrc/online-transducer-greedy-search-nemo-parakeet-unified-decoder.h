// edgevox-onnx/csrc/online-transducer-greedy-search-nemo-parakeet-unified-decoder.h
//
// Copyright (c)  2026  Milan Leonard

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_PARAKEET_UNIFIED_DECODER_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_PARAKEET_UNIFIED_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/online-transducer-decoder.h"
#include "edgevox-onnx/csrc/online-transducer-nemo-parakeet-unified-model.h"

namespace edgevox_onnx {

class OnlineStream;

class OnlineTransducerGreedySearchNeMoParakeetUnifiedDecoder {
 public:
  OnlineTransducerGreedySearchNeMoParakeetUnifiedDecoder(
      OnlineTransducerNeMoParakeetUnifiedModel *model, float blank_penalty)
      : model_(model), blank_penalty_(blank_penalty) {}

  void Decode(Ort::Value encoder_out, OnlineStream **ss, int32_t n) const;

 private:
  OnlineTransducerNeMoParakeetUnifiedModel *model_;  // Not owned
  float blank_penalty_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_PARAKEET_UNIFIED_DECODER_H_
