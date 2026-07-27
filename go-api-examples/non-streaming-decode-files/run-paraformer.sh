#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-paraformer-zh-2023-09-14 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  tar xvf edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  rm edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --paraformer ./edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx \
  --tokens ./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt \
  --model-type paraformer \
  --debug 0 \
  ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/0.wav
