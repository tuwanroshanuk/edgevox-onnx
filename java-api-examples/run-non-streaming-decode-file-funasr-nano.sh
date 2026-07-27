#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  tar xvf edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  rm edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileFunAsrNano.java
