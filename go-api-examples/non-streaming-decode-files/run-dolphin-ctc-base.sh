#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -f ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  tar xvf edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  rm edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  ls -lh edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02
fi

go mod tidy
go build

./non-streaming-decode-files \
  --dolphin-model ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx \
  --tokens ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/tokens.txt \
  --debug 0 \
  ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/test_wavs/0.wav
