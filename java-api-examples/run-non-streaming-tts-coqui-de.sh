#!/usr/bin/env bash

set -ex

source ./setup.sh

# please visit
# https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models
# to download more models
if [ ! -f ./vits-coqui-de-css10/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/vits-coqui-de-css10.tar.bz2
  tar xf vits-coqui-de-css10.tar.bz2
  rm vits-coqui-de-css10.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingTtsCoquiDe.java
