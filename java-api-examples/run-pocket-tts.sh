#!/usr/bin/env bash

set -ex

source ./setup.sh

# please visit
# https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models
# to download more models

if [ ! -f ./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  tar xvf edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  rm edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
fi

if false; then
  javac \
    -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
    PocketTts.java
  javap -p -s PocketTts.class
  javap -p -s PocketTts$1.class
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  PocketTts.java
