// edgevox-onnx/python/csrc/online-t-one-ctc-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/online-t-one-ctc-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/csrc/online-t-one-ctc-model-config.h"

namespace edgevox_onnx {

void PybindOnlineToneCtcModelConfig(py::module *m) {
  using PyClass = OnlineToneCtcModelConfig;
  py::class_<PyClass>(*m, "OnlineToneCtcModelConfig")
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
