#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2

  tar xvf edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  rm edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
fi

dart run \
  ./bin/paraformer.dart \
  --model ./edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx \
  --tokens ./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt \
  --input-wav ./edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/3-sichuan.wav
