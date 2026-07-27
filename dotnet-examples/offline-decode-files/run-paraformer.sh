#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-paraformer-zh-2023-09-14 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  tar xvf edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  rm edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
fi

dotnet run \
  --tokens=./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt \
  --paraformer=./edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx \
  --num-threads=2 \
  --files ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/0.wav \
  ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/1.wav \
  ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/2.wav \
  ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/8k.wav
