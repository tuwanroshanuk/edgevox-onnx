// edgevox-onnx/csrc/online-ctc-fst-decoder.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_H_

#include <memory>
#include <vector>

#include "fst/fst.h"
#include "edgevox-onnx/csrc/online-ctc-decoder.h"
#include "edgevox-onnx/csrc/online-ctc-fst-decoder-config.h"

namespace edgevox_onnx {

class OnlineCtcFstDecoder : public OnlineCtcDecoder {
 public:
  OnlineCtcFstDecoder(const OnlineCtcFstDecoderConfig &config,
                      int32_t blank_id);

  void Decode(const float *log_probs, int32_t batch_size, int32_t num_frames,
              int32_t vocab_size, std::vector<OnlineCtcDecoderResult> *results,
              OnlineStream **ss = nullptr, int32_t n = 0) override;

  std::unique_ptr<kaldi_decoder::FasterDecoder> CreateFasterDecoder()
      const override;

 private:
  OnlineCtcFstDecoderConfig config_;
  kaldi_decoder::FasterDecoderOptions options_;

  std::unique_ptr<fst::Fst<fst::StdArc>> fst_;
  int32_t blank_id_ = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_H_
