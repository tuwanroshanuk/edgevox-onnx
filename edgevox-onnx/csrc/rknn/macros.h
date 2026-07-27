// edgevox-onnx/csrc/rknn/macros.h
//
// Copyright      2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_RKNN_MACROS_H_
#define EDGEVOX_ONNX_CSRC_RKNN_MACROS_H_

#include "edgevox-onnx/csrc/macros.h"

#define EDGEVOX_ONNX_RKNN_CHECK(ret, msg, ...)      \
  do {                                             \
    if (ret != RKNN_SUCC) {                        \
      EDGEVOX_ONNX_LOGE("Return code is: %d", ret); \
      EDGEVOX_ONNX_LOGE(msg, ##__VA_ARGS__);        \
      EDGEVOX_ONNX_EXIT(-1);                        \
    }                                              \
  } while (0)

#endif  // EDGEVOX_ONNX_CSRC_RKNN_MACROS_H_
