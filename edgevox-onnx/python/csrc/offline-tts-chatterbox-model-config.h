// edgevox-onnx/python/csrc/offline-tts-chatterbox-model-config.h
#ifndef EDGEVOX_ONNX_PYTHON_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_CONFIG_H_
#define EDGEVOX_ONNX_PYTHON_CSRC_OFFLINE_TTS_CHATTERBOX_MODEL_CONFIG_H_

#include "pybind11/pybind11.h"

namespace edgevox_onnx {
namespace py = pybind11;
void PybindOfflineTtsChatterboxModelConfig(py::module *m);
}  // namespace edgevox_onnx

#endif
