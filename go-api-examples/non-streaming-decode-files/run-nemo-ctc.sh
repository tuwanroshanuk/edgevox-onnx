#!/usr/bin/env bash

set -ex

export CGO_ENABLED=1

if [ ! -d edgevox-onnx-nemo-ctc-en-conformer-medium ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
  tar xvf edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
  rm edgevox-onnx-nemo-ctc-en-conformer-medium.tar.bz2
fi

go mod tidy
go build

./non-streaming-decode-files \
  --nemo-ctc ./edgevox-onnx-nemo-ctc-en-conformer-medium/model.onnx \
  --tokens ./edgevox-onnx-nemo-ctc-en-conformer-medium/tokens.txt \
  --model-type nemo_ctc \
  --debug 0 \
  ./edgevox-onnx-nemo-ctc-en-conformer-medium/test_wavs/0.wav
