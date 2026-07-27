#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  tar xvf edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  rm edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
fi

dart run \
  ./bin/cohere-transcribe.dart \
  --encoder ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx \
  --decoder ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/decoder.int8.onnx \
  --tokens ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/tokens.txt \
  --input-wav ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/test_wavs/en.wav \
  --language en
