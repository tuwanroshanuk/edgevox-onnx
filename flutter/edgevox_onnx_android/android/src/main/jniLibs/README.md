# Introduction

Pre-built libs are not checked-in.

Please use

 - https://github.com/k2-fsa/edgevox-onnx/blob/master/build-android-arm64-v8a.sh
 - https://github.com/k2-fsa/edgevox-onnx/blob/master/build-android-armv7-eabi.sh
 - https://github.com/k2-fsa/edgevox-onnx/blob/master/build-android-x86-64.sh
 - https://github.com/k2-fsa/edgevox-onnx/blob/master/build-android-x86.sh

The following is an example for `arm64-v8a`:

```bash
git clone https://github.com/k2-fsa/edgevox-onnx
cd edgevox-onnx

export EDGEVOX_ONNX_ENABLE_JNI=OFF
export EDGEVOX_ONNX_ENABLE_C_API=ON
./build-android-arm64-v8a.sh

cp -v build-android-arm64-v8a/install/lib/*.so flutter/edgevox_onnx_android/android/src/main/jniLibs/arm64-v8a/
```
