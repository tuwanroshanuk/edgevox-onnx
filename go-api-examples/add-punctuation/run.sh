#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d ./edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  tar xvf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  rm edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
fi

go mod tidy
go build

./add-punctuation
