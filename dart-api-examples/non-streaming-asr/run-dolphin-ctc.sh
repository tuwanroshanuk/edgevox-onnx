#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  tar xvf edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  rm edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  ls -lh edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02
fi

dart run \
  ./bin/dolphin-ctc.dart \
  --model ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx \
  --tokens ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/tokens.txt \
  --input-wav ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/test_wavs/0.wav
