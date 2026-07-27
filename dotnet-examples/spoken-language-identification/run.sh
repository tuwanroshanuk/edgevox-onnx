#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-whisper-tiny ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.tar.bz2
  rm edgevox-onnx-whisper-tiny.tar.bz2
fi

dotnet run

