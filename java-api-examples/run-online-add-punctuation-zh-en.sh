#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-online-punct-en-2024-08-06/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  tar xvf edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  rm edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./OnlineAddPunctuation.java
