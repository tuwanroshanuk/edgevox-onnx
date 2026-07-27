#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-zipformer-small-audio-tagging-2024-04-15/model.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2
  tar xvf edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2
  rm edgevox-onnx-zipformer-small-audio-tagging-2024-04-15.tar.bz2

  ls -lh edgevox-onnx-zipformer-small-audio-tagging-2024-04-15
fi

if [ ! -f ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  tar xvf edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  rm edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2

  ls -lh edgevox-onnx-ced-mini-audio-tagging-2024-04-19
fi

dotnet run
