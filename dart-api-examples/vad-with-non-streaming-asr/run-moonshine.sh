#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
  tar xvf edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
  rm edgevox-onnx-moonshine-tiny-en-int8.tar.bz2
fi

if [ ! -f ./Obama.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/Obama.wav
fi

if [[ ! -f ./silero_vad.onnx ]]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

dart run \
  ./bin/moonshine.dart \
  --silero-vad ./silero_vad.onnx \
  --preprocessor ./edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx \
  --encoder ./edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx \
  --uncached-decoder ./edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx \
  --cached-decoder ./edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx \
  --tokens ./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt \
  --input-wav ./Obama.wav
