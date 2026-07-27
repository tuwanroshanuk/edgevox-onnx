// edgevox-onnx/csrc/edgevox-onnx-offline-punctuation.cc
//
// Copyright (c)  2022-2024  Xiaomi Corporation
#include <stdio.h>

#include <chrono>
#include <string>

#include "edgevox-onnx/csrc/offline-punctuation.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/parse-options.h"

int main(int32_t argc, char *argv[]) {
  const char *kUsageMessage = R"usage(
Add punctuations to the input text.

The input text can contain both Chinese and English words.

Usage:

wget https://github.com/k2-fsa/sherpa-onnx/releases/download/punctuation-models/sherpa-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
tar xvf sherpa-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
rm sherpa-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2

./bin/edgevox-onnx-offline-punctuation \
  --ct-transformer=./sherpa-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12/model.onnx
  "你好吗how are you Fantasitic 谢谢我很好你怎么样呢"

The output text should look like below:
)usage";

  edgevox_onnx::ParseOptions po(kUsageMessage);
  edgevox_onnx::OfflinePunctuationConfig config;
  config.Register(&po);
  po.Read(argc, argv);
  if (po.NumArgs() != 1) {
    fprintf(stderr,
            "Error: Please provide only 1 position argument containing the "
            "input text.\n\n");
    po.PrintUsage();
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", config.ToString().c_str());

  if (!config.Validate()) {
    fprintf(stderr, "Errors in config!\n");
    return -1;
  }

  fprintf(stderr, "Creating OfflinePunctuation ...\n");
  edgevox_onnx::OfflinePunctuation punct(config);
  fprintf(stderr, "Started\n");
  const auto begin = std::chrono::steady_clock::now();

  std::string text = po.GetArg(1);
  std::string text_with_punct = punct.AddPunctuation(text);
  fprintf(stderr, "Done\n");
  const auto end = std::chrono::steady_clock::now();

  float elapsed_seconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count() /
      1000.;

  fprintf(stderr, "Num threads: %d\n", config.model.num_threads);
  fprintf(stderr, "Elapsed seconds: %.3f s\n", elapsed_seconds);
  fprintf(stderr, "Input text: %s\n", text.c_str());
  fprintf(stderr, "Output text: ");
  fprintf(stdout, "%s\n", text_with_punct.c_str());
}
