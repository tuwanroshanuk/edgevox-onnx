#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
  tar xvf edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
  rm edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --telespeech-ctc ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/model.int8.onnx \
  --tokens ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/tokens.txt \
  --model-type telespeech_ctc \
  --debug 0 \
  ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/test_wavs/3-sichuan.wav
