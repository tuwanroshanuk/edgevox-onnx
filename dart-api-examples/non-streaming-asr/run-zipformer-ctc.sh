#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2

  tar xvf edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
  rm edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03.tar.bz2
fi

dart run \
  ./bin/zipformer-ctc.dart \
  --model ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/model.int8.onnx \
  --tokens ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/tokens.txt \
  --input-wav ./edgevox-onnx-zipformer-ctc-zh-int8-2025-07-03/test_wavs/0.wav
