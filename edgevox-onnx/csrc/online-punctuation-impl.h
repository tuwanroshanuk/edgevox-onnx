// edgevox-onnx/csrc/online-punctuation-impl.h
//
// Copyright (c) 2024 Jian You (jianyou@cisco.com, Cisco Systems)

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_IMPL_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/online-punctuation.h"

namespace edgevox_onnx {

class OnlinePunctuationImpl {
 public:
  virtual ~OnlinePunctuationImpl() = default;

  static std::unique_ptr<OnlinePunctuationImpl> Create(
      const OnlinePunctuationConfig &config);

  template <typename Manager>
  static std::unique_ptr<OnlinePunctuationImpl> Create(
      Manager *mgr, const OnlinePunctuationConfig &config);

  virtual std::string AddPunctuationWithCase(const std::string &text) const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_IMPL_H_
