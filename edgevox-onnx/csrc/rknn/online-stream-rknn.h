// edgevox-onnx/csrc/rknn/online-stream-rknn.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_RKNN_ONLINE_STREAM_RKNN_H_
#define EDGEVOX_ONNX_CSRC_RKNN_ONLINE_STREAM_RKNN_H_
#include <memory>
#include <vector>

#include "rknn_api.h"  // NOLINT
#include "edgevox-onnx/csrc/online-stream.h"
#include "edgevox-onnx/csrc/rknn/online-transducer-decoder-rknn.h"

namespace edgevox_onnx {

class OnlineStreamRknn : public OnlineStream {
 public:
  explicit OnlineStreamRknn(const FeatureExtractorConfig &config = {},
                            ContextGraphPtr context_graph = nullptr);

  ~OnlineStreamRknn();

  void SetZipformerEncoderStates(
      std::vector<std::vector<uint8_t>> states) const;

  std::vector<std::vector<uint8_t>> &GetZipformerEncoderStates() const;

  void SetZipformerResult(OnlineTransducerDecoderResultRknn r) const;

  OnlineTransducerDecoderResultRknn &GetZipformerResult() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_RKNN_ONLINE_STREAM_RKNN_H_
