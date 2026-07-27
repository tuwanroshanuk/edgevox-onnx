#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/kws-models/edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  tar xvf edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  rm edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./KeywordSpotterFromFile.java
