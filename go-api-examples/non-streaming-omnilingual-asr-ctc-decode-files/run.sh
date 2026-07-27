#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  tar xvf edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  rm edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
fi

go mod tidy
go build
./non-streaming-omnilingual-asr-ctc-decode-files
