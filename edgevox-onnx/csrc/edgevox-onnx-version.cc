// edgevox-onnx/csrc/edgevox-onnx-version.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include <stdio.h>

#include <cstdint>

#include "edgevox-onnx/csrc/version.h"

int32_t main() {
  printf("edgevox-onnx version : %s\n", edgevox_onnx::GetVersionStr());
  printf("edgevox-onnx Git SHA1: %s\n", edgevox_onnx::GetGitSha1());
  printf("edgevox-onnx Git date: %s\n", edgevox_onnx::GetGitDate());

  return 0;
}
