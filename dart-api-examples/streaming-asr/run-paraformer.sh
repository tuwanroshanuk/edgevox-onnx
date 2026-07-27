#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  tar xvf edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  rm edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
fi

dart run \
  ./bin/paraformer.dart \
  --encoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/encoder.int8.onnx \
  --decoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/decoder.int8.onnx \
  --tokens ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt \
  --input-wav ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/0.wav
