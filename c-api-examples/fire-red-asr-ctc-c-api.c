// c-api-examples/fire-red-asr-ctc-c-api.c
//
// Copyright (c)  2026  Xiaomi Corporation

//
// This file demonstrates how to use FireRedASR with edgevox-onnx's C API.
// clang-format off
/*
wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
tar xvf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
rm edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
*/
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  // clang-format off
  const char *wav_filename = "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/test_wavs/1.wav";
  const char *model_filename = "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx";
  const char *tokens_filename = "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt";
  // clang-format on

  const char *provider = "cpu";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  // Recognizer config
  EdgevoxOnnxOfflineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config.debug = 1;
  recognizer_config.model_config.num_threads = 1;
  recognizer_config.model_config.provider = provider;
  recognizer_config.model_config.tokens = tokens_filename;
  recognizer_config.model_config.fire_red_asr_ctc.model = model_filename;

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
