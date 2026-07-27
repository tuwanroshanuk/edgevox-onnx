#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  tar xvf edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
  rm edgevox-onnx-funasr-nano-int8-2025-12-30.tar.bz2
fi

dart run \
  ./bin/funasr-nano.dart \
  --encoder-adaptor ./edgevox-onnx-funasr-nano-int8-2025-12-30/encoder_adaptor.int8.onnx \
  --llm ./edgevox-onnx-funasr-nano-int8-2025-12-30/llm.int8.onnx \
  --embedding ./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx \
  --tokenizer ./edgevox-onnx-funasr-nano-int8-2025-12-30/Qwen3-0.6B \
  --input-wav ./edgevox-onnx-funasr-nano-int8-2025-12-30/test_wavs/lyrics.wav
