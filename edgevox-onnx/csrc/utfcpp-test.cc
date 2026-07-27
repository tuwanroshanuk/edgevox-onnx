// edgevox-onnx/csrc/utfcpp-test.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "edgevox-onnx/csrc/text-utils.h"

namespace edgevox_onnx {

TEST(UTF8, Case1) {
  std::string hello = "你好, 早上好！世界.  hello!。Hallo! how are you?";
  std::vector<std::string> ss = SplitUtf8(hello);
  for (const auto &s : ss) {
    std::cout << s << "\n";
  }
}

}  // namespace edgevox_onnx
