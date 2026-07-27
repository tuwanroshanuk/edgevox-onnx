#!/usr/bin/env bash

# Please refer to
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-paraformer/paraformer-models.html#csukuangfj-edgevox-onnx-streaming-paraformer-bilingual-zh-en-chinese-english
# to download the model files

set -ex
if [ ! -d ./edgevox-onnx-streaming-paraformer-bilingual-zh-en ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  tar xvf edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
  rm edgevox-onnx-streaming-paraformer-bilingual-zh-en.tar.bz2
fi

dotnet run -c Release \
  --tokens ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt \
  --paraformer-encoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/encoder.int8.onnx \
  --paraformer-decoder ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/decoder.int8.onnx \
  --decoding-method greedy_search \
  --files ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/1.wav \
  ./edgevox-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/0.wav
