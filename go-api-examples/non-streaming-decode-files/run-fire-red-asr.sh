#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  rm edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  ls -lh edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16
fi

go mod tidy
go build

./non-streaming-decode-files \
  --fire-red-asr-encoder=./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx \
  --fire-red-asr-decoder=./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/decoder.int8.onnx \
  --tokens=./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/tokens.txt \
  ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/test_wavs/0.wav

