#!/usr/bin/env bash
set -ex

# https://k2-fsa.github.io/sherpa/onnx/vad/silero-vad.html
if [ ! -f "./silero_vad.onnx" ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

# see
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/nemo-transducer-models.html#edgevox-onnx-nemo-parakeet-tdt-0-6b-v2-int8-english
if [ ! -f ./edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8/encoder.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8.tar.bz2

  tar xvf edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8.tar.bz2
  rm edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8.tar.bz2
  ls -lh edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8
fi

cargo run --example parakeet_tdt_simulate_streaming_microphone --features mic -- \
    --silero-vad-model ./silero_vad.onnx \
    --encoder ./edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8/encoder.int8.onnx \
    --decoder ./edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8/decoder.int8.onnx \
    --joiner ./edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8/joiner.int8.onnx \
    --tokens ./edgevox-onnx-nemo-parakeet-tdt-0.6b-v2-int8/tokens.txt
