#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

if [ ! -f ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/model.int8.onnx ]; then
  echo "Please download the pre-trained model for testing."
  echo "You can refer to"
  echo ""
  echo "https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-ctc/icefall/zipformer.html#edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03-chinese"
  echo ""
  echo "for help"

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2

  tar xvf edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  rm edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  ls -lh edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03
fi

if [ ! -e ./zipformer-ctc-asr ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./zipformer-ctc-asr.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o zipformer-ctc-asr

  strip zipformer-ctc-asr
else
  echo "./zipformer-ctc-asr exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./zipformer-ctc-asr
