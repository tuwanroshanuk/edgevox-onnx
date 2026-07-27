#!/usr/bin/env bash

set -ex

source ./setup.sh

# please visit
# https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models
# to download more models

if [ ! -f ./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/duration_predictor.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
  tar xvf edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
  rm edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  SupertonicTts.java
