// edgevox-onnx/csrc/online-transducer-greedy-search-decoder.h
//
// Copyright (c)  2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/online-transducer-decoder.h"
#include "edgevox-onnx/csrc/online-transducer-model.h"

namespace edgevox_onnx {

class OnlineTransducerGreedySearchDecoder : public OnlineTransducerDecoder {
 public:
  OnlineTransducerGreedySearchDecoder(OnlineTransducerModel *model,
                                      int32_t unk_id,
                                      float blank_penalty,
                                      float temperature_scale)
      : model_(model),
      unk_id_(unk_id),
      blank_penalty_(blank_penalty),
      temperature_scale_(temperature_scale) {}

  OnlineTransducerDecoderResult GetEmptyResult() const override;

  void StripLeadingBlanks(OnlineTransducerDecoderResult *r) const override;

  void Decode(Ort::Value encoder_out,
              std::vector<OnlineTransducerDecoderResult> *result) override;

 private:
  OnlineTransducerModel *model_;  // Not owned
  int32_t unk_id_;
  float blank_penalty_;
  float temperature_scale_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_GREEDY_SEARCH_DECODER_H_
