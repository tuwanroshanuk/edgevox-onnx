// edgevox-onnx/csrc/offline-punctuation-impl.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-punctuation.h"

namespace edgevox_onnx {

class OfflinePunctuationImpl {
 public:
  virtual ~OfflinePunctuationImpl() = default;

  static std::unique_ptr<OfflinePunctuationImpl> Create(
      const OfflinePunctuationConfig &config);

  template <typename Manager>
  static std::unique_ptr<OfflinePunctuationImpl> Create(
      Manager *mgr, const OfflinePunctuationConfig &config);

  virtual std::string AddPunctuation(const std::string &text) const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_IMPL_H_
