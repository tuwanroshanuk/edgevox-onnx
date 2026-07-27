#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d ./edgevox-onnx-streaming-paraformer-bilingual-zh-en ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  tar xvf edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  rm edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
fi

go mod tidy
go build

./streaming-decode-files \
  --paraformer-encoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/encoder.int8.onnx \
  --paraformer-decoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/decoder.int8.onnx \
  --tokens ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt \
  --decoding-method greedy_search \
  --model-type paraformer \
  --debug 0 \
  ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/0.wav
