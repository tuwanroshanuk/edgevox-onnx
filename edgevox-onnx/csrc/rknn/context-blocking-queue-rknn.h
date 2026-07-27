// edgevox-onnx/csrc/rknn/context-blocking-queue-rknn.h
//
// Copyright      2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_RKNN_CONTEXT_BLOCKING_QUEUE_RKNN_H_
#define EDGEVOX_ONNX_CSRC_RKNN_CONTEXT_BLOCKING_QUEUE_RKNN_H_

#include <memory>

#include "rknn_api.h"  // NOLINT

namespace edgevox_onnx {

class ContextBlockingQueueRknn {
 public:
  ContextBlockingQueueRknn(rknn_context context, int32_t num_threads,
                           int32_t capacity = 10);
  ~ContextBlockingQueueRknn();

  rknn_context Take();
  void Put(rknn_context context);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_RKNN_CONTEXT_BLOCKING_QUEUE_RKNN_H_
