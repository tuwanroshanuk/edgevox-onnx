#!/usr/bin/env bash
set -ex

if [ ! -f ./edgevox-onnx-whisper-tiny/tiny-encoder.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
  tar xvf edgevox-onnx-whisper-tiny.tar.bz2
  rm edgevox-onnx-whisper-tiny.tar.bz2
fi

if [ ! -f ./spoken-language-identification-test-wavs/en-english.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/spoken-language-identification-test-wavs.tar.bz2
  tar xvf spoken-language-identification-test-wavs.tar.bz2
  rm spoken-language-identification-test-wavs.tar.bz2
fi

cargo run --example spoken_language_identification --   --wav ./spoken-language-identification-test-wavs/de-german.wav   --whisper-encoder ./edgevox-onnx-whisper-tiny/tiny-encoder.int8.onnx   --whisper-decoder ./edgevox-onnx-whisper-tiny/tiny-decoder.int8.onnx   --provider cpu   --num-threads 1
