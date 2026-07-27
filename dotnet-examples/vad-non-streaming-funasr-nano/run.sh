#!/usr/bin/env bash

set -ex

if [ ! -f ./silero_vad.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

if [ ! -f ./lei-jun-test.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/lei-jun-test.wav
fi

if [ ! -f ./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  tar xvf edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  rm edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
fi

dotnet run
