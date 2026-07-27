// edgevox-onnx/csrc/online-stream-state.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_STREAM_STATE_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_STREAM_STATE_H_

#include <cstdint>
#include <vector>

#include "edgevox-onnx/csrc/hypothesis.h"

namespace edgevox_onnx {

struct OnlineStreamStateTensor {
  std::vector<float> float_data;
  std::vector<int32_t> int32_data;
};

struct OnlineTransducerDecoderResultNoOrt {
  int32_t frame_offset = 0;
  std::vector<int32_t> tokens;
  int32_t num_trailing_blanks = 0;
  std::vector<int32_t> timestamps;
  std::vector<float> ys_probs;
  std::vector<float> lm_probs;
  std::vector<float> context_scores;
  std::vector<float> decoder_out;
  Hypotheses hyps;

  // Decoder states for models with LSTM decoders (e.g., nemo_transducer).
  // Each element is a flat state tensor (h, c, etc.). Ignored by zipformer.
  std::vector<std::vector<float>> states;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_STREAM_STATE_H_
