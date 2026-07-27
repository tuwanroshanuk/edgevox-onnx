#!/usr/bin/env bash

set -ex

if [ ! -e ./edgevox-onnx-online-punct-en-2024-08-06/model.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  tar xvf edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
  rm edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
fi

dotnet run
