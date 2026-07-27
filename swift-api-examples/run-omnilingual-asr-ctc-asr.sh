#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  tar xvf edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  rm edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
fi

if [ ! -e ./omnilingual-asr-ctc ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./omnilingual-asr-ctc.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o omnilingual-asr-ctc

  strip omnilingual-asr-ctc
else
  echo "./omnilingual-asr-ctc exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./omnilingual-asr-ctc
