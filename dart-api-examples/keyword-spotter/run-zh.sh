#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/kws-models/edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  tar xvf edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
  rm edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01.tar.bz2
fi

dart run \
  ./bin/zipformer-transducer.dart \
  --encoder ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/encoder-epoch-12-avg-2-chunk-16-left-64.onnx \
  --decoder ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/decoder-epoch-12-avg-2-chunk-16-left-64.onnx \
  --joiner ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/joiner-epoch-12-avg-2-chunk-16-left-64.onnx \
  --tokens ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/tokens.txt \
  --keywords-file ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/test_wavs/test_keywords.txt \
  --input-wav ./edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/test_wavs/3.wav

