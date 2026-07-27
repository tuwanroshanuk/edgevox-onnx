#!/usr/bin/env bash
set -ex

if [ ! -f ./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  tar xvf edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  rm edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
fi

dotnet run
