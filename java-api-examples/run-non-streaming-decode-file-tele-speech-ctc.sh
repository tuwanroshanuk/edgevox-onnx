#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
  tar xvf edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
  rm edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./NonStreamingDecodeFileTeleSpeechCtc.java
