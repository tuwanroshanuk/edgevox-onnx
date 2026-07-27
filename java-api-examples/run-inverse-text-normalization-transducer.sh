#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  rm edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
fi

if [ ! -f ./itn-zh-number.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn-zh-number.wav
fi

if [ ! -f ./itn_zh_number.fst ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn_zh_number.fst
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  InverseTextNormalizationStreamingTransducer.java
