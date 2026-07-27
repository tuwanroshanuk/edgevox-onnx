#!/usr/bin/env bash

set -ex

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
EDGEVOX_ONNX_DIR=$(cd $SCRIPT_DIR/../.. && pwd)

echo "EDGEVOX_ONNX_DIR: $EDGEVOX_ONNX_DIR"

if [[ ! -f ../../build/install/lib/libedgevox-onnx-c-api.dylib  && ! -f ../../build/install/lib/libedgevox-onnx-c-api.so && ! -f ../../build/install/lib/edgevox-onnx-c-api.dll ]]; then
  mkdir -p ../../build
  pushd ../../build
  cmake \
    -DCMAKE_INSTALL_PREFIX=./install \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DBUILD_SHARED_LIBS=ON \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    ..

  cmake --build . --target install --config Release
  ls -lh lib
  popd
fi

if [ ! -f ./sherpa-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  tar xvf sherpa-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  rm sherpa-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
  ls -lh sherpa-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25
fi


fpc \
  -dEDGEVOX_ONNX_USE_SHARED_LIBS \
  -Fu$EDGEVOX_ONNX_DIR/edgevox-onnx/pascal-api \
  -Fl$EDGEVOX_ONNX_DIR/build/install/lib \
  ./fire_red_asr_ctc.pas

export LD_LIBRARY_PATH=$EDGEVOX_ONNX_DIR/build/install/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$EDGEVOX_ONNX_DIR/build/install/lib:$DYLD_LIBRARY_PATH

./fire_red_asr_ctc
