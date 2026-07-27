#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  tar xvf edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  rm edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  StreamingDecodeFileToneCtc.java
