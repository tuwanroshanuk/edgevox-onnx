#include "edgevox-onnx/python/csrc/offline-tts-chatterbox-model-config.h"

#include "edgevox-onnx/csrc/offline-tts-chatterbox-model-config.h"

namespace edgevox_onnx {

void PybindOfflineTtsChatterboxModelConfig(py::module *m) {
  using PyClass = OfflineTtsChatterboxModelConfig;
  py::class_<PyClass>(*m, "OfflineTtsChatterboxModelConfig")
      .def(py::init<>())
      .def_readwrite("speech_encoder", &PyClass::speech_encoder)
      .def_readwrite("embed_tokens", &PyClass::embed_tokens)
      .def_readwrite("language_model", &PyClass::language_model)
      .def_readwrite("conditional_decoder", &PyClass::conditional_decoder)
      .def_readwrite("tokenizer", &PyClass::tokenizer)
      .def("__str__", &PyClass::ToString);
}

}  // namespace edgevox_onnx
