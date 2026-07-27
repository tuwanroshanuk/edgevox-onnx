// edgevox-onnx/python/csrc/offline-speech-denoiser-gtcrn-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/offline-speech-denoiser-gtcrn-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/offline-speech-denoiser-gtcrn-model-config.h"

namespace edgevox_onnx {

void PybindOfflineSpeechDenoiserGtcrnModelConfig(py::module *m) {
  using PyClass = OfflineSpeechDenoiserGtcrnModelConfig;
  py::class_<PyClass>(*m, "OfflineSpeechDenoiserGtcrnModelConfig")
      .def(py::init<const std::string &>(), py::arg("model") = "")
      .def_readwrite("model", &PyClass::model)
      .def("validate", &PyClass::Validate)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
