// cxx-api-examples/online-punctuation-cxx-api.cc
// Copyright (c)  2025  Xiaomi Corporation

// To use punctuation model:
// clang-format off
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
// tar xvf edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
// rm edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
// clang-format on

#include <iostream>
#include <string>

#include "edgevox-onnx/c-api/cxx-api.h"

int32_t main() {
  using namespace edgevox_onnx::cxx;  // NOLINT

  OnlinePunctuationConfig punctuation_config;
  punctuation_config.model.cnn_bilstm =
      "edgevox-onnx-online-punct-en-2024-08-06/model.int8.onnx";
  punctuation_config.model.bpe_vocab =
      "edgevox-onnx-online-punct-en-2024-08-06/bpe.vocab";
  punctuation_config.model.num_threads = 1;
  punctuation_config.model.debug = false;
  punctuation_config.model.provider = "cpu";

  OnlinePunctuation punct = OnlinePunctuation::Create(punctuation_config);
  if (!punct.Get()) {
    std::cerr
        << "Failed to create punctuation model. Please check your config\n";
    return -1;
  }

  std::string text = "how are you i am fine thank you";
  std::string text_with_punct = punct.AddPunctuation(text);
  std::cout << "Original text: " << text << std::endl;
  std::cout << "With punctuation: " << text_with_punct << std::endl;

  return 0;
}
