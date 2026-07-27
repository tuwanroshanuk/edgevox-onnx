#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-tdnn-yesno ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-tdnn-yesno.tar.bz2
  tar xvf edgevox-onnx-tdnn-yesno.tar.bz2
  rm edgevox-onnx-tdnn-yesno.tar.bz2
fi

dotnet run \
  --sample-rate=8000 \
  --feat-dim=23 \
  --tokens=./edgevox-onnx-tdnn-yesno/tokens.txt \
  --tdnn-model=./edgevox-onnx-tdnn-yesno/model-epoch-14-avg-2.onnx \
  --files ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_0_1_0_0_0_1.wav \
  ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_1_0_0_0_1_0.wav \
  ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_1_0_0_1_1_1.wav \
  ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_1_0_1_0_0_1.wav \
  ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_1_1_0_0_0_1.wav \
  ./edgevox-onnx-tdnn-yesno/test_wavs/0_0_1_1_0_1_1_0.wav
