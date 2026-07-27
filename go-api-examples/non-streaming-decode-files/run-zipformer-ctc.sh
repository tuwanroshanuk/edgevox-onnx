#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2

  tar xvf edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  rm edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --zipformer-ctc ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/model.int8.onnx \
  --tokens ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt \
  --debug 0 \
  ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/test_wavs/0.wav
