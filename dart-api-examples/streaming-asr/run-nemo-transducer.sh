#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms.tar.bz2
  tar xvf edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms.tar.bz2
  rm edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms.tar.bz2
fi

dart run \
  ./bin/zipformer-transducer.dart \
  --encoder ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/encoder.onnx \
  --decoder ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/decoder.onnx \
  --joiner ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/joiner.onnx \
  --tokens ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/tokens.txt \
  --input-wav ./edgevox-onnx-nemo-streaming-fast-conformer-transducer-en-80ms/test_wavs/0.wav
