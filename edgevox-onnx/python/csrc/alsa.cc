// edgevox-onnx/python/csrc/alsa.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/alsa.h"

#include <vector>

#include "edgevox-onnx/csrc/alsa.h"

namespace edgevox_onnx {

void PybindAlsa(py::module *m) {
  using PyClass = Alsa;
  py::class_<PyClass>(*m, "Alsa")
      .def(py::init<const char *>(), py::arg("device_name"),
           py::call_guard<py::gil_scoped_release>())
      .def(
          "read",
          [](PyClass &self, int32_t num_samples) -> std::vector<float> {
            return self.Read(num_samples);
          },
          py::arg("num_samples"), py::call_guard<py::gil_scoped_release>())
      .def_property_readonly("expected_sample_rate",
                             &PyClass::GetExpectedSampleRate)
      .def_property_readonly("actual_sample_rate",
                             &PyClass::GetActualSampleRate);
}

}  // namespace edgevox_onnx
