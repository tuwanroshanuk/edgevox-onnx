#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12/model.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  tar xvf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  rm edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./OfflineAddPunctuation.java
