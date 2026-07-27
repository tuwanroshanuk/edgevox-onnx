#!/usr/bin/env bash
set -ex

# see
# https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
if [ ! -f ./edgevox-onnx-whisper-tiny/tiny-encoder.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.tar.bz2
  rm edgevox-onnx-whisper-tiny.tar.bz2
fi

cargo run --example whisper -- \
    --wav ./edgevox-onnx-whisper-tiny/test_wavs/0.wav \
    --encoder ./edgevox-onnx-whisper-tiny/tiny-encoder.onnx \
    --decoder ./edgevox-onnx-whisper-tiny/tiny-decoder.onnx \
    --tokens ./edgevox-onnx-whisper-tiny/tiny-tokens.txt \
    --language en \
    --num-threads 2
