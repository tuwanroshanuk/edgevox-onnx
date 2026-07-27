#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  tar xvf edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  rm edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02.tar.bz2
  ls -lh edgevox-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileDolphinCtc.java
