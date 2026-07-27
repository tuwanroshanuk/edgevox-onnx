// edgevox-onnx/csrc/timer.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_TIMER_H_
#define EDGEVOX_ONNX_CSRC_TIMER_H_

#include <memory>

namespace edgevox_onnx {

class Timer {
 public:
  explicit Timer(bool debug = true);
  ~Timer();

  void Reset() const;

  // Return time in seconds
  double Elapsed() const;

  void Log(const char *tag) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_TIMER_H_
