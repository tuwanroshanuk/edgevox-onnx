#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
  rm edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
fi

go mod tidy
go build

./streaming-decode-files \
  --zipformer2-ctc ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/ctc-epoch-20-avg-1-chunk-16-left-128.onnx \
  --tokens ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/tokens.txt \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/DEV_T0000000000.wav
