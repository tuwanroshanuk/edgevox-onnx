// edgevox-onnx/python/csrc/offline-medasr-ctc-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-medasr-ctc-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/python/csrc/offline-medasr-ctc-model-config.h"

namespace edgevox_onnx {

void PybindOfflineMedAsrCtcModelConfig(py::module *m) {
  using PyClass = OfflineMedAsrCtcModelConfig;
  py::class_<PyClass>(*m, "OfflineMedAsrCtcModelConfig")
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
