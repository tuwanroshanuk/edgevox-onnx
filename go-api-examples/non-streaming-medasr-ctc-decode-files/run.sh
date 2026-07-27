#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
  tar xvf edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
  rm edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
fi

go mod tidy
go build
./non-streaming-medasr-ctc-decode-files
