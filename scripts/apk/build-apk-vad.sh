#!/usr/bin/env bash

# Please set the environment variable ANDROID_NDK
# before running this script

# Inside the $ANDROID_NDK directory, you can find a binary ndk-build
# and some other files like the file "build/cmake/android.toolchain.cmake"

set -e

log() {
  # This function is from espnet
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $*"
}

EDGEVOX_ONNX_VERSION=$(grep "EDGEVOX_ONNX_VERSION" ./CMakeLists.txt  | cut -d " " -f 2  | cut -d '"' -f 2)

log "Building APK for edgevox-onnx v${EDGEVOX_ONNX_VERSION}"

export EDGEVOX_ONNX_ENABLE_TTS=OFF

log "====================arm64-v8a================="
./build-android-arm64-v8a.sh
log "====================armv7-eabi================"
./build-android-armv7-eabi.sh
log "====================x86-64===================="
./build-android-x86-64.sh
log "====================x86===================="
./build-android-x86.sh


mkdir -p apks

log "https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/silero_vad.onnx"


# Download the model
pushd ./android/EdgevoxOnnxVad/app/src/main/assets/
wget -c https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/silero_vad.onnx
popd

for arch in arm64-v8a armeabi-v7a x86_64 x86; do
  log "------------------------------------------------------------"
  log "build apk for $arch"
  log "------------------------------------------------------------"
  src_arch=$arch
  if [ $arch == "armeabi-v7a" ]; then
    src_arch=armv7-eabi
  elif [ $arch == "x86_64" ]; then
    src_arch=x86-64
  fi

  ls -lh ./build-android-$src_arch/install/lib/*.so

  cp -v ./build-android-$src_arch/install/lib/*.so ./android/EdgevoxOnnxVad/app/src/main/jniLibs/$arch/

  pushd ./android/EdgevoxOnnxVad
  sed -i.bak s/2048/9012/g ./gradle.properties
  git diff ./gradle.properties
  ./gradlew assembleRelease
  popd

  mv android/EdgevoxOnnxVad/app/build/outputs/apk/release/app-release-unsigned.apk ./apks/edgevox-onnx-${EDGEVOX_ONNX_VERSION}-$arch-silero_vad.apk
  ls -lh apks
  rm -v ./android/EdgevoxOnnxVad/app/src/main/jniLibs/$arch/*.so
done

rm -rf ./android/EdgevoxOnnxVad/app/src/main/assets/*.onnx


# Now for ten-vad
git checkout .
pushd android/EdgevoxOnnxVad/app/src/main/java/com/nexus/edgevox/onnx
sed -i.bak s/"type = 0/type = 1/" ./MainActivity.kt
git diff
popd

log "https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/ten-vad.onnx"

# Download the model
pushd ./android/EdgevoxOnnxVad/app/src/main/assets/
wget -c https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/ten-vad.onnx
popd

for arch in arm64-v8a armeabi-v7a x86_64 x86; do
  log "------------------------------------------------------------"
  log "build apk for $arch"
  log "------------------------------------------------------------"
  src_arch=$arch
  if [ $arch == "armeabi-v7a" ]; then
    src_arch=armv7-eabi
  elif [ $arch == "x86_64" ]; then
    src_arch=x86-64
  fi

  ls -lh ./build-android-$src_arch/install/lib/*.so

  cp -v ./build-android-$src_arch/install/lib/*.so ./android/EdgevoxOnnxVad/app/src/main/jniLibs/$arch/

  pushd ./android/EdgevoxOnnxVad
  sed -i.bak s/2048/9012/g ./gradle.properties
  git diff ./gradle.properties
  ./gradlew assembleRelease
  popd

  mv android/EdgevoxOnnxVad/app/build/outputs/apk/release/app-release-unsigned.apk ./apks/edgevox-onnx-${EDGEVOX_ONNX_VERSION}-$arch-ten_vad.apk
  ls -lh apks
  rm -v ./android/EdgevoxOnnxVad/app/src/main/jniLibs/$arch/*.so
done

rm -rf ./android/EdgevoxOnnxVad/app/src/main/assets/*.onnx

ls -lh apks/
