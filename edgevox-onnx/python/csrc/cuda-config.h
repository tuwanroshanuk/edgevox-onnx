// edgevox-onnx/python/csrc/cuda-config.h
//
// Copyright (c)  2024  Uniphore (Author: Manickavela A)

#ifndef EDGEVOX_ONNX_PYTHON_CSRC_CUDA_CONFIG_H_
#define EDGEVOX_ONNX_PYTHON_CSRC_CUDA_CONFIG_H_

#include "edgevox-onnx/python/csrc/edgevox-onnx.h"

namespace edgevox_onnx {

void PybindCudaConfig(py::module *m);

}

#endif  // EDGEVOX_ONNX_PYTHON_CSRC_CUDA_CONFIG_H_
