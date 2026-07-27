#!/usr/bin/env bash
# Common setup script for edgevox-onnx Java API examples
# Source this file at the beginning of each run-xxx.sh script:
#   source ./setup.sh

# Build edgevox-onnx C++ library if not exists
if [[ ! -f ../build/lib/libedgevox-onnx-jni.dylib  && ! -f ../build/lib/libedgevox-onnx-jni.so ]]; then
  mkdir -p ../build
  pushd ../build
  cmake \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DBUILD_SHARED_LIBS=ON \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    -DEDGEVOX_ONNX_ENABLE_JNI=ON \
    ..

  make -j4
  ls -lh lib
  popd
fi

# Build edgevox-onnx JVM jar if not exists
if [ ! -f ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar ]; then
  pushd ../edgevox-onnx/java-api
  mvn package
  popd
fi
