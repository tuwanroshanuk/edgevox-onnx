#!/usr/bin/env bash
set -ex

# see
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#csukuangfj-edgevox-onnx-streaming-zipformer-en-2023-06-21-english
if [ ! -f ./edgevox-onnx-streaming-zipformer-en-2023-06-21/encoder-epoch-99-avg-1.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-en-2023-06-21.tar.bz2

  tar xvf edgevox-onnx-streaming-zipformer-en-2023-06-21.tar.bz2
  rm edgevox-onnx-streaming-zipformer-en-2023-06-21.tar.bz2
  ls -lh edgevox-onnx-streaming-zipformer-en-2023-06-21
fi

cargo run --example streaming_zipformer -- \
    --wav edgevox-onnx-streaming-zipformer-en-2023-06-21/test_wavs/1.wav \
    --encoder edgevox-onnx-streaming-zipformer-en-2023-06-21/encoder-epoch-99-avg-1.int8.onnx \
    --decoder edgevox-onnx-streaming-zipformer-en-2023-06-21/decoder-epoch-99-avg-1.onnx \
    --joiner edgevox-onnx-streaming-zipformer-en-2023-06-21/joiner-epoch-99-avg-1.int8.onnx \
    --tokens edgevox-onnx-streaming-zipformer-en-2023-06-21/tokens.txt \
    --provider cpu \
    --debug
