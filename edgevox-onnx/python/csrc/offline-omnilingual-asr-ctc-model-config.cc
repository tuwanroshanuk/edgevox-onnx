// edgevox-onnx/python/csrc/offline-omnilingual-asr-ctc-model-config.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/offline-omnilingual-asr-ctc-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/offline-omnilingual-asr-ctc-model-config.h"

namespace edgevox_onnx {

void PybindOfflineOmnilingualAsrCtcModelConfig(py::module *m) {
  using PyClass = OfflineOmnilingualAsrCtcModelConfig;
  py::class_<PyClass>(*m, "OfflineOmnilingualAsrCtcModelConfig")
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("model"))
      .def_readwrite("model", &PyClass::model)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
