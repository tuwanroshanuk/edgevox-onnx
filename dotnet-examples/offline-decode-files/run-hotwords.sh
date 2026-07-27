#!/usr/bin/env bash

set -ex

if [ ! -d ./edgevox-onnx-zipformer-en-2023-04-01 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
  tar xvf edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
  rm edgevox-onnx-zipformer-en-2023-04-01.tar.bz2
fi

if [ ! -f ./edgevox-onnx-zipformer-en-2023-04-01/hotwords_en.txt ]; then
cat >./edgevox-onnx-zipformer-en-2023-04-01/hotwords_en.txt <<EOF
▁ QUA R TER S
▁FOR E VER
EOF
fi

dotnet run \
  --tokens=./edgevox-onnx-zipformer-en-2023-04-01/tokens.txt \
  --encoder=./edgevox-onnx-zipformer-en-2023-04-01/encoder-epoch-99-avg-1.onnx \
  --decoder=./edgevox-onnx-zipformer-en-2023-04-01/decoder-epoch-99-avg-1.onnx \
  --joiner=./edgevox-onnx-zipformer-en-2023-04-01/joiner-epoch-99-avg-1.onnx \
  --num-threads=2 \
  --decoding-method=modified_beam_search \
  --files ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/0.wav \
  ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/1.wav

dotnet run \
  --hotwords-file=./edgevox-onnx-zipformer-en-2023-04-01/hotwords_en.txt \
  --hotwords-score=2.0 \
  --tokens=./edgevox-onnx-zipformer-en-2023-04-01/tokens.txt \
  --encoder=./edgevox-onnx-zipformer-en-2023-04-01/encoder-epoch-99-avg-1.onnx \
  --decoder=./edgevox-onnx-zipformer-en-2023-04-01/decoder-epoch-99-avg-1.onnx \
  --joiner=./edgevox-onnx-zipformer-en-2023-04-01/joiner-epoch-99-avg-1.onnx \
  --num-threads=2 \
  --decoding-method=modified_beam_search \
  --files ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/0.wav \
  ./edgevox-onnx-zipformer-en-2023-04-01/test_wavs/1.wav

# 0.wav: QUARTER -> QUARTERS
# 1.wav: FOR EVER -> FOREVER
