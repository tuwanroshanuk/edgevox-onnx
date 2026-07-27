#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2

  tar xvf edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
  rm edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04.tar.bz2
fi

dart run \
  ./bin/telespeech-ctc.dart \
  --model ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/model.int8.onnx \
  --tokens ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/tokens.txt \
  --input-wav ./edgevox-onnx-telespeech-ctc-int8-zh-2024-06-04/test_wavs/3-sichuan.wav
