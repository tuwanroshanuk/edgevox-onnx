#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  rm edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
fi

dart run \
  ./bin/fire-red-asr-ctc.dart \
  --model ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx \
  --tokens ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt \
  --input-wav ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/test_wavs/1.wav
