// edgevox-onnx/csrc/offline-transducer-greedy-search-decoder.h
//
// Copyright (c)  2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/offline-transducer-decoder.h"
#include "edgevox-onnx/csrc/offline-transducer-model.h"

namespace edgevox_onnx {

class OfflineTransducerGreedySearchDecoder : public OfflineTransducerDecoder {
 public:
  OfflineTransducerGreedySearchDecoder(OfflineTransducerModel *model,
                                       int32_t unk_id,
                                       float blank_penalty)
      : model_(model), unk_id_(unk_id), blank_penalty_(blank_penalty) {}

  std::vector<OfflineTransducerDecoderResult> Decode(
      Ort::Value encoder_out, Ort::Value encoder_out_length,
      OfflineStream **ss = nullptr, int32_t n = 0) override;

 private:
  OfflineTransducerModel *model_;  // Not owned
  int32_t unk_id_;
  float blank_penalty_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_
