#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./silero_vad.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

if [ ! -f ./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2

  tar xvf edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  rm edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
fi

if [ ! -f ./itn_zh_number.fst ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn_zh_number.fst
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./VadFromMicWithNonStreamingParaformer.java
