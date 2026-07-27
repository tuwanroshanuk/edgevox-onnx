#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2

  tar xvf edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  rm edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileZipformerCtc.java
