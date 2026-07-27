#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/HLG.fst ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
  rm edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
fi

go mod tidy
go build
ls -lh
./streaming-hlg-decoding
