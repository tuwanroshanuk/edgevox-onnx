#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2

  tar xvf edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  rm edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
fi

dotnet run \
  --tokens=./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt \
  --zipformer-ctc=./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/model.int8.onnx \
  --num-threads=1 \
  --files ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/test_wavs/0.wav
