#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
  rm edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
fi

dart run \
  ./bin/zipformer-ctc.dart \
  --model ./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/ctc-epoch-30-avg-3-chunk-16-left-128.int8.onnx \
  --tokens ./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/tokens.txt \
  --input-wav ./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/test_wavs/1.wav
