#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -d ./edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  tar xvf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  rm edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
fi

if [ ! -e ./add-punctuations ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./add-punctuations.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o ./add-punctuations

  strip ./add-punctuations
else
  echo "./add-punctuations exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./add-punctuations
