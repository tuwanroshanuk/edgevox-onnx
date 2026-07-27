#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  rm edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  ls -lh edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileFireRedAsr.java
