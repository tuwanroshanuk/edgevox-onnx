#!/usr/bin/env bash
set -ex

if [ ! -f ./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  tar xvf edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
  rm edgevox-onnx-ced-mini-audio-tagging-2024-04-19.tar.bz2
fi

cargo run --example audio_tagging_ced
