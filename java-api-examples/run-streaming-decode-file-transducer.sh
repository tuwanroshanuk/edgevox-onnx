#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  rm edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  StreamingDecodeFileTransducer.java
