#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
  tar xvf edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
  rm edgevox-onnx-medasr-ctc-en-int8-2025-12-25.tar.bz2
fi

dotnet run \
  --medasr=./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/model.int8.onnx \
  --tokens=./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/tokens.txt \
  --files ./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/test_wavs/0.wav
