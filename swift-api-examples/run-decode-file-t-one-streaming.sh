#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -d ./edgevox-onnx-streaming-t-one-russian-2025-09-08 ]; then
  echo "Downloading the pre-trained model for testing."

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  tar xvf edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  rm edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
fi

if [ ! -e ./decode-file-t-one-streaming ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./decode-file-t-one-streaming.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o decode-file-t-one-streaming

  strip decode-file-t-one-streaming
else
  echo "./decode-file-t-one-streaming exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./decode-file-t-one-streaming
