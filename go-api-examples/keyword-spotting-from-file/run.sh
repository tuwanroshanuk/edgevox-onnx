#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/kws-models/edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  tar xvf edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  rm edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
fi

go mod tidy
go build
./keyword-spotting-from-file
