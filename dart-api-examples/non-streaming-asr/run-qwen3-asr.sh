#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  tar xvf edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  rm edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
fi

dart run \
  ./bin/qwen3-asr.dart \
  --conv-frontend ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/conv_frontend.onnx \
  --encoder ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx \
  --decoder ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/decoder.int8.onnx \
  --tokenizer ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/tokenizer \
  --input-wav ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/test_wavs/raokouling.wav
