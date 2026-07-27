#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
fi

dart run \
  ./bin/sense-voice.dart \
  --model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.int8.onnx \
  --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt \
  --use-itn true \
  --input-wav ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/test_wavs/zh.wav
