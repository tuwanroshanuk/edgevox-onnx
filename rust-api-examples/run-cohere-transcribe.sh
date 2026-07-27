#!/usr/bin/env bash
set -ex

if [ ! -f ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx ]; then
  curl -SsL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2

  tar xvf edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  rm edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  ls -lh edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01
fi

cargo run --example cohere_transcribe -- \
    --wav ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/test_wavs/en.wav \
    --encoder ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx \
    --decoder ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/decoder.int8.onnx \
    --tokens ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/tokens.txt \
    --language en \
    --num-threads 2 \
    --debug
