// edgevox-onnx/python/csrc/offline-tts-kitten-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/offline-tts-kitten-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/offline-tts-kitten-model-config.h"

namespace edgevox_onnx {

void PybindOfflineTtsKittenModelConfig(py::module *m) {
  using PyClass = OfflineTtsKittenModelConfig;

  py::class_<PyClass>(*m, "OfflineTtsKittenModelConfig")
      .def(py::init<>())
      .def(py::init<const std::string &, const std::string &,
                    const std::string &, const std::string &, float>(),
           py::arg("model"), py::arg("voices"), py::arg("tokens"),
           py::arg("data_dir"), py::arg("length_scale") = 1.0)
      .def_readwrite("model", &PyClass::model)
      .def_readwrite("voices", &PyClass::voices)
      .def_readwrite("tokens", &PyClass::tokens)
      .def_readwrite("data_dir", &PyClass::data_dir)
      .def_readwrite("length_scale", &PyClass::length_scale)
      .def("__str__", &PyClass::ToString)
      .def("validate", &PyClass::Validate);
}

}  // namespace edgevox_onnx
