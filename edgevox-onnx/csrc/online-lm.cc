// edgevox-onnx/csrc/online-lm.cc
//
// Copyright (c)  2023  Pingfeng Luo
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/csrc/online-lm.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "edgevox-onnx/csrc/online-rnn-lm.h"

namespace edgevox_onnx {

std::unique_ptr<OnlineLM> OnlineLM::Create(const OnlineLMConfig &config) {
  return std::make_unique<OnlineRnnLM>(config);
}

}  // namespace edgevox_onnx
