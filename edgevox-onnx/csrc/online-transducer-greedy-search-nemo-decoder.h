// edgevox-onnx/csrc/online-transducer-greedy-search-nemo-decoder.h
//
// Copyright (c)  2024  Xiaomi Corporation
// Copyright (c)  2024  Sangeet Sagar

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/online-transducer-decoder.h"
#include "edgevox-onnx/csrc/online-transducer-nemo-model.h"

namespace edgevox_onnx {

class OnlineStream;

class OnlineTransducerGreedySearchNeMoDecoder {
 public:
  OnlineTransducerGreedySearchNeMoDecoder(OnlineTransducerNeMoModel *model,
                                          float blank_penalty)
      : model_(model), blank_penalty_(blank_penalty) {}

  // @param n number of elements in ss
  void Decode(Ort::Value encoder_out, OnlineStream **ss, int32_t n) const;

 private:
  OnlineTransducerNeMoModel *model_;  // Not owned
  float blank_penalty_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_NEMO_DECODER_H_
