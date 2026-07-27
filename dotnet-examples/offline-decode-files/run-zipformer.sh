#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-zipformer-en-2023-04-01 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
  tar xvf edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
  rm edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
fi

dotnet run \
  --tokens=./edgevox-onnx-zipformer-en-2023-04-01/tokens.txt \
  --encoder=./edgevox-onnx-zipformer-en-2023-04-01/encoder-epoch-99-avg-1.onnx \
  --decoder=./edgevox-onnx-zipformer-en-2023-04-01/decoder-epoch-99-avg-1.onnx \
  --joiner=./edgevox-onnx-zipformer-en-2023-04-01/joiner-epoch-99-avg-1.onnx \
  --num-threads=2 \
  --decoding-method=modified_beam_search \
  --files ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/0.wav \
  ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/1.wav \
  ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/8k.wav
