// edgevox-onnx/python/csrc/online-paraformer-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/online-paraformer-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/csrc/online-paraformer-model-config.h"

namespace edgevox_onnx {

void PybindOnlineParaformerModelConfig(py::module *m) {
  using PyClass = OnlineParaformerModelConfig;
  py::class_<PyClass>(*m, "OnlineParaformerModelConfig")
      .def(py::init<const std::string &, const std::string &>(),
           py::arg("encoder"), py::arg("decoder"))
      .def_readwrite("encoder", &PyClass::encoder)
      .def_readwrite("decoder", &PyClass::decoder)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
