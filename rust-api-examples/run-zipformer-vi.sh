#!/usr/bin/env bash
set -ex

# see also
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/offline-transducer/zipformer-transducer-models.html#edgevox-onnx-zipformer-vi-30m-int8-2026-02-09-vietnamese
if [ ! -f "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/encoder.int8.onnx" ]; then
    curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-zipformer-vi-30M-int8-2026-02-09.tar.bz2
    tar xvf edgevox-onnx-zipformer-vi-30M-int8-2026-02-09.tar.bz2
    rm edgevox-onnx-zipformer-vi-30M-int8-2026-02-09.tar.bz2
    ls -lh edgevox-onnx-zipformer-vi-30M-int8-2026-02-09
fi

# Run Zipformer transducer
cargo run --example zipformer -- \
    --wav "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/test_wavs/0.wav" \
    --encoder "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/encoder.int8.onnx" \
    --decoder "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/decoder.onnx" \
    --joiner "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/joiner.int8.onnx" \
    --tokens "./edgevox-onnx-zipformer-vi-30M-int8-2026-02-09/tokens.txt" \
    --provider cpu \
    --num-threads 2 \
    --debug
