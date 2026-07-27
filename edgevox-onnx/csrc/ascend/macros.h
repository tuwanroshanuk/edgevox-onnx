// edgevox-onnx/csrc/ascend/macros.h
//
// Copyright      2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ASCEND_MACROS_H_
#define EDGEVOX_ONNX_CSRC_ASCEND_MACROS_H_

#include "edgevox-onnx/csrc/macros.h"

#define EDGEVOX_ONNX_ASCEND_CHECK(ret, msg, ...)    \
  do {                                             \
    if (ret != ACL_ERROR_NONE) {                   \
      const char *_msg = aclGetRecentErrMsg();     \
      EDGEVOX_ONNX_LOGE("Return code is: %d", ret); \
      EDGEVOX_ONNX_LOGE("Error message: %s", _msg); \
      EDGEVOX_ONNX_LOGE(msg, ##__VA_ARGS__);        \
      EDGEVOX_ONNX_EXIT(-1);                        \
    }                                              \
  } while (0)

#endif  // EDGEVOX_ONNX_CSRC_ASCEND_MACROS_H_
