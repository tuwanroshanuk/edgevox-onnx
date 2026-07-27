#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f zh.wav ]; then
  # wget https://huggingface.co/csukuangfj/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/resolve/main/test_wavs/zh.wav
  wget https://hf-mirror.com/csukuangfj/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/resolve/main/test_wavs/zh.wav
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingWebsocketClient.java
