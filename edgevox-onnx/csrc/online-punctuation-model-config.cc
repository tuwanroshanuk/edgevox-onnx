// edgevox-onnx/csrc/online-punctuation-model-config.cc
//
// Copyright (c) 2024 Jian You (jianyou@cisco.com, Cisco Systems)

#include "edgevox-onnx/csrc/online-punctuation-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OnlinePunctuationModelConfig::Register(ParseOptions *po) {
  po->Register("cnn-bilstm", &cnn_bilstm,
               "Path to the light-weight CNN-BiLSTM model");

  po->Register("bpe-vocab", &bpe_vocab, "Path to the bpe vocab file");

  po->Register("num-threads", &num_threads,
               "Number of threads to run the neural network");

  po->Register("debug", &debug,
               "true to print model information while loading it.");

  po->Register("provider", &provider,
               "Specify a provider to use: cpu, cuda, coreml");
}

bool OnlinePunctuationModelConfig::Validate() const {
  if (cnn_bilstm.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --cnn-bilstm");
    return false;
  }

  if (!FileExists(cnn_bilstm)) {
    EDGEVOX_ONNX_LOGE("--cnn-bilstm '%s' does not exist", cnn_bilstm.c_str());
    return false;
  }

  if (bpe_vocab.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --bpe-vocab");
    return false;
  }

  if (!FileExists(bpe_vocab)) {
    EDGEVOX_ONNX_LOGE("--bpe-vocab '%s' does not exist", bpe_vocab.c_str());
    return false;
  }

  return true;
}

std::string OnlinePunctuationModelConfig::ToString() const {
  std::ostringstream os;

  os << "OnlinePunctuationModelConfig(";
  os << "cnn_bilstm=\"" << cnn_bilstm << "\", ";
  os << "bpe_vocab=\"" << bpe_vocab << "\", ";
  os << "num_threads=" << num_threads << ", ";
  os << "debug=" << (debug ? "True" : "False") << ", ";
  os << "provider=\"" << provider << "\")";

  return os.str();
}

}  // namespace edgevox_onnx
