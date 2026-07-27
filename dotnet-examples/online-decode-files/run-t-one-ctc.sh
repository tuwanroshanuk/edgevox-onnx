#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  tar xvf edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
  rm edgevox-onnx-streaming-t-one-russian-2025-09-08.tar.bz2
fi

dotnet run -c Release \
  --tokens ./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt \
  --t-one-ctc ./edgevox-onnx-streaming-t-one-russian-2025-09-08/model.onnx \
  --files ./edgevox-onnx-streaming-t-one-russian-2025-09-08/0.wav
