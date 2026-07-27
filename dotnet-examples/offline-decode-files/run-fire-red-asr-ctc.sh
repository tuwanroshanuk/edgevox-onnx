#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  tar xvf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  rm edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2

  ls -lh edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25
fi

dotnet run \
  --num-threads=2 \
  --fire-red-asr-ctc=./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx \
  --tokens=./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt \
  --files ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/test_wavs/1.wav
