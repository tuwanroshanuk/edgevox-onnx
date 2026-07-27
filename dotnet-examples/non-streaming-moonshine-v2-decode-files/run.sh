#!/usr/bin/env bash

set -ex

if [ ! -f ./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/encoder_model.ort ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
  tar xvf edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
  rm edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27.tar.bz2
fi

dotnet run
