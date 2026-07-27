#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  rm edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
fi

dart run \
  ./bin/zipformer-transducer.dart \
  --encoder ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/encoder-epoch-99-avg-1.int8.onnx \
  --decoder ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/decoder-epoch-99-avg-1.onnx \
  --joiner ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/joiner-epoch-99-avg-1.int8.onnx \
  --tokens ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt \
  --input-wav ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/test_wavs/0.wav
