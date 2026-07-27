// edgevox-onnx/python/csrc/vad-model-config.h
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/vad-model-config.h"

#include <string>

#include "edgevox-onnx/csrc/vad-model-config.h"
#include "edgevox-onnx/python/csrc/silero-vad-model-config.h"
#include "edgevox-onnx/python/csrc/ten-vad-model-config.h"

namespace edgevox_onnx {

void PybindVadModelConfig(py::module *m) {
  PybindSileroVadModelConfig(m);
  PybindTenVadModelConfig(m);

  using PyClass = VadModelConfig;
  py::class_<PyClass>(*m, "VadModelConfig")
      .def(py::init<>())
      .def(py::init<const SileroVadModelConfig &, const TenVadModelConfig &,
                    int32_t, int32_t, const std::string &, bool>(),
           py::arg("silero_vad") = SileroVadModelConfig{},
           py::arg("ten_vad") = TenVadModelConfig{},
           py::arg("sample_rate") = 16000, py::arg("num_threads") = 1,
           py::arg("provider") = "cpu", py::arg("debug") = false)
      .def_readwrite("silero_vad", &PyClass::silero_vad)
      .def_readwrite("ten_vad", &PyClass::ten_vad)
      .def_readwrite("sample_rate", &PyClass::sample_rate)
      .def_readwrite("num_threads", &PyClass::num_threads)
      .def_readwrite("provider", &PyClass::provider)
      .def_readwrite("debug", &PyClass::debug)
      .def("__str__", &PyClass::ToString)
      .def("validate", &PyClass::Validate);
}

}  // namespace edgevox_onnx
