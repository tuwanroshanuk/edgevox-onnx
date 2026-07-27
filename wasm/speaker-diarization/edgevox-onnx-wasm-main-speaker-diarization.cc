// wasm/edgevox-onnx-wasm-main-speaker-diarization.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include <stdio.h>

#include <algorithm>
#include <memory>

#include "edgevox-onnx/c-api/c-api.h"

// see also
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

extern "C" {

static_assert(sizeof(EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig) ==
                  1 * 4,
              "");

static_assert(
    sizeof(EdgevoxOnnxOfflineSpeakerSegmentationModelConfig) ==
        sizeof(EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig) + 3 * 4,
    "");

static_assert(sizeof(EdgevoxOnnxFastClusteringConfig) == 2 * 4, "");

static_assert(sizeof(EdgevoxOnnxSpeakerEmbeddingExtractorConfig) == 4 * 4, "");

static_assert(sizeof(EdgevoxOnnxOfflineSpeakerDiarizationConfig) ==
                  sizeof(EdgevoxOnnxOfflineSpeakerSegmentationModelConfig) +
                      sizeof(EdgevoxOnnxSpeakerEmbeddingExtractorConfig) +
                      sizeof(EdgevoxOnnxFastClusteringConfig) + 2 * 4,
              "");

void MyPrint(const EdgevoxOnnxOfflineSpeakerDiarizationConfig *sd_config) {
  const auto &segmentation = sd_config->segmentation;
  const auto &embedding = sd_config->embedding;
  const auto &clustering = sd_config->clustering;

  fprintf(stdout, "----------segmentation config----------\n");
  fprintf(stdout, "pyannote model: %s\n", segmentation.pyannote.model);
  fprintf(stdout, "num threads: %d\n", segmentation.num_threads);
  fprintf(stdout, "debug: %d\n", segmentation.debug);
  fprintf(stdout, "provider: %s\n", segmentation.provider);

  fprintf(stdout, "----------embedding config----------\n");
  fprintf(stdout, "model: %s\n", embedding.model);
  fprintf(stdout, "num threads: %d\n", embedding.num_threads);
  fprintf(stdout, "debug: %d\n", embedding.debug);
  fprintf(stdout, "provider: %s\n", embedding.provider);

  fprintf(stdout, "----------clustering config----------\n");
  fprintf(stdout, "num_clusters: %d\n", clustering.num_clusters);
  fprintf(stdout, "threshold: %.3f\n", clustering.threshold);

  fprintf(stdout, "min_duration_on: %.3f\n", sd_config->min_duration_on);
  fprintf(stdout, "min_duration_off: %.3f\n", sd_config->min_duration_off);
}

void CopyHeap(const char *src, int32_t num_bytes, char *dst) {
  std::copy(src, src + num_bytes, dst);
}
}
