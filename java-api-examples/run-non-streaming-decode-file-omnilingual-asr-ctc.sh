#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  tar xvf edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  rm edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
fi


java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileOmnilingualAsrCtc.java
