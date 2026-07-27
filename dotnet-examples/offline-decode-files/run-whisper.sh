#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-whisper-tiny.en ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.en.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.en.tar.bz2
  rm edgevox-onnx-whisper-tiny.en.tar.bz2
fi

dotnet run \
  --num-threads=2 \
  --whisper-encoder=./edgevox-onnx-whisper-tiny.en/tiny.en-encoder.onnx \
  --whisper-decoder=./edgevox-onnx-whisper-tiny.en/tiny.en-decoder.onnx \
  --tokens=./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt \
  --files ./edgevox-onnx-whisper-tiny.en/test_wavs/0.wav \
  ./edgevox-onnx-whisper-tiny.en/test_wavs/1.wav \
  ./edgevox-onnx-whisper-tiny.en/test_wavs/8k.wav
