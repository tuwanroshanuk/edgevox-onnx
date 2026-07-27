#!/usr/bin/env bash
set -ex

# https://k2-fsa.github.io/sherpa/onnx/vad/silero-vad.html
if [ ! -f "./silero_vad.onnx" ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
fi

# see
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01-japanese
if [ ! -f ./edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01/encoder-epoch-99-avg-1.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01.tar.bz2

  tar xvf edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01.tar.bz2
  rm edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01.tar.bz2
  ls -lh edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01
fi

cargo run --example zipformer_transducer_simulate_streaming_microphone --features mic -- \
    --silero-vad-model ./silero_vad.onnx \
    --encoder ./edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01/encoder-epoch-99-avg-1.int8.onnx \
    --decoder ./edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01/decoder-epoch-99-avg-1.onnx \
    --joiner ./edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01/joiner-epoch-99-avg-1.int8.onnx \
    --tokens ./edgevox-onnx-zipformer-ja-reazonspeech-2024-08-01/tokens.txt
