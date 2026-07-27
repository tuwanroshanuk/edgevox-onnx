#!/usr/bin/env bash
set -ex

# see
# https://k2-fsa.github.io/sherpa/onnx/moonshine
if [ ! -f ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/encoder_model.ort ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
  tar xvf edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
  rm edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
fi

cargo run --example moonshine_v2 -- \
    --wav ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/test_wavs/0.wav \
    --encoder ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/encoder_model.ort \
    --decoder ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/decoder_model_merged.ort \
    --tokens ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/tokens.txt \
    --num-threads 2
