// c-api-examples/nemo-giga-am-v2-c-api.c
//
// Copyright (c)  2026  Xiaomi Corporation

//
// This file demonstrates how to use the NeMo transducer GigaAM v2 model
// with edgevox-onnx's C API.
//
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19.tar.bz2
// tar xvf edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19.tar.bz2
// rm edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "./edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19/"
      "test_wavs/example.wav";
  const char *encoder_filename =
      "./edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19/"
      "encoder.int8.onnx";
  const char *decoder_filename =
      "./edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19/"
      "decoder.onnx";
  const char *joiner_filename =
      "./edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19/"
      "joiner.onnx";
  const char *tokens_filename =
      "./edgevox-onnx-nemo-transducer-giga-am-v2-russian-2025-04-19/"
      "tokens.txt";
  const char *provider = "cpu";

  if (!EdgevoxOnnxFileExists(wav_filename)) {
    fprintf(stderr, "File not found: %s\n", wav_filename);
    return -1;
  }
  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read or parse %s\n", wav_filename);
    return -1;
  }

  EdgevoxOnnxOfflineModelConfig offline_model_config;
  memset(&offline_model_config, 0, sizeof(offline_model_config));
  offline_model_config.debug = 0;
  offline_model_config.num_threads = 1;
  offline_model_config.provider = provider;
  offline_model_config.tokens = tokens_filename;
  offline_model_config.transducer.encoder = encoder_filename;
  offline_model_config.transducer.decoder = decoder_filename;
  offline_model_config.transducer.joiner = joiner_filename;

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

  printf("Recognized text: %s\n", result->text);

  EdgevoxOnnxDestroyOfflineRecognizerResult(result);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
