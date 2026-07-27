// edgevox-onnx/python/csrc/offline-dolphin-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-dolphin-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/python/csrc/offline-dolphin-model-config.h"

namespace edgevox_onnx {

void PybindOfflineDolphinModelConfig(py::module *m) {
  using PyClass = OfflineDolphinModelConfig;
  py::class_<PyClass>(*m, "OfflineDolphinModelConfig")
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
