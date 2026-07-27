#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./silero_vad.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

if [ ! -f ./edgevox-onnx-whisper-tiny/tiny-encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.tar.bz2
  rm edgevox-onnx-whisper-tiny.tar.bz2
fi

go mod tidy
go build
./vad-spoken-language-identification
