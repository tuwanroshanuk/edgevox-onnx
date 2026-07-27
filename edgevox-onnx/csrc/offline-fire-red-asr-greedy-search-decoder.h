// edgevox-onnx/csrc/offline-fire-red-asr-greedy-search-decoder.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_GREEDY_SEARCH_DECODER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_GREEDY_SEARCH_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/offline-fire-red-asr-decoder.h"
#include "edgevox-onnx/csrc/offline-fire-red-asr-model.h"

namespace edgevox_onnx {

class OfflineFireRedAsrGreedySearchDecoder : public OfflineFireRedAsrDecoder {
 public:
  explicit OfflineFireRedAsrGreedySearchDecoder(OfflineFireRedAsrModel *model)
      : model_(model) {}

  std::vector<OfflineFireRedAsrDecoderResult> Decode(
      Ort::Value cross_k, Ort::Value cross_v,
      int32_t num_feature_frames) override;

 private:
  OfflineFireRedAsrModel *model_;  // not owned
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_GREEDY_SEARCH_DECODER_H_
