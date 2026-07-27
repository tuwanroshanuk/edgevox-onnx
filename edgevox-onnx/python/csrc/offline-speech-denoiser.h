// edgevox-onnx/python/csrc/offline-speech-denoiser.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_PYTHON_CSRC_OFFLINE_SPEECH_DENOISER_H_
#define EDGEVOX_ONNX_PYTHON_CSRC_OFFLINE_SPEECH_DENOISER_H_

#include "edgevox-onnx/python/csrc/edgevox-onnx.h"

namespace edgevox_onnx {

void PybindDenoisedAudio(py::module *m);

void PybindOfflineSpeechDenoiser(py::module *m);

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_PYTHON_CSRC_OFFLINE_SPEECH_DENOISER_H_
