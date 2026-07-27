#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.en.tar.bz2

  tar xvf edgevox-onnx-whisper-tiny.en.tar.bz2
  rm edgevox-onnx-whisper-tiny.en.tar.bz2
fi



if [ ! -f ./Obama.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/Obama.wav
fi

if [[ ! -f ./silero_vad.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

dart run \
  ./bin/whisper.dart \
  --silero-vad ./silero_vad.onnx \
  --encoder ./edgevox-onnx-whisper-tiny.en/tiny.en-encoder.int8.onnx \
  --decoder ./edgevox-onnx-whisper-tiny.en/tiny.en-decoder.int8.onnx \
  --tokens ./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt \
  --input-wav ./Obama.wav
