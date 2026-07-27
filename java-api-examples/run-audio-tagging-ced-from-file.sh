#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  tar xvf edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  rm edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./AudioTaggingCEDFromFile.java
