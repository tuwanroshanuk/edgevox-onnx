#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  tar xvf edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  rm edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
fi

go mod tidy
go build

./non-streaming-funasr-nano-decode-files
