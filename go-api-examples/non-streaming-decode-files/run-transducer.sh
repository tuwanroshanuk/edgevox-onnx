#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-streaming-zipformer-en-2023-06-26 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-en-2023-06-26.tar.bz2
  tar xvf edgevox-onnx-zipformer-en-2023-06-26.tar.bz2
  rm edgevox-onnx-zipformer-en-2023-06-26.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --encoder ./edgevox-onnx-zipformer-en-2023-06-26/encoder-epoch-99-avg-1.onnx \
  --decoder ./edgevox-onnx-zipformer-en-2023-06-26/decoder-epoch-99-avg-1.onnx \
  --joiner ./edgevox-onnx-zipformer-en-2023-06-26/joiner-epoch-99-avg-1.onnx \
  --tokens ./edgevox-onnx-zipformer-en-2023-06-26/tokens.txt \
  --model-type transducer \
  --debug 0 \
  ./edgevox-onnx-zipformer-en-2023-06-26/test_wavs/0.wav
