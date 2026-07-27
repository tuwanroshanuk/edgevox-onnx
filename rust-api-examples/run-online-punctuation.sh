#!/usr/bin/env bash
set -ex

if [ ! -d ./edgevox-onnx-online-punct-en-2024-08-06 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  tar xvf edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  rm edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
fi

cargo run --example online_punctuation -- \
  --cnn-bilstm ./edgevox-onnx-online-punct-en-2024-08-06/model.onnx \
  --bpe-vocab ./edgevox-onnx-online-punct-en-2024-08-06/bpe.vocab
