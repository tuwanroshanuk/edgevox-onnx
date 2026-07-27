#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8.tar.bz2
  tar xvf edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8.tar.bz2
  rm edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8.tar.bz2
fi

go mod tidy
go build
./non-streaming-canary-decode-files
