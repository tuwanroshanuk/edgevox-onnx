#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  tar xvf edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  rm edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  ls -lh edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileCohereTranscribe.java
