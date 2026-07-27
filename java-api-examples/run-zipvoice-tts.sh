#!/usr/bin/env bash

set -ex

source ./setup.sh

# please visit
# https://k2-fsa.github.io/sherpa/onnx/tts/zipvoice.html
# to download more models
if [ ! -f ./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
  tar xf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
  rm edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
fi

if [ ! -f ./vocos_24khz.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos_24khz.onnx
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ZipVoiceTts.java
