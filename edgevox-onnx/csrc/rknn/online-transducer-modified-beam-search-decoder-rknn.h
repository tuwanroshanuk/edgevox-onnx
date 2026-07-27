// edgevox-onnx/csrc/rknn/online-transducer-modified-beam-search-decoder-rknn.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_RKNN_ONLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_DECODER_RKNN_H_
#define EDGEVOX_ONNX_CSRC_RKNN_ONLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_DECODER_RKNN_H_

#include <vector>

#include "edgevox-onnx/csrc/rknn/online-transducer-decoder-rknn.h"
#include "edgevox-onnx/csrc/rknn/online-zipformer-transducer-model-rknn.h"

namespace edgevox_onnx {

class OnlineTransducerModifiedBeamSearchDecoderRknn
    : public OnlineTransducerDecoderRknn {
 public:
  explicit OnlineTransducerModifiedBeamSearchDecoderRknn(
      OnlineZipformerTransducerModelRknn *model, int32_t max_active_paths,
      int32_t unk_id = 2, float blank_penalty = 0.0)
      : model_(model),
        max_active_paths_(max_active_paths),
        unk_id_(unk_id),
        blank_penalty_(blank_penalty) {}

  OnlineTransducerDecoderResultRknn GetEmptyResult() const override;

  void StripLeadingBlanks(OnlineTransducerDecoderResultRknn *r) const override;

  void Decode(std::vector<float> encoder_out,
              OnlineTransducerDecoderResultRknn *result) const override;

 private:
  OnlineZipformerTransducerModelRknn *model_;  // Not owned
  int32_t max_active_paths_;
  int32_t unk_id_;
  float blank_penalty_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_RKNN_ONLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_DECODER_RKNN_H_
