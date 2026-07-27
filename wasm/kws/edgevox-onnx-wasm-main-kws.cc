// wasm/edgevox-onnx-wasm-main-kws.cc
//
// Copyright (c)  2024  lovemefan
#include <stdio.h>

#include <algorithm>
#include <memory>

#include "edgevox-onnx/c-api/c-api.h"

// see also
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

extern "C" {

static_assert(sizeof(EdgevoxOnnxOnlineTransducerModelConfig) == 3 * 4, "");
static_assert(sizeof(EdgevoxOnnxOnlineParaformerModelConfig) == 2 * 4, "");
static_assert(sizeof(EdgevoxOnnxOnlineZipformer2CtcModelConfig) == 1 * 4, "");
static_assert(sizeof(EdgevoxOnnxOnlineModelConfig) ==
                  sizeof(EdgevoxOnnxOnlineTransducerModelConfig) +
                      sizeof(EdgevoxOnnxOnlineParaformerModelConfig) +
                      sizeof(EdgevoxOnnxOnlineZipformer2CtcModelConfig) + 9 * 4 +
                      sizeof(EdgevoxOnnxOnlineNemoCtcModelConfig) +
                      sizeof(EdgevoxOnnxOnlineToneCtcModelConfig),
              "");
static_assert(sizeof(EdgevoxOnnxFeatureConfig) == 2 * 4, "");
static_assert(sizeof(EdgevoxOnnxKeywordSpotterConfig) ==
                  sizeof(EdgevoxOnnxFeatureConfig) +
                      sizeof(EdgevoxOnnxOnlineModelConfig) + 7 * 4,
              "");

void CopyHeap(const char *src, int32_t num_bytes, char *dst) {
  std::copy(src, src + num_bytes, dst);
}
}
