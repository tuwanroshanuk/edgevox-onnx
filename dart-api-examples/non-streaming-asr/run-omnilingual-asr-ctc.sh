#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  tar xvf edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
  rm edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12.tar.bz2
fi

dart run \
  ./bin/omnilingual-asr-ctc.dart \
  --model ./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/model.int8.onnx \
  --tokens ./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt \
  --input-wav ./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/test_wavs/en.wav
