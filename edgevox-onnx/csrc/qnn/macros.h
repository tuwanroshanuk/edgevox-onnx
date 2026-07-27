// edgevox-onnx/csrc/qnn/macros.h
//
// Copyright      2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_QNN_MACROS_H_
#define EDGEVOX_ONNX_CSRC_QNN_MACROS_H_

#include "edgevox-onnx/csrc/macros.h"

#define EDGEVOX_ONNX_QNN_CHECK(ret, msg, ...)                             \
  do {                                                                   \
    if (ret != QNN_SUCCESS) {                                            \
      EDGEVOX_ONNX_LOGE("Return code is: %d", static_cast<int32_t>(ret)); \
      EDGEVOX_ONNX_LOGE(msg, ##__VA_ARGS__);                              \
      EDGEVOX_ONNX_EXIT(-1);                                              \
    }                                                                    \
  } while (0)

#endif  // EDGEVOX_ONNX_CSRC_QNN_MACROS_H_
