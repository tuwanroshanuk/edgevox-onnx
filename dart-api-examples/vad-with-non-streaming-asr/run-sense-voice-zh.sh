#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
fi

if [ ! -f ./lei-jun-test.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/lei-jun-test.wav
fi

if [[ ! -f ./silero_vad.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

dart run \
  ./bin/sense-voice.dart \
  --silero-vad ./silero_vad.onnx \
  --model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.onnx \
  --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt \
  --use-itn true \
  --input-wav ./lei-jun-test.wav

