#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
  tar xvf edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
  rm edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --moonshine-preprocessor=./edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx \
  --moonshine-encoder=./edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx \
  --moonshine-uncached-decoder=./edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx \
  --moonshine-cached-decoder=./edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx \
  --tokens=./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt \
  ./edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav

