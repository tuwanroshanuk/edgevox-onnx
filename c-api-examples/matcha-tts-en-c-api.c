// c-api-examples/matcha-tts-en-c-api.c
//
// Copyright (c)  2025  Xiaomi Corporation

// This file shows how to use edgevox-onnx C API
// for English TTS with MatchaTTS.
//
// clang-format off
/*
Usage

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/matcha-icefall-en_US-ljspeech.tar.bz2
tar xvf matcha-icefall-en_US-ljspeech.tar.bz2
rm matcha-icefall-en_US-ljspeech.tar.bz2

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos-22khz-univ.onnx

./matcha-tts-en-c-api

 */
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

static int32_t ProgressCallback(const float *samples, int32_t num_samples,
                                float progress, void *arg) {
  fprintf(stderr, "Progress: %.3f%%\n", progress * 100);
  // return 1 to continue generating
  // return 0 to stop generating
  return 1;
}

int32_t main(int32_t argc, char *argv[]) {
  EdgevoxOnnxOfflineTtsConfig config;
  memset(&config, 0, sizeof(config));
  config.model.matcha.acoustic_model =
      "./matcha-icefall-en_US-ljspeech/model-steps-3.onnx";

  config.model.matcha.vocoder = "./vocos-22khz-univ.onnx";

  config.model.matcha.tokens = "./matcha-icefall-en_US-ljspeech/tokens.txt";

  config.model.matcha.data_dir =
      "./matcha-icefall-en_US-ljspeech/espeak-ng-data";

  config.model.num_threads = 1;

  // If you don't want to see debug messages, please set it to 0
  config.model.debug = 1;

  const char *filename = "./generated-matcha-en.wav";
  const char *text =
      "Today as always, men fall into two groups: slaves and free men. Whoever "
      "does not have two-thirds of his day for himself, is a slave, whatever "
      "he may be: a statesman, a businessman, an official, or a scholar. "
      "Friends fell out often because life was changing so fast. The easiest "
      "thing in the world was to lose touch with someone.";

  const EdgevoxOnnxOfflineTts *tts = EdgevoxOnnxCreateOfflineTts(&config);
  EdgevoxOnnxGenerationConfig cfg = {0};
  cfg.sid = 0;
  cfg.speed = 1.0f;  // larger -> faster in speech speed
  cfg.silence_scale = config.silence_scale;

#if 0
  // If you don't want to use a callback, then please enable this branch
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, NULL, NULL);
#else
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, ProgressCallback,
                                             NULL);
#endif

  EdgevoxOnnxWriteWave(audio->samples, audio->n, audio->sample_rate, filename);

  EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);
  EdgevoxOnnxDestroyOfflineTts(tts);

  fprintf(stderr, "Input text is: %s\n", text);
  fprintf(stderr, "Speaker ID is: %d\n", cfg.sid);
  fprintf(stderr, "Saved to: %s\n", filename);

  return 0;
}
