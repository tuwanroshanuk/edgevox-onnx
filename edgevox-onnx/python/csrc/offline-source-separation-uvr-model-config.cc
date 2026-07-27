// edgevox-onnx/python/csrc/offline-source-separation-uvr-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/offline-source-separation-uvr-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/offline-source-separation-uvr-model-config.h"

namespace edgevox_onnx {

void PybindOfflineSourceSeparationUvrModelConfig(py::module *m) {
  using PyClass = OfflineSourceSeparationUvrModelConfig;
  py::class_<PyClass>(*m, "OfflineSourceSeparationUvrModelConfig")
      .def(py::init<const std::string &>(), py::arg("model") = "")
      .def_readwrite("model", &PyClass::model)
      .def("validate", &PyClass::Validate)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
