// edgevox-onnx/python/csrc/online-transducer-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/online-transducer-model-config.h"

#include <string>

#include "edgevox-onnx/python/csrc/online-transducer-model-config.h"

namespace edgevox_onnx {

void PybindOnlineTransducerModelConfig(py::module *m) {
  using PyClass = OnlineTransducerModelConfig;
  py::class_<PyClass>(*m, "OnlineTransducerModelConfig")
      .def(py::init<const std::string &, const std::string &,
                    const std::string &>(),
           py::arg("encoder"), py::arg("decoder"), py::arg("joiner"))
      .def_readwrite("encoder", &PyClass::encoder)
      .def_readwrite("decoder", &PyClass::decoder)
      .def_readwrite("joiner", &PyClass::joiner)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
