#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  tar xvf edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  rm edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  StreamingDecodeFileParaformer.java
