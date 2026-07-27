// edgevox-onnx/csrc/online-ctc-fst-decoder-config.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OnlineCtcFstDecoderConfig {
  // Path to H.fst, HL.fst or HLG.fst
  std::string graph;
  int32_t max_active = 3000;

  OnlineCtcFstDecoderConfig() = default;

  OnlineCtcFstDecoderConfig(const std::string &graph, int32_t max_active)
      : graph(graph), max_active(max_active) {}

  std::string ToString() const;

  void Register(ParseOptions *po);
  bool Validate() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_CTC_FST_DECODER_CONFIG_H_
