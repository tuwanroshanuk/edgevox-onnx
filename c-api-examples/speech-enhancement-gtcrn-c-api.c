// c-api-examples/speech-enhancement-gtcrn-c-api.c
//
// Copyright (c)  2025  Xiaomi Corporation
//
// We assume you have pre-downloaded model
// from
// https://github.com/k2-fsa/sherpa-onnx/releases/tag/speech-enhancement-models
//
//
// An example command to download
// clang-format off
/*
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/gtcrn_simple.onnx
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/inp_16k.wav
*/
// clang-format on
#include <stdio.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  EdgevoxOnnxOfflineSpeechDenoiserConfig config;
  const char *model_filename = "./gtcrn_simple.onnx";
  const char *wav_filename = "./inp_16k.wav";
  const char *out_wave_filename = "./enhanced.wav";

  memset(&config, 0, sizeof(config));
  config.model.gtcrn.model = model_filename;

  const EdgevoxOnnxOfflineSpeechDenoiser *sd =
      EdgevoxOnnxCreateOfflineSpeechDenoiser(&config);
  if (!sd) {
    fprintf(stderr, "Please check your config");
    return -1;
  }

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    EdgevoxOnnxDestroyOfflineSpeechDenoiser(sd);
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  const EdgevoxOnnxDenoisedAudio *denoised = EdgevoxOnnxOfflineSpeechDenoiserRun(
      sd, wave->samples, wave->num_samples, wave->sample_rate);

  EdgevoxOnnxWriteWave(denoised->samples, denoised->n, denoised->sample_rate,
                      out_wave_filename);

  EdgevoxOnnxDestroyDenoisedAudio(denoised);
  EdgevoxOnnxFreeWave(wave);
  EdgevoxOnnxDestroyOfflineSpeechDenoiser(sd);

  fprintf(stdout, "Saved to %s\n", out_wave_filename);
}
