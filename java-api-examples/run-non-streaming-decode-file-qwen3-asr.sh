#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  tar xvf edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  rm edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileQwen3Asr.java
