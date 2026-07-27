// edgevox-onnx/csrc/online-punctuation.h
//
// Copyright (c) 2024 Jian You (jianyou@cisco.com, Cisco Systems)

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/online-punctuation-model-config.h"
#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OnlinePunctuationConfig {
  OnlinePunctuationModelConfig model;

  OnlinePunctuationConfig() = default;

  explicit OnlinePunctuationConfig(const OnlinePunctuationModelConfig &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

class OnlinePunctuationImpl;

class OnlinePunctuation {
 public:
  explicit OnlinePunctuation(const OnlinePunctuationConfig &config);

  template <typename Manager>
  OnlinePunctuation(Manager *mgr, const OnlinePunctuationConfig &config);

  ~OnlinePunctuation();

  // Add punctuation and casing to the input text and return it.
  std::string AddPunctuationWithCase(const std::string &text) const;

 private:
  std::unique_ptr<OnlinePunctuationImpl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_PUNCTUATION_H_
