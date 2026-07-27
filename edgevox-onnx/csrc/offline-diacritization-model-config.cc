// edgevox-onnx/csrc/offline-diacritization-model-config.cc
//
// Copyright (c)  2026  Matias Lin

#include "edgevox-onnx/csrc/offline-diacritization-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineDiacritizationModelConfig::Register(ParseOptions *po) {
  po->Register("catt-encoder", &catt_encoder,
               "Path to the CATT (Context-Aware Transformer for "
               "Tashkeel) ONNX encoder model for Arabic diacritization.");

  po->Register("catt-decoder", &catt_decoder,
               "Path to the CATT (Context-Aware Transformer for "
               "Tashkeel) ONNX decoder model for Arabic diacritization.");

  po->Register("num-threads", &num_threads,
               "Number of threads to run the neural network");

  po->Register("debug", &debug,
               "true to print model information while loading it.");

  po->Register("provider", &provider,
               "Specify a provider to use: cpu, cuda, coreml");
}

bool OfflineDiacritizationModelConfig::Validate() const {
  if (catt_encoder.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --catt-encoder");
    return false;
  }

  if (catt_decoder.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --catt-decoder");
    return false;
  }

  if (!FileExists(catt_encoder)) {
    EDGEVOX_ONNX_LOGE("--catt-encoder %s does not exist", catt_encoder.c_str());
    return false;
  }

  if (!FileExists(catt_decoder)) {
    EDGEVOX_ONNX_LOGE("--catt-decoder %s does not exist", catt_decoder.c_str());
    return false;
  }

  return true;
}

std::string OfflineDiacritizationModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineDiacritizationModelConfig(";
  os << "catt_encoder=\"" << catt_encoder << "\", ";
  os << "catt_decoder=\"" << catt_decoder << "\", ";
  os << "num_threads=" << num_threads << ", ";
  os << "debug=" << (debug ? "True" : "False") << ", ";
  os << "provider=\"" << provider << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
