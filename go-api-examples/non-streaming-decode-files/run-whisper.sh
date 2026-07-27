#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-whisper-tiny.en ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.en.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.en.tar.bz2
  rm edgevox-onnx-whisper-tiny.en.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --whisper-encoder=./edgevox-onnx-whisper-tiny.en/tiny.en-encoder.onnx \
  --whisper-decoder=./edgevox-onnx-whisper-tiny.en/tiny.en-decoder.onnx \
  --tokens=./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt \
  ./edgevox-onnx-whisper-tiny.en/test_wavs/0.wav

