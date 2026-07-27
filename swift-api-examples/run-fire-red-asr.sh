#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx ]; then
  echo "Please download the pre-trained model for testing."
  echo "You can refer to"
  echo ""
  echo "https://k2-fsa.github.io/sherpa/onnx/FireRedAsr/index.html"
  echo ""
  echo "for help"

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  rm edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  ls -lh edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16
fi

if [ ! -e ./fire-red-asr ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./fire-red-asr.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o fire-red-asr

  strip fire-red-asr
else
  echo "./fire-red-asr exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./fire-red-asr
