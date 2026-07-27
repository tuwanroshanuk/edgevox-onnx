// c-api-examples/spoken-language-identification-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

// We assume you have pre-downloaded the whisper multi-lingual models
// from https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
// An example command to download the "tiny" whisper model is given below:
//
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
// tar xvf edgevox-onnx-whisper-tiny.tar.bz2
// rm edgevox-onnx-whisper-tiny.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  EdgevoxOnnxSpokenLanguageIdentificationConfig config;

  memset(&config, 0, sizeof(config));

  config.whisper.encoder = "./edgevox-onnx-whisper-tiny/tiny-encoder.int8.onnx";
  config.whisper.decoder = "./edgevox-onnx-whisper-tiny/tiny-decoder.int8.onnx";
  config.num_threads = 1;
  config.debug = 1;
  config.provider = "cpu";

  const EdgevoxOnnxSpokenLanguageIdentification *slid =
      EdgevoxOnnxCreateSpokenLanguageIdentification(&config);
  if (!slid) {
    fprintf(stderr, "Failed to create spoken language identifier");
    return -1;
  }

  // You can find more test waves from
  // https://hf-mirror.com/spaces/k2-fsa/spoken-language-identification/tree/main/test_wavs
  const char *wav_filename = "./edgevox-onnx-whisper-tiny/test_wavs/0.wav";
  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(slid);

  EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
                                  wave->num_samples);

  const EdgevoxOnnxSpokenLanguageIdentificationResult *result =
      EdgevoxOnnxSpokenLanguageIdentificationCompute(slid, stream);

  fprintf(stderr, "wav_filename: %s\n", wav_filename);
  fprintf(stderr, "Detected language: %s\n", result->lang);

  EdgevoxOnnxDestroySpokenLanguageIdentificationResult(result);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxFreeWave(wave);
  EdgevoxOnnxDestroySpokenLanguageIdentification(slid);

  return 0;
}
