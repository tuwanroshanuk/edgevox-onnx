#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17  ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --sense-voice-model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.int8.onnx \
  --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt \
  --debug 0 \
  ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/test_wavs/zh.wav
