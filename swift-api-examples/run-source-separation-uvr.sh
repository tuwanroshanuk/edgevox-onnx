#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f ./UVR-MDX-NET-Voc_FT.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/source-separation-models/UVR-MDX-NET-Voc_FT.onnx
fi

if [ ! -f ./qi-feng-le-zh.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/source-separation-models/qi-feng-le-zh.wav
fi

if [ ! -e ./source-separation-uvr ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./source-separation-uvr.swift ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o source-separation-uvr

  strip source-separation-uvr
else
  echo "./source-separation-uvr exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./source-separation-uvr
