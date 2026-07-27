// Copyright 2025 Xiaomi Corporation

package com.k2fsa.sherpa.onnx.example;

import com.k2fsa.sherpa.onnx.*;

public class VersionTest {
  public static void main(String[] args) {
    System.out.printf("edgevox-onnx version: %s\n", VersionInfo.getVersion());
    System.out.printf("edgevox-onnx gitSha1: %s\n", VersionInfo.getGitSha1());
    System.out.printf("edgevox-onnx gitDate: %s\n", VersionInfo.getGitDate());
  }
}
