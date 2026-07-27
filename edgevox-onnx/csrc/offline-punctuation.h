// edgevox-onnx/csrc/offline-punctuation.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-punctuation-model-config.h"
#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflinePunctuationConfig {
  OfflinePunctuationModelConfig model;

  OfflinePunctuationConfig() = default;

  explicit OfflinePunctuationConfig(const OfflinePunctuationModelConfig &model)
      : model(model) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

class OfflinePunctuationImpl;

class OfflinePunctuation {
 public:
  explicit OfflinePunctuation(const OfflinePunctuationConfig &config);

  template <typename Manager>
  OfflinePunctuation(Manager *mgr, const OfflinePunctuationConfig &config);

  ~OfflinePunctuation();

  // Add punctuation to the input text and return it.
  std::string AddPunctuation(const std::string &text) const;

 private:
  std::unique_ptr<OfflinePunctuationImpl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_PUNCTUATION_H_
