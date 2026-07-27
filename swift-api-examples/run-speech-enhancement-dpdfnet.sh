#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f ./dpdfnet_baseline.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speech-enhancement-models/dpdfnet_baseline.onnx
fi

if [ ! -f ./inp_16k.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speech-enhancement-models/inp_16k.wav
fi

if [ ! -e ./speech-enhancement-dpdfnet ]; then
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./speech-enhancement-dpdfnet.swift ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o speech-enhancement-dpdfnet

  strip speech-enhancement-dpdfnet
else
  echo "./speech-enhancement-dpdfnet exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./speech-enhancement-dpdfnet
