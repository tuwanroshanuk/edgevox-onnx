#!/usr/bin/env bash
set -ex

# https://k2-fsa.github.io/sherpa/onnx/vad/silero-vad.html
if [ ! -f "./silero_vad.onnx" ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

if [ ! -f ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2

  tar xvf edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  rm edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25.tar.bz2
  ls -lh edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25
fi

cargo run --example qwen3_asr_simulate_streaming_microphone --features mic -- \
    --silero-vad-model ./silero_vad.onnx \
    --conv-frontend ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/conv_frontend.onnx \
    --encoder ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx \
    --decoder ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/decoder.int8.onnx \
    --tokenizer ./edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/tokenizer
