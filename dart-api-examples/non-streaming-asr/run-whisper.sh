#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.en.tar.bz2

  tar xvf edgevox-onnx-whisper-tiny.en.tar.bz2
  rm edgevox-onnx-whisper-tiny.en.tar.bz2
fi

dart run \
  ./bin/whisper.dart \
  --encoder ./edgevox-onnx-whisper-tiny.en/tiny.en-encoder.int8.onnx \
  --decoder ./edgevox-onnx-whisper-tiny.en/tiny.en-decoder.int8.onnx \
  --tokens ./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt \
  --input-wav ./edgevox-onnx-whisper-tiny.en/test_wavs/0.wav
