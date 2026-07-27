// edgevox-onnx/csrc/axcl/axcl-manager.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_AXCL_AXCL_MANAGER_H_
#define EDGEVOX_ONNX_CSRC_AXCL_AXCL_MANAGER_H_

#include <cstdint>
#include <mutex>

namespace edgevox_onnx {

class AxclManager {
 public:
  explicit AxclManager(const char *config = nullptr);
  ~AxclManager();

  AxclManager(const AxclManager &) = delete;
  AxclManager &operator=(const AxclManager &) = delete;

  AxclManager(AxclManager &&) = delete;
  AxclManager &operator=(AxclManager &&) = delete;

 private:
  static std::mutex mutex_;
  static int32_t count_;
};
}  // namespace edgevox_onnx
#endif  // EDGEVOX_ONNX_CSRC_AXCL_AXCL_MANAGER_H_
