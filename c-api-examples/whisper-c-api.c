// c-api-examples/whisper-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

// We assume you have pre-downloaded the whisper multi-lingual models
// from https://github.com/k2-fsa/sherpa-onnx/releases/tag/asr-models
// An example command to download the "tiny" whisper model is given below:
//
// clang-format off
//
// wget https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-whisper-tiny.tar.bz2
// tar xvf sherpa-onnx-whisper-tiny.tar.bz2
// rm sherpa-onnx-whisper-tiny.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename = "./sherpa-onnx-whisper-tiny/test_wavs/0.wav";
  const char *encoder_filename = "sherpa-onnx-whisper-tiny/tiny-encoder.onnx";
  const char *decoder_filename = "sherpa-onnx-whisper-tiny/tiny-decoder.onnx";
  const char *tokens_filename = "sherpa-onnx-whisper-tiny/tiny-tokens.txt";
  const char *language = "en";
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
  recognizer_config.model_config.whisper.decoder = decoder_filename;
  recognizer_config.model_config.whisper.encoder = encoder_filename;
  recognizer_config.model_config.whisper.language = language;
  recognizer_config.model_config.whisper.tail_paddings = 0;
  recognizer_config.model_config.whisper.task = "transcribe";

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
