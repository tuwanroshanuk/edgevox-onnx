// c-api-examples/audio-tagging-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

// We assume you have pre-downloaded the model files for testing
// from https://github.com/k2-fsa/edgevox-onnx/releases/tag/audio-tagging-models
//
// An example is given below:
//
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
// tar xvf edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
// rm edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  EdgevoxOnnxAudioTaggingConfig config;
  memset(&config, 0, sizeof(config));

  config.model.zipformer.model =
      "./edgevox-onnx-zipformer-audio-tagging-2024-04-09/model.int8.onnx";
  config.model.num_threads = 1;
  config.model.debug = 1;
  config.model.provider = "cpu";
  // clang-format off
  config.labels = "./edgevox-onnx-zipformer-audio-tagging-2024-04-09/class_labels_indices.csv";
  // clang-format on

  const EdgevoxOnnxAudioTagging *tagger = EdgevoxOnnxCreateAudioTagging(&config);
  if (!tagger) {
    fprintf(stderr, "Failed to create audio tagger. Please check your config");
    return -1;
  }

  // You can find more test waves from
  // https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
  const char *wav_filename =
      "./edgevox-onnx-zipformer-audio-tagging-2024-04-09/test_wavs/1.wav";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  const EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxAudioTaggingCreateOfflineStream(tagger);

  EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
                                  wave->num_samples);

  int32_t top_k = 5;
  const EdgevoxOnnxAudioEvent *const *results =
      EdgevoxOnnxAudioTaggingCompute(tagger, stream, top_k);

  fprintf(stderr, "--------------------------------------------------\n");
  fprintf(stderr, "Index\t\tProbability\t\tEvent name\n");
  fprintf(stderr, "--------------------------------------------------\n");
  for (int32_t i = 0; i != top_k; ++i) {
    fprintf(stderr, "%d\t\t%.3f\t\t\t%s\n", i, results[i]->prob,
            results[i]->name);
  }
  fprintf(stderr, "--------------------------------------------------\n");

  EdgevoxOnnxAudioTaggingFreeResults(results);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxFreeWave(wave);
  EdgevoxOnnxDestroyAudioTagging(tagger);

  return 0;
};
