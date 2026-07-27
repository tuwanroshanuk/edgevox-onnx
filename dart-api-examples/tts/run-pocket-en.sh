#!/usr/bin/env bash

set -ex

dart pub get

# please visit
# https://k2-fsa.github.io/sherpa/onnx/tts/pocket.html
# to download more models
if [ ! -f ./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  tar xvf edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
  rm edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
fi

dart run \
  ./bin/pocket-en.dart \
  --lm-flow ./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx \
  --lm-main ./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx \
  --encoder ./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx \
  --decoder ./edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx \
  --text-conditioner ./edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx \
  --vocab-json ./edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json \
  --token-scores-json ./edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json \
  --reference-audio ./edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav \
  --output-wav pocket-en-0.wav \
  --text "Friends fell out often because life was changing so fast. The easiest thing in the world was to lose touch with someone."

ls -lh *.wav
