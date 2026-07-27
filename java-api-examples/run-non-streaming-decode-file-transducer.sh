#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-zipformer-gigaspeech-2023-12-12/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-gigaspeech-2023-12-12.tar.bz2

  tar xvf edgevox-onnx-zipformer-gigaspeech-2023-12-12.tar.bz2
  rm edgevox-onnx-zipformer-gigaspeech-2023-12-12.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileTransducer.java
