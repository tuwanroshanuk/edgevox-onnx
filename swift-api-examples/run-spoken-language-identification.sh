#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -d ./edgevox-onnx-whisper-tiny ]; then
  echo "Download a pre-trained model for testing."

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.tar.bz2
  rm edgevox-onnx-whisper-tiny.tar.bz2
fi

if [ ! -e ./spoken-language-identification ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./spoken-language-identification.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o spoken-language-identification

  strip spoken-language-identification
else
  echo "./spoken-language-identification exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./spoken-language-identification
