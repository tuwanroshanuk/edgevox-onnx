#!/usr/bin/env  bash

set -ex

dir=build-swift-macos
mkdir -p $dir
cd $dir

cmake \
  -DEDGEVOX_ONNX_ENABLE_BINARY=OFF \
  -DEDGEVOX_ONNX_BUILD_C_API_EXAMPLES=OFF \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DCMAKE_INSTALL_PREFIX=./install \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
  -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
  -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
  -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
  -DEDGEVOX_ONNX_ENABLE_JNI=OFF \
  -DEDGEVOX_ONNX_ENABLE_C_API=ON \
  -DEDGEVOX_ONNX_ENABLE_WEBSOCKET=OFF \
  ../

make -j4
make install
rm -fv ./install/include/cargs.h

libtool -static -o ./install/lib/libedgevox-onnx.a \
  ./install/lib/libedgevox-onnx-c-api.a \
  ./install/lib/libedgevox-onnx-core.a \
  ./install/lib/libkaldi-native-fbank-core.a \
  ./install/lib/libkissfft-float.a \
  ./install/lib/libedgevox-onnx-fstfar.a \
  ./install/lib/libedgevox-onnx-fst.a \
  ./install/lib/libedgevox-onnx-kaldifst-core.a \
  ./install/lib/libkaldi-decoder-core.a \
  ./install/lib/libucd.a \
  ./install/lib/libpiper_phonemize.a \
  ./install/lib/libespeak-ng.a \
  ./install/lib/libssentencepiece_core.a

xcodebuild -create-xcframework \
  -library install/lib/libedgevox-onnx.a \
  -headers install/include \
  -output edgevox-onnx.xcframework
