#!/usr/bin/env bash

set -ex

dart pub get

if [[ ! -f ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  tar xvf edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  rm edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
fi

for w in 1 2 3 4 5 6; do
  dart run \
    ./bin/ced.dart \
    --model ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.int8.onnx \
    --labels ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/class_labels_indices.csv \
    --wav ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/$w.wav
done
