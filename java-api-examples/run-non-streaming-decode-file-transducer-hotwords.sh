#!/usr/bin/env bash

set -ex

source ./setup.sh

if [ ! -f ./edgevox-onnx-conformer-zh-stateless2-2023-05-23/tokens.txt ]; then
  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-conformer-zh-stateless2-2023-05-23.tar.bz2
  tar xvf edgevox-onnx-conformer-zh-stateless2-2023-05-23.tar.bz2
  rm edgevox-onnx-conformer-zh-stateless2-2023-05-23.tar.bz2
fi

if [ ! -f hotwords_cn.txt ]; then
  cat > hotwords_cn.txt <<EOF
朱丽楠
EOF
fi

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  NonStreamingDecodeFileTransducerHotwords.java
