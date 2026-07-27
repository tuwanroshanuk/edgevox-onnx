// c-api-examples/moonshine-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

//
// This file demonstrates how to use Moonshine tiny with edgevox-onnx's C API.
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
// tar xvf edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
// rm edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "./edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav";
  const char *preprocessor =
      "./edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx";
  const char *encoder = "./edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx";
  const char *uncached_decoder =
      "./edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx";
  const char *cached_decoder =
      "./edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx";
  const char *tokens = "./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  // Offline model config
  EdgevoxOnnxOfflineModelConfig offline_model_config;
  memset(&offline_model_config, 0, sizeof(offline_model_config));
  offline_model_config.debug = 1;
  offline_model_config.num_threads = 1;
  offline_model_config.provider = "cpu";
  offline_model_config.tokens = tokens;
  offline_model_config.moonshine.preprocessor = preprocessor;
  offline_model_config.moonshine.encoder = encoder;
  offline_model_config.moonshine.uncached_decoder = uncached_decoder;
  offline_model_config.moonshine.cached_decoder = cached_decoder;

  // Recognizer config
  EdgevoxOnnxOfflineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config = offline_model_config;

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      EdgevoxOnnxCreateOfflineRecognizer(&recognizer_config);

  if (recognizer == NULL) {
    fprintf(stderr, "Please check your config!\n");
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  const EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxCreateOfflineStream(recognizer);

  EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
                                  wave->num_samples);
  EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
  const EdgevoxOnnxOfflineRecognizerResult *result =
      EdgevoxOnnxGetOfflineStreamResult(stream);

  fprintf(stderr, "Decoded text: %s\n", result->text);

  EdgevoxOnnxDestroyOfflineRecognizerResult(result);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
