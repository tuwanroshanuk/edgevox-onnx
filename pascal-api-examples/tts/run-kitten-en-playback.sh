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
  popd
fi

# please visit
# https://k2-fsa.github.io/sherpa/onnx/tts/pretrained_models/kitten.html
if [ ! -f ./kitten-nano-en-v0_1-fp16/model.fp16.onnx ]; then
  curl -SL -O https://github.com/k2-fsa/sherpa-onnx/releases/download/tts-models/kitten-nano-en-v0_1-fp16.tar.bz2
  tar xf kitten-nano-en-v0_1-fp16.tar.bz2
  rm kitten-nano-en-v0_1-fp16.tar.bz2
fi

fpc \
  -dEDGEVOX_ONNX_USE_SHARED_LIBS \
  -Fu$EDGEVOX_ONNX_DIR/edgevox-onnx/pascal-api \
  -Fl$EDGEVOX_ONNX_DIR/build/install/lib \
  -Fl/usr/local/Cellar/portaudio/19.7.0/lib \
  ./kitten-en-playback.pas

# Please see ../portaudio-test/README.md
# for how to install portaudio on macOS

export LD_LIBRARY_PATH=$EDGEVOX_ONNX_DIR/build/install/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$EDGEVOX_ONNX_DIR/build/install/lib:$DYLD_LIBRARY_PATH

./kitten-en-playback
