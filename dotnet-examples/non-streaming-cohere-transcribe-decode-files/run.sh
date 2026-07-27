#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01/encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  tar xvf edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
  rm edgevox-onnx-cohere-transcribe-14-lang-int8-2026-04-01.tar.bz2
fi

dotnet run
