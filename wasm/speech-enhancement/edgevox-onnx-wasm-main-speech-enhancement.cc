// wasm/edgevox-onnx-wasm-main-speech-enhancement.cc
//
// Copyright (c)  2025  Xiaomi Corporation
#include <stdio.h>

#include <algorithm>
#include <memory>

#include "edgevox-onnx/c-api/c-api.h"

// see also
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

extern "C" {

static_assert(sizeof(EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig) == 1 * 4,
              "");
static_assert(sizeof(EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig) ==
                  1 * 4,
              "");
static_assert(
    sizeof(EdgevoxOnnxOfflineSpeechDenoiserModelConfig) ==
        sizeof(EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig) +
            sizeof(EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig) + 3 * 4,
    "");
static_assert(sizeof(EdgevoxOnnxOfflineSpeechDenoiserConfig) ==
                  sizeof(EdgevoxOnnxOfflineSpeechDenoiserModelConfig),
              "");

void MyPrint(EdgevoxOnnxOfflineSpeechDenoiserConfig *config) {
  auto model = &config->model;
  auto gtcrn = &model->gtcrn;
  auto dpdfnet = &model->dpdfnet;
  fprintf(stdout, "----------offline speech denoiser model config----------\n");
  fprintf(stdout, "gtcrn: %s\n", gtcrn->model);
  fprintf(stdout, "dpdfnet: %s\n", dpdfnet->model);
  fprintf(stdout, "num threads: %d\n", model->num_threads);
  fprintf(stdout, "debug: %d\n", model->debug);
  fprintf(stdout, "provider: %s\n", model->provider);
}

void CopyHeap(const char *src, int32_t num_bytes, char *dst) {
  std::copy(src, src + num_bytes, dst);
}
}
