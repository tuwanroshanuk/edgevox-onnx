#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-streaming-zipformer-en-2023-06-26 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-en-2023-06-26.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-en-2023-06-26.tar.bz2
  rm edgevox-onnx-streaming-zipformer-en-2023-06-26.tar.bz2
fi

go mod tidy
go build

./streaming-decode-files \
  --encoder ./edgevox-onnx-streaming-zipformer-en-2023-06-26/encoder-epoch-99-avg-1-chunk-16-left-128.onnx \
  --decoder ./edgevox-onnx-streaming-zipformer-en-2023-06-26/decoder-epoch-99-avg-1-chunk-16-left-128.onnx \
  --joiner ./edgevox-onnx-streaming-zipformer-en-2023-06-26/joiner-epoch-99-avg-1-chunk-16-left-128.onnx \
  --tokens ./edgevox-onnx-streaming-zipformer-en-2023-06-26/tokens.txt \
  --model-type zipformer2 \
  --debug 0 \
  ./edgevox-onnx-streaming-zipformer-en-2023-06-26/test_wavs/0.wav
