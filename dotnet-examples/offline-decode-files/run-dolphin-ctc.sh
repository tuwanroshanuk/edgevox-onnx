#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  tar xvf edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  rm edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  ls -lh edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02
fi

dotnet run \
  --tokens=./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/tokens.txt \
  --dolphin-model=./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx \
  --num-threads=1 \
  --files ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/test_wavs/0.wav
