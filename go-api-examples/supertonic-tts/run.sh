#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/duration_predictor.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
  tar xvf edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
  rm edgevox-onnx-supertonic-3-tts-int8-2026-05-11.tar.bz2
fi

go mod tidy
go build

./supertonic-tts
