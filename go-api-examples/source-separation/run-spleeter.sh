#!/usr/bin/env bash
set -ex

export CGO_ENABLED=1

if [ ! -d ./edgevox-onnx-spleeter-2stems-fp16 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/source-separation-models/edgevox-onnx-spleeter-2stems-fp16.tar.bz2
  tar xjf edgevox-onnx-spleeter-2stems-fp16.tar.bz2
  rm edgevox-onnx-spleeter-2stems-fp16.tar.bz2
fi

if [ ! -f ./qi-feng-le-zh.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/source-separation-models/qi-feng-le-zh.wav
fi

go mod tidy
go build

./source-separation spleeter
