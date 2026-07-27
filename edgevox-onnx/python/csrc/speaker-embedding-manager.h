// edgevox-onnx/python/csrc/speaker-embedding-manager.h
//
// Copyright (c)  2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_PYTHON_CSRC_SPEAKER_EMBEDDING_MANAGER_H_
#define EDGEVOX_ONNX_PYTHON_CSRC_SPEAKER_EMBEDDING_MANAGER_H_

#include "edgevox-onnx/python/csrc/edgevox-onnx.h"

namespace edgevox_onnx {

void PybindSpeakerEmbeddingManager(py::module *m);

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_PYTHON_CSRC_SPEAKER_EMBEDDING_MANAGER_H_
