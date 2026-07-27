#!/usr/bin/env bash
set -ex

# see
# https://k2-fsa.github.io/sherpa/onnx/sense-voice/pretrained.html#edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17-int8-chinese-english-japanese-korean-cantonese
if [ ! -f ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/model.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2

  tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
  rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
  ls -lh edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17
fi

cargo run --example sense_voice -- \
    --wav ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/test_wavs/en.wav \
    --model ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/model.int8.onnx \
    --tokens ./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/tokens.txt \
    --num-threads 2 \
    --debug
