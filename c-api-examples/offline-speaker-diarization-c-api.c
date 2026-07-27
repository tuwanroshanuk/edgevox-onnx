// c-api-examples/offline-sepaker-diarization-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

//
// This file demonstrates how to implement speaker diarization with
// edgevox-onnx's C API.

// clang-format off
/*
Usage:

Step 1: Download a speaker segmentation model

Please visit https://github.com/k2-fsa/edgevox-onnx/releases/tag/speaker-segmentation-models
for a list of available models. The following is an example

  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-segmentation-models/edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
  tar xvf edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
  rm edgevox-onnx-pyannote-segmentation-3-0.tar.bz2

Step 2: Download a speaker embedding extractor model

Please visit https://github.com/k2-fsa/edgevox-onnx/releases/tag/speaker-recongition-models
for a list of available models. The following is an example

  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-recongition-models/3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx

Step 3. Download test wave files

Please visit https://github.com/k2-fsa/edgevox-onnx/releases/tag/speaker-segmentation-models
for a list of available test wave files. The following is an example

  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-segmentation-models/0-four-speakers-zh.wav

Step 4. Run it

 */
// clang-format on

#include <stdio.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

static int32_t ProgressCallback(int32_t num_processed_chunks,
                                int32_t num_total_chunks, void *arg) {
  float progress = 100.0 * num_processed_chunks / num_total_chunks;
  fprintf(stderr, "progress %.2f%%\n", progress);

  // the return value is currently ignored
  return 0;
}

int main() {
  // Please see the comments at the start of this file for how to download
  // the .onnx file and .wav files below
  const char *segmentation_model =
      "./edgevox-onnx-pyannote-segmentation-3-0/model.onnx";

  const char *embedding_extractor_model =
      "./3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx";

  const char *wav_filename = "./0-four-speakers-zh.wav";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  EdgevoxOnnxOfflineSpeakerDiarizationConfig config;
  memset(&config, 0, sizeof(config));

  config.segmentation.pyannote.model = segmentation_model;
  config.embedding.model = embedding_extractor_model;

  // the test wave ./0-four-speakers-zh.wav has 4 speakers, so
  // we set num_clusters to 4
  //
  config.clustering.num_clusters = 4;
  // If you don't know the number of speakers in the test wave file, please
  // use
  // config.clustering.threshold = 0.5; // You need to tune this threshold

  const EdgevoxOnnxOfflineSpeakerDiarization *sd =
      EdgevoxOnnxCreateOfflineSpeakerDiarization(&config);

  if (!sd) {
    fprintf(stderr, "Failed to initialize offline speaker diarization\n");
    return -1;
  }

  if (EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(sd) !=
      wave->sample_rate) {
    fprintf(
        stderr,
        "Expected sample rate: %d. Actual sample rate from the wave file: %d\n",
        EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(sd),
        wave->sample_rate);
    goto failed;
  }

  const EdgevoxOnnxOfflineSpeakerDiarizationResult *result =
      EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback(
          sd, wave->samples, wave->num_samples, ProgressCallback, NULL);
  if (!result) {
    fprintf(stderr, "Failed to do speaker diarization");
    goto failed;
  }

  int32_t num_segments =
      EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(result);

  const EdgevoxOnnxOfflineSpeakerDiarizationSegment *segments =
      EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(result);

  for (int32_t i = 0; i != num_segments; ++i) {
    fprintf(stderr, "%.3f -- %.3f speaker_%02d\n", segments[i].start,
            segments[i].end, segments[i].speaker);
  }

failed:

  EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(segments);
  EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(result);
  EdgevoxOnnxDestroyOfflineSpeakerDiarization(sd);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
