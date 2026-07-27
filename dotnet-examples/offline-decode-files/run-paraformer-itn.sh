#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-paraformer-zh-2023-09-14 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  tar xvf edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
  rm edgevox-onnx-paraformer-zh-2023-09-14.tar.bz2
fi

if [ ! -f ./itn-zh-number.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn-zh-number.wav
fi

if [ ! -f ./itn_zh_number.fst ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn_zh_number.fst
fi

dotnet run \
  --tokens=./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt \
  --paraformer=./edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx \
  --rule-fsts=./itn_zh_number.fst \
  --num-threads=2 \
  --files ./itn-zh-number.wav
