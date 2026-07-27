#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-nemo-ctc-en-conformer-medium ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
  tar xvf edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
  rm edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
fi

dotnet run \
  --tokens=./edgevox-onnx-nemo-ctc-en-conformer-medium/tokens.txt \
  --nemo-ctc=./edgevox-onnx-nemo-ctc-en-conformer-medium/model.onnx \
  --num-threads=1 \
  --files ./edgevox-onnx-nemo-ctc-en-conformer-medium/test_wavs/0.wav \
  ./edgevox-onnx-nemo-ctc-en-conformer-medium/test_wavs/1.wav \
  ./edgevox-onnx-nemo-ctc-en-conformer-medium/test_wavs/8k.wav
