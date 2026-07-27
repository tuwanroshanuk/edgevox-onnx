#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f ./gtcrn_simple.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speech-enhancement-models/gtcrn_simple.onnx
fi

if [ ! -f ./inp_16k.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speech-enhancement-models/inp_16k.wav
fi

if [ ! -e ./online-speech-enhancement-gtcrn ]; then
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./online-speech-enhancement-gtcrn.swift ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o online-speech-enhancement-gtcrn

  strip online-speech-enhancement-gtcrn
else
  echo "./online-speech-enhancement-gtcrn exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./online-speech-enhancement-gtcrn
