// edgevox-onnx/csrc/offline-diacritization-model-config.h
//
// Copyright (c)  2026  Matias Lin
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_MODEL_CONFIG_H_

#include <string>

#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct OfflineDiacritizationModelConfig {
  std::string catt_encoder;
  std::string catt_decoder;

  int32_t num_threads = 1;
  bool debug = false;
  std::string provider = "cpu";

  OfflineDiacritizationModelConfig() = default;
  OfflineDiacritizationModelConfig(const std::string &catt_encoder,
                                   const std::string &catt_decoder,
                                   int32_t num_threads, bool debug,
                                   const std::string &provider)
      : catt_encoder(catt_encoder),
        catt_decoder(catt_decoder),
        num_threads(num_threads),
        debug(debug),
        provider(provider) {}

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_MODEL_CONFIG_H_
