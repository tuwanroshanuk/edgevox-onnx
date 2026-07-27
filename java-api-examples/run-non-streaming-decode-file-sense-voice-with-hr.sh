#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
fi

if [ ! -d dict ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/dict.tar.bz2
  tar xf dict.tar.bz2
  rm dict.tar.bz2

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/replace.fst
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/test-hr.wav
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/lexicon.txt
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileSenseVoiceWithHr.java
