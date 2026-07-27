// edgevox-onnx/python/csrc/offline-tdnn-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-tdnn-model-config.h"

#include <string>
#include <vector>

#include "edgevox-onnx/python/csrc/offline-tdnn-model-config.h"

namespace edgevox_onnx {

void PybindOfflineTdnnModelConfig(py::module *m) {
  using PyClass = OfflineTdnnModelConfig;
  py::class_<PyClass>(*m, "OfflineTdnnModelConfig")
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
