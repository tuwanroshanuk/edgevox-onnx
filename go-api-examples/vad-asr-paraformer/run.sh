#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./silero_vad.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

if [ ! -f ./edgevox-onnx-paraformer-trilingual-zh-cantonese-en/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-trilingual-zh-cantonese-en.tar.bz2
  tar xvf edgevox-onnx-paraformer-trilingual-zh-cantonese-en.tar.bz2
  rm edgevox-onnx-paraformer-trilingual-zh-cantonese-en.tar.bz2
fi

go mod tidy
go build
./vad-asr-paraformer
