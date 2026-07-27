// edgevox-onnx/python/csrc/offline-speech-denoiser-dpdfnet-model-config.cc
//
// Copyright (c)  2026  Ceva Inc

#include "edgevox-onnx/python/csrc/offline-speech-denoiser-dpdfnet-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/offline-speech-denoiser-dpdfnet-model-config.h"

namespace edgevox_onnx {

void PybindOfflineSpeechDenoiserDpdfNetModelConfig(py::module *m) {
  using PyClass = OfflineSpeechDenoiserDpdfNetModelConfig;
  py::class_<PyClass>(*m, "OfflineSpeechDenoiserDpdfNetModelConfig")
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("model") = "")
      .def_readwrite("model", &PyClass::model)
      .def("validate", &PyClass::Validate)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
