// edgevox-onnx/python/csrc/offline-wenet-ctc-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-wenet-ctc-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/python/csrc/offline-wenet-ctc-model-config.h"

namespace edgevox_onnx {

void PybindOfflineWenetCtcModelConfig(py::module *m) {
  using PyClass = OfflineWenetCtcModelConfig;
  py::class_<PyClass>(*m, "OfflineWenetCtcModelConfig")
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
