// edgevox-onnx/csrc/edgevox-onnx-offline-diacritization.cc
//
// Copyright (c)  2026  Matias Lin

#include <stdio.h>

#include <chrono>
#include <string>

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/offline-diacritization.h"
#include "edgevox-onnx/csrc/parse-options.h"

int main(int32_t argc, char *argv[]) {
  const char *kUsageMessage = R"Usage(
Add diacritics (tashkeel) to the input Arabic text.

Usage:
  ./bin/edgevox-onnx-offline-diacritization \
    --catt-encoder=/path/to/encoder.onnx \
    --catt-decoder=/path/to/decoder.onnx \
    [Arabic input]

The output text will be the input with diacritics added.
)Usage";

  edgevox_onnx::ParseOptions po(kUsageMessage);
  edgevox_onnx::OfflineDiacritizationConfig config;
  config.Register(&po);
  po.Read(argc, argv);
  if (po.NumArgs() != 1) {
    fprintf(stderr,
            "Error: Please provide only 1 positional argument containing the "
            "Arabic input text. Found %d.\n\n",
            po.NumArgs());
    po.PrintUsage();
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", config.ToString().c_str());

  if (!config.Validate()) {
    fprintf(stderr, "Errors in config\n");
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

  fprintf(stderr, "Creating OfflineDiacritization...\n");
  edgevox_onnx::OfflineDiacritization diacrt(config);
  fprintf(stderr, "Started\n");
  const auto begin = std::chrono::steady_clock::now();

  std::string text = po.GetArg(1);
  std::string text_with_diacritics = diacrt.AddDiacritics(text);
  fprintf(stderr, "Done\n");

  const auto end = std::chrono::steady_clock::now();
  long long elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count();

  fprintf(stderr, "Num threads: %d\n", config.model.num_threads);
  fprintf(stderr, "Elapsed milliseconds: %lld\n", elapsed_ms);
  fprintf(stderr, "Input text: %s\n", text.c_str());
  fprintf(stderr, "Output text: ");
  fprintf(stdout, "%s\n", text_with_diacritics.c_str());
}
