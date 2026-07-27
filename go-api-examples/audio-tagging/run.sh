#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-zipformer-small-audio-tagging-2024-04-15/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2

  tar xvf edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2
  rm edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2
fi

go mod tidy
go build

./audio-tagging
