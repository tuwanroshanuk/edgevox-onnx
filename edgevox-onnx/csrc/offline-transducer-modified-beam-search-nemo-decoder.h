// edgevox-onnx/csrc/offline-transducer-modified-beam-search-nemo-decoder.h
//
// Copyright (c)  2026  (authors: github.com/nefastosaturo, github.com/nullbio)

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_NEMO_DECODER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_NEMO_DECODER_H_

#include <vector>

#include "edgevox-onnx/csrc/offline-transducer-decoder.h"
#include "edgevox-onnx/csrc/offline-transducer-nemo-model.h"

namespace edgevox_onnx {

class OfflineTransducerModifiedBeamSearchNeMoDecoder
    : public OfflineTransducerDecoder {
 public:
  OfflineTransducerModifiedBeamSearchNeMoDecoder(
      OfflineTransducerNeMoModel *model, int32_t max_active_paths,
      int32_t unk_id, float blank_penalty, bool is_tdt,
      float hotwords_score = 0.0f)
      : model_(model),
        max_active_paths_(max_active_paths),
        unk_id_(unk_id),
        blank_penalty_(blank_penalty),
        is_tdt_(is_tdt),
        hotwords_score_(hotwords_score) {}

  std::vector<OfflineTransducerDecoderResult> Decode(
      Ort::Value encoder_out,
      Ort::Value encoder_out_length,
      OfflineStream **ss = nullptr,
      int32_t n = 0) override;

 private:
  OfflineTransducerNeMoModel *model_;  // Not owned

  int32_t max_active_paths_;
  int32_t unk_id_;
  float blank_penalty_;
  bool is_tdt_;  // Token-and-Duration Transducer mode
  float hotwords_score_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TRANSDUCER_MODIFIED_BEAM_SEARCH_NEMO_DECODER_H_
