#!/usr/bin/env bash
set -ex

# https://k2-fsa.github.io/sherpa/onnx/vad/silero-vad.html
if [ ! -f "./silero_vad.onnx" ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

# see
# https://k2-fsa.github.io/sherpa/onnx/FireRedAsr/pretrained.html
if [ ! -f ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2

  tar xvf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  rm edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  ls -lh edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25
fi

cargo run --example fire_red_asr_ctc_simulate_streaming_microphone --features mic -- \
    --silero-vad-model ./silero_vad.onnx \
    --model ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx \
    --tokens ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt
