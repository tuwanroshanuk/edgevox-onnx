#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  rm edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16.tar.bz2
  ls -lh edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16
fi

dart pub get

dart run \
  ./bin/fire-red-asr.dart \
  --encoder ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx \
  --decoder ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/decoder.int8.onnx \
  --tokens ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/tokens.txt \
  --input-wav ./edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/test_wavs/0.wav
