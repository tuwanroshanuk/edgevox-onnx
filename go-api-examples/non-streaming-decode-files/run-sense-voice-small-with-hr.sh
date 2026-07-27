#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17  ]; then
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

go mod tidy
go build

./non-streaming-decode-files \
  --sense-voice-model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.int8.onnx \
  --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt \
  --debug 1 \
  --hr-lexicon ./lexicon.txt \
  --hr-rule-fsts ./replace.fst \
  ./test-hr.wav
