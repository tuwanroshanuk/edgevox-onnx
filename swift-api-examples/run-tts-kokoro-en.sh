#!/usr/bin/env bash

set -ex

if [ ! -d ../build-swift-macos ]; then
  echo "Please run ../build-swift-macos.sh first!"
  exit 1
fi

# please visit
# https://k2-fsa.github.io/sherpa/onnx/tts/pretrained_models/kokoro.html
# to download more models
if [ ! -f ./kokoro-en-v0_19/model.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/kokoro-en-v0_19.tar.bz2
  tar xf kokoro-en-v0_19.tar.bz2
  rm kokoro-en-v0_19.tar.bz2
fi

if [ ! -e ./tts-kokoro-en ]; then
  # Note: We use -lc++ to link against libc++ instead of libstdc++
  swiftc \
    -lc++ \
    -I ../build-swift-macos/install/include \
    -import-objc-header ./EdgevoxOnnx-Bridging-Header.h \
    ./tts-kokoro-en.swift  ./EdgevoxOnnx.swift \
    -L ../build-swift-macos/install/lib/ \
    -l edgevox-onnx \
    -l onnxruntime \
    -o tts-kokoro-en

  strip tts-kokoro-en
else
  echo "./tts-kokoro-en exists - skip building"
fi

export DYLD_LIBRARY_PATH=$PWD/../build-swift-macos/install/lib:$DYLD_LIBRARY_PATH
./tts-kokoro-en
