#!/usr/bin/env bash

set -ex

dart pub get

if [[ ! -f ./edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12/model.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  tar xvf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
  rm edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12.tar.bz2
fi

dart run \
  ./bin/punctuations.dart \
  --model ./edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12/model.onnx
