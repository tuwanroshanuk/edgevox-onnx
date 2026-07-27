// edgevox-onnx/csrc/offline-transducer-greedy-search-nemo-decoder.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/offline-transducer-decoder.h"
#include "edgevox-onnx/csrc/offline-transducer-nemo-model.h"

namespace edgevox_onnx {

class OfflineTransducerGreedySearchNeMoDecoder
    : public OfflineTransducerDecoder {
 public:
  OfflineTransducerGreedySearchNeMoDecoder(OfflineTransducerNeMoModel *model,
                                           float blank_penalty, bool is_tdt)
      : model_(model), blank_penalty_(blank_penalty), is_tdt_(is_tdt) {}

  std::vector<OfflineTransducerDecoderResult> Decode(
      Ort::Value encoder_out, Ort::Value encoder_out_length,
      OfflineStream **ss = nullptr, int32_t n = 0) override;

 private:
  OfflineTransducerNeMoModel *model_;  // Not owned
  float blank_penalty_;
  bool is_tdt_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_
