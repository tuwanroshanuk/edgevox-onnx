#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  tar xvf edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  rm edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  ls -lh edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02
fi

if [ ! -f ./lei-jun-test.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/lei-jun-test.wav
fi

if [[ ! -f ./silero_vad.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

dart run \
  ./bin/dolphin-ctc.dart \
  --silero-vad ./silero_vad.onnx \
  --model ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx \
  --tokens ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/tokens.txt \
  --input-wav ./lei-jun-test.wav
