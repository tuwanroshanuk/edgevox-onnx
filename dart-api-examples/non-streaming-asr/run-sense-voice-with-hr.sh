#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17.tar.bz2
fi

if [ ! -d dict ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/dict.tar.bz2
  tar xf dict.tar.bz2
  rm dict.tar.bz2

  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/replace.fst
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/test-hr.wav
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/lexicon.txt
fi

dart run \
  ./bin/sense-voice-with-hr.dart \
  --model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.int8.onnx \
  --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt \
  --use-itn true \
  --hr-lexicon ./lexicon.txt \
  --hr-rule-fsts ./replace.fst \
  --input-wav ./test-hr.wav
