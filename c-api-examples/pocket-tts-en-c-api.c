// c-api-examples/pocket-tts-en-c-api.c
//
// Copyright (c)  2026  Xiaoyingtao Corporation

// This file shows how to use edgevox-onnx C API
// for English TTS with Pocket TTS.
//
// clang-format off
/*
Usage

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
tar xf edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
rm edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2

./pocket-tts-en-c-api

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
  config.model.pocket.lm_flow =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx";
  config.model.pocket.lm_main =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx";
  config.model.pocket.encoder =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx";
  config.model.pocket.decoder =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx";
  config.model.pocket.text_conditioner =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx";
  config.model.pocket.vocab_json =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json";
  config.model.pocket.token_scores_json =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json";
  // Voice embedding cache capacity (default: 50)
  // Increase this if you have many different reference audios to avoid
  // recomputing voice embeddings
  config.model.pocket.voice_embedding_cache_capacity = 50;

  config.model.num_threads = 2;

  // If you don't want to see debug messages, please set it to 0
  config.model.debug = 1;

  const char *filename = "./generated-pocket-en.wav";
  const char *text =
      "Today as always, men fall into two groups: slaves and free men. Whoever "
      "does not have two-thirds of his day for himself, is a slave, whatever "
      "he may be: a statesman, a businessman, an official, or a scholar. "
      "Friends fell out often because life was changing so fast. The easiest "
      "thing in the world was to lose touch with someone.";

  const EdgevoxOnnxOfflineTts *tts = EdgevoxOnnxCreateOfflineTts(&config);
  if (!tts) {
    fprintf(stderr, "Error create Offline TTS\n");
    return -1;
  }
  float speed = 1.0;  // larger -> faster in speech speed
  EdgevoxOnnxGenerationConfig cfg = {0};
  const char *reference_audio_file =
      "./edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav";
  const EdgevoxOnnxWave *wave = NULL;
  wave = EdgevoxOnnxReadWave(reference_audio_file);
  if (!wave) {
    fprintf(stderr, "Failed to read %s\n", reference_audio_file);
    EdgevoxOnnxDestroyOfflineTts(tts);
    return -1;
  }
  cfg.reference_audio = wave->samples;
  cfg.reference_audio_len = wave->num_samples;
  cfg.reference_sample_rate = wave->sample_rate;
  // Extra parameters passed as JSON string
  // - max_reference_audio_len: maximum length of reference audio in seconds
  // - seed: random seed for reproducibility (optional, -1 for random)
  cfg.extra = "{\"max_reference_audio_len\": 10.0, \"seed\": 42}";

#if 0
  // If you don't want to use a callback, then please enable this branch
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, NULL, NULL);
#else
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, ProgressCallback,
                                             NULL);
#endif

  if (wave) EdgevoxOnnxFreeWave(wave);

  fprintf(stderr, "Input text is: %s\n", text);

  if (audio) {
    EdgevoxOnnxWriteWave(audio->samples, audio->n, audio->sample_rate, filename);
    fprintf(stderr, "Saved to: %s\n", filename);
    EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);
  }

  EdgevoxOnnxDestroyOfflineTts(tts);

  return 0;
}
