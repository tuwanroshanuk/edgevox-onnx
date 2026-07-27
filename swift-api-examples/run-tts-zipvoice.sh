#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

# please visit
# https://k2-fsa.github.io/sherpa/onnx/tts/zipvoice.html
# to download more models
if [ ! -f ./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
  tar xf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
  rm edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
fi

if [ ! -f ./vocos_24khz.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos_24khz.onnx
fi

if [ ! -e ./tts-zipvoice ]; then
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./tts-zipvoice.swift ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o tts-zipvoice

  strip tts-zipvoice
else
  echo "./tts-zipvoice exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./tts-zipvoice
