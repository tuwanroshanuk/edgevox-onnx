#!/usr/bin/env bash
set -ex

# If BUILD_SHARED_LIBS is ON, we use libonnxruntime.so
# If BUILD_SHARED_LIBS is OFF, we use libonnxruntime.a
#
# In any case, we will have libedgevox-onnx-jni.so
#
# If BUILD_SHARED_LIBS is OFF, then libonnxruntime.a is linked into libedgevox-onnx-jni.so
# and you only need to copy libedgevox-onnx-jni.so to your Android projects.
#
# If BUILD_SHARED_LIBS is ON, then you need to copy both libedgevox-onnx-jni.so
# and libonnxruntime.so to your Android projects
#
if [ -z $BUILD_SHARED_LIBS ]; then
  BUILD_SHARED_LIBS=ON
fi

if [ $BUILD_SHARED_LIBS == ON ]; then
  dir=$PWD/build-android-arm64-v8a
else
  dir=$PWD/build-android-arm64-v8a-static
fi

if [ -n "${EDGEVOX_ONNXRUNTIME_LIB_DIR:-}" ] && [ -n "${EDGEVOX_ONNXRUNTIME_INCLUDE_DIR:-}" ]; then
  if [ ! -d "$EDGEVOX_ONNXRUNTIME_LIB_DIR" ]; then
    echo "Error: EDGEVOX_ONNXRUNTIME_LIB_DIR does not exist: $EDGEVOX_ONNXRUNTIME_LIB_DIR"
    exit 1
  fi
  if [ ! -d "$EDGEVOX_ONNXRUNTIME_INCLUDE_DIR" ]; then
    echo "Error: EDGEVOX_ONNXRUNTIME_INCLUDE_DIR does not exist: $EDGEVOX_ONNXRUNTIME_INCLUDE_DIR"
    exit 1
  fi
  EDGEVOX_ONNXRUNTIME_LIB_DIR=$(cd "$EDGEVOX_ONNXRUNTIME_LIB_DIR" && pwd)
  EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$(cd "$EDGEVOX_ONNXRUNTIME_INCLUDE_DIR" && pwd)
  export EDGEVOX_ONNXRUNTIME_LIB_DIR
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR
elif [ -n "${EDGEVOX_ONNX_ONNXRUNTIME_ROOT:-}" ] && [ "$BUILD_SHARED_LIBS" == ON ]; then
  if [ ! -d "$EDGEVOX_ONNX_ONNXRUNTIME_ROOT" ]; then
    echo "Error: EDGEVOX_ONNX_ONNXRUNTIME_ROOT does not exist: $EDGEVOX_ONNX_ONNXRUNTIME_ROOT"
    exit 1
  fi
  EDGEVOX_ONNX_ONNXRUNTIME_ROOT=$(cd "$EDGEVOX_ONNX_ONNXRUNTIME_ROOT" && pwd)
  export EDGEVOX_ONNX_ONNXRUNTIME_ROOT
fi

mkdir -p $dir
cd $dir

# Note from https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-android
# (optional) remove the hardcoded debug flag in Android NDK android-ndk
# issue: https://github.com/android/ndk/issues/243
#
# open $ANDROID_NDK/build/cmake/android.toolchain.cmake for ndk < r23
# or $ANDROID_NDK/build/cmake/android-legacy.toolchain.cmake for ndk >= r23
#
# delete "-g" line
#
# list(APPEND ANDROID_COMPILER_FLAGS
#   -g
#   -DANDROID

if [ -z $ANDROID_NDK ]; then
  ANDROID_NDK=/star-fj/fangjun/software/android-sdk/ndk/22.1.7171670
  if [ $BUILD_SHARED_LIBS == OFF ]; then
    ANDROID_NDK=/star-fj/fangjun/software/android-sdk/ndk/27.0.11718014
  fi
  # or use
  # ANDROID_NDK=/star-fj/fangjun/software/android-ndk
  #
  # Inside the $ANDROID_NDK directory, you can find a binary ndk-build
  # and some other files like the file "build/cmake/android.toolchain.cmake"

  if [ ! -d $ANDROID_NDK ]; then
    # For macOS, I have installed Android Studio, select the menu
    # Tools -> SDK manager -> Android SDK
    # and set "Android SDK location" to /Users/fangjun/software/my-android
    ANDROID_NDK=/Users/fangjun/software/my-android/ndk/22.1.7171670

    if [ $BUILD_SHARED_LIBS == OFF ]; then
      ANDROID_NDK=/Users/fangjun/software/my-android/ndk/27.0.11718014
    fi
  fi
fi

if [ ! -d $ANDROID_NDK ]; then
  echo Please set the environment variable ANDROID_NDK before you run this script
  exit 1
fi

echo "ANDROID_NDK: $ANDROID_NDK"
sleep 1
onnxruntime_version=${EDGEVOX_ONNX_ONNXRUNTIME_VERSION:-1.27.0}

if [ -n "${EDGEVOX_ONNXRUNTIME_LIB_DIR:-}" ] && [ -n "${EDGEVOX_ONNXRUNTIME_INCLUDE_DIR:-}" ]; then
  echo "Using externally provided ONNX Runtime"
elif [ -n "${EDGEVOX_ONNX_ONNXRUNTIME_ROOT:-}" ] && [ "$BUILD_SHARED_LIBS" == ON ]; then
  export EDGEVOX_ONNXRUNTIME_LIB_DIR="$EDGEVOX_ONNX_ONNXRUNTIME_ROOT/jni/arm64-v8a/"
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR="$EDGEVOX_ONNX_ONNXRUNTIME_ROOT/headers/"
elif [ "$BUILD_SHARED_LIBS" == ON ]; then
  if [ ! -f $onnxruntime_version/jni/arm64-v8a/libonnxruntime.so ]; then
    mkdir -p $onnxruntime_version
    pushd $onnxruntime_version
    wget -c -q https://github.com/csukuangfj/onnxruntime-libs/releases/download/v${onnxruntime_version}/onnxruntime-android-${onnxruntime_version}.zip
    unzip onnxruntime-android-${onnxruntime_version}.zip
    rm onnxruntime-android-${onnxruntime_version}.zip
    popd
  fi

  export EDGEVOX_ONNXRUNTIME_LIB_DIR=$dir/$onnxruntime_version/jni/arm64-v8a/
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$dir/$onnxruntime_version/headers/
else
  if [ ! -f ${onnxruntime_version}-static/lib/libonnxruntime.a ]; then
    wget -c -q https://github.com/csukuangfj/onnxruntime-libs/releases/download/v${onnxruntime_version}/onnxruntime-android-arm64-v8a-static_lib-${onnxruntime_version}.zip
    unzip onnxruntime-android-arm64-v8a-static_lib-${onnxruntime_version}.zip
    rm onnxruntime-android-arm64-v8a-static_lib-${onnxruntime_version}.zip
    mv onnxruntime-android-arm64-v8a-static_lib-${onnxruntime_version} ${onnxruntime_version}-static
  fi

  export EDGEVOX_ONNXRUNTIME_LIB_DIR=$dir/$onnxruntime_version-static/lib/
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$dir/$onnxruntime_version-static/include/
fi

echo "EDGEVOX_ONNXRUNTIME_LIB_DIR: $EDGEVOX_ONNXRUNTIME_LIB_DIR"
echo "EDGEVOX_ONNXRUNTIME_INCLUDE_DIR $EDGEVOX_ONNXRUNTIME_INCLUDE_DIR"

if [ -z $EDGEVOX_ONNX_ENABLE_RKNN ]; then
  EDGEVOX_ONNX_ENABLE_RKNN=OFF
fi

if [ -z $EDGEVOX_ONNX_ENABLE_QNN ]; then
  EDGEVOX_ONNX_ENABLE_QNN=OFF
fi

if [ $EDGEVOX_ONNX_ENABLE_RKNN == ON ]; then
  rknn_version=2.2.0
  if [ ! -d ./librknnrt-android ]; then
    rm -fv librknnrt-android.tar.bz2
    wget https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/librknnrt-android.tar.bz2
    tar xvf librknnrt-android.tar.bz2
    rm librknnrt-android.tar.bz2
  fi

  export EDGEVOX_ONNX_RKNN_TOOLKIT2_LIB_DIR=$PWD/librknnrt-android/v$rknn_version/arm64-v8a/
  export CPLUS_INCLUDE_PATH=$PWD/librknnrt-android/v$rknn_version/include:$CPLUS_INCLUDE_PATH
fi

if [ -z $EDGEVOX_ONNX_ENABLE_TTS ]; then
  EDGEVOX_ONNX_ENABLE_TTS=ON
fi

if [ -z $EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION ]; then
  EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION=ON
fi

if [ -z $EDGEVOX_ONNX_ENABLE_BINARY ]; then
  EDGEVOX_ONNX_ENABLE_BINARY=OFF
fi

if [ -z $EDGEVOX_ONNX_ENABLE_C_API ]; then
  EDGEVOX_ONNX_ENABLE_C_API=OFF
fi

if [ -z $EDGEVOX_ONNX_ANDROID_PLATFORM ]; then
  EDGEVOX_ONNX_ANDROID_PLATFORM=android-21
fi

if [ -z $EDGEVOX_ONNX_ENABLE_JNI ]; then
  EDGEVOX_ONNX_ENABLE_JNI=ON
fi

cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DEDGEVOX_ONNX_ENABLE_TTS=$EDGEVOX_ONNX_ENABLE_TTS \
    -DEDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION=$EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION \
    -DEDGEVOX_ONNX_ENABLE_BINARY=$EDGEVOX_ONNX_ENABLE_BINARY \
    -DBUILD_PIPER_PHONMIZE_EXE=OFF \
    -DBUILD_PIPER_PHONMIZE_TESTS=OFF \
    -DBUILD_ESPEAK_NG_EXE=OFF \
    -DBUILD_ESPEAK_NG_TESTS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    -DEDGEVOX_ONNX_ENABLE_JNI=$EDGEVOX_ONNX_ENABLE_JNI \
    -DEDGEVOX_ONNX_LINK_LIBSTDCPP_STATICALLY=OFF \
    -DEDGEVOX_ONNX_ENABLE_C_API=$EDGEVOX_ONNX_ENABLE_C_API \
    -DCMAKE_INSTALL_PREFIX=./install \
    -DEDGEVOX_ONNX_ENABLE_RKNN=$EDGEVOX_ONNX_ENABLE_RKNN \
    -DEDGEVOX_ONNX_ENABLE_QNN=$EDGEVOX_ONNX_ENABLE_QNN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=$EDGEVOX_ONNX_ANDROID_PLATFORM ..

    # By default, it links to libc++_static.a
    # -DANDROID_STL=c++_shared \

# Please use -DANDROID_PLATFORM=android-27 if you want to use Android NNAPI

# make VERBOSE=1 -j4
make -j4
make install/strip
if [ "$BUILD_SHARED_LIBS" == ON ]; then
  cp -fv "$EDGEVOX_ONNXRUNTIME_LIB_DIR/libonnxruntime.so" install/lib
fi

if [ $EDGEVOX_ONNX_ENABLE_RKNN == ON ]; then
  cp -fv $EDGEVOX_ONNX_RKNN_TOOLKIT2_LIB_DIR/librknnrt.so install/lib
fi

rm -rf install/share
rm -rf install/lib/pkgconfig
rm -rf install/lib/lib*.a
if [ -f install/lib/libedgevox-onnx-c-api.so ]; then
  cat >install/lib/README.md <<EOF
# Introduction

Note that if you use Android Studio, then you only need to
copy libonnxruntime.so and libedgevox-onnx-jni.so
to your jniLibs, and you don't need libedgevox-onnx-c-api.so or
libedgevox-onnx-cxx-api.so.

libedgevox-onnx-c-api.so and libedgevox-onnx-cxx-api.so are for users
who don't use JNI. In that case, libedgevox-onnx-jni.so is not needed.

In any case, libonnxruntime.so is always needed.
EOF
  ls -lh install/lib/README.md
fi

# To run the generated binaries on Android, please use the following steps.
#
#
# 1. Copy edgevox-onnx and its dependencies to Android
#
#   cd build-android-arm64-v8a/install/lib
#   adb push ./lib*.so /data/local/tmp
#   cd ../bin
#   adb push ./edgevox-onnx /data/local/tmp
#
# 2. Login into Android
#
#   adb shell
#   cd /data/local/tmp
#   ./edgevox-onnx
#
# It should show the help message of edgevox-onnx.
#
# Please use the above approach to copy model files to your phone.
#
# ----------------------------------------
# For android rknn
# ----------------------------------------
# If you get the following error from the logcat
# 2025-04-15 15:27:43.441 19568-19646 RKNN                    com.nexus.edgevox.onnx                E  Meet unsupported input dtype for gather
# 2025-04-15 15:27:43.442 19568-19646 RKNN                    com.nexus.edgevox.onnx                E  Op type:Gather, name: Gather:/Concat_78_2gather, fallback cpu failed. If using rknn, update to the latest toolkit2 and runtime from: https://console.zbox.filez.com/l/I00fc3 (PWD: rknn). If using rknn-llm, update from: https://github.com/airockchip/rknn-llm
# 2025-04-15 15:27:43.442 19568-19646 edgevox-onnx             com.nexus.edgevox.onnx                W  Return code is: -1
# 2025-04-15 15:27:43.442 19568-19646 edgevox-onnx             com.nexus.edgevox.onnx                W  Failed to run encoder
#
# You need to update /vendor/lib64/librknnrt.so and /vendor/lib/librknnrt.so
#
# adb root
# adb remount /vendor
# adb push ./install/lib/librknnrt.so /vendor/lib64
# adb push ./install/lib/librknnrt.so /vendor/lib
