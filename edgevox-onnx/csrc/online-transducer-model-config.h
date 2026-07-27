// edgevox-onnx/csrc/online-transducer-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

#include "edgevox-onnx/csrc/qnn-config.h"

namespace edgevox_onnx {

struct OnlineTransducerModelConfig {
  std::string encoder;
  std::string decoder;
  std::string joiner;
  QnnConfig qnn_config;

  OnlineTransducerModelConfig() = default;
  OnlineTransducerModelConfig(const std::string &encoder,
                              const std::string &decoder,
                              const std::string &joiner)
      : encoder(encoder), decoder(decoder), joiner(joiner) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_TRANSDUCER_MODEL_CONFIG_H_
