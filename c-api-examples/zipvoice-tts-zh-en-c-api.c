// c-api-examples/zipvoice-tts-zh-en-c-api.c
//
// Copyright (c)  2026  Xiaomi Corporation

// This file shows how to use edgevox-onnx C API
// for Chinese/English zero-shot TTS with ZipVoice.
//
// clang-format off
/*
Usage

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
tar xf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
rm edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos_24khz.onnx

./zipvoice-tts-zh-en-c-api
*/
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

static int32_t ProgressCallback(const float *samples, int32_t num_samples,
                                float progress, void *arg) {
  fprintf(stderr, "Progress: %.3f%%\n", progress * 100);
  return 1;
}

int32_t main(int32_t argc, char *argv[]) {
  EdgevoxOnnxOfflineTtsConfig config;
  memset(&config, 0, sizeof(config));
  config.model.zipvoice.encoder =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx";
  config.model.zipvoice.decoder =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/decoder.int8.onnx";
  config.model.zipvoice.data_dir =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/espeak-ng-data";
  config.model.zipvoice.lexicon =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/lexicon.txt";
  config.model.zipvoice.tokens =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/tokens.txt";
  config.model.zipvoice.vocoder = "./vocos_24khz.onnx";
  config.model.zipvoice.espeak_voice = "en-us";

  config.model.num_threads = 2;

  // If you want to see more debug messages, please set it to 1
  config.model.debug = 0;

  const char *filename = "./generated-zipvoice-zh-en-c.wav";
  const char *text =
      "小米的价值观是真诚, 热爱. 真诚，就是不欺人也不自欺. 热爱, "
      "就是全心投入并享受其中.";
  const char *reference_text =
      "那还是三十六年前, 一九八七年. 我呢考上了武汉大学的计算机系.";
  const char *reference_audio_file =
      "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/test_wavs/leijun-1.wav";

  const EdgevoxOnnxOfflineTts *tts = EdgevoxOnnxCreateOfflineTts(&config);
  if (!tts) {
    fprintf(stderr, "Error create Offline TTS\n");
    return -1;
  }

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(reference_audio_file);
  if (!wave) {
    fprintf(stderr, "Failed to read %s\n", reference_audio_file);
    EdgevoxOnnxDestroyOfflineTts(tts);
    return -1;
  }

  EdgevoxOnnxGenerationConfig cfg = {0};
  cfg.speed = 1.0f;
  cfg.num_steps = 4;
  cfg.reference_audio = wave->samples;
  cfg.reference_audio_len = wave->num_samples;
  cfg.reference_sample_rate = wave->sample_rate;
  cfg.reference_text = reference_text;
  cfg.extra = "{\"min_char_in_sentence\": 10}";

#if 0
  // If you don't want to use a callback, then please enable this branch
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, NULL, NULL);
#else
  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text, &cfg, ProgressCallback,
                                             NULL);
#endif

  EdgevoxOnnxFreeWave(wave);

  fprintf(stderr, "Input text is: %s\n", text);

  if (audio) {
    EdgevoxOnnxWriteWave(audio->samples, audio->n, audio->sample_rate, filename);
    fprintf(stderr, "Saved to: %s\n", filename);
    EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);
  }

  EdgevoxOnnxDestroyOfflineTts(tts);

  return 0;
}
