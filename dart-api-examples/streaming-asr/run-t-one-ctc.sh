#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  tar xvf edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  rm edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
fi

dart run \
  ./bin/t-one-ctc.dart \
  --model ./edgevox-onnx-streaming-t-one-russian-2025-09-08/model.onnx \
  --tokens ./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt \
  --input-wav ./edgevox-onnx-streaming-t-one-russian-2025-09-08/0.wav
