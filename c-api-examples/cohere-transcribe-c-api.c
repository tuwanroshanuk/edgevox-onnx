// c-api-examples/cohere-transcribe-c-api.c
//
// Copyright (c)  2026  Xiaomi Corporation

// We assume you have pre-downloaded the Cohere Transcribe model
// from https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
// An example is given below:
//
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
// tar xvf edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
// rm edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
//
// clang-format on

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/test_wavs/"
      "en.wav";
  const char *encoder_filename =
      "edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx";
  const char *decoder_filename =
      "edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/decoder.int8.onnx";
  const char *tokens_filename =
      "edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/tokens.txt";
  const char *provider = "cpu";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  EdgevoxOnnxOfflineModelConfig offline_model_config;
  memset(&offline_model_config, 0, sizeof(offline_model_config));
  offline_model_config.debug = 0;
  offline_model_config.num_threads = 1;
  offline_model_config.provider = provider;
  offline_model_config.tokens = tokens_filename;
  offline_model_config.cohere_transcribe.encoder = encoder_filename;
  offline_model_config.cohere_transcribe.decoder = decoder_filename;
  offline_model_config.cohere_transcribe.use_punct = 1;
  offline_model_config.cohere_transcribe.use_itn = 1;

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

  // we are testing an English wave file, so we use the language en
  EdgevoxOnnxOfflineStreamSetOption(stream, "language", "en");

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
