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
  dir=$PWD/build-android-x86-64
else
  dir=$PWD/build-android-x86-64-static
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
  export EDGEVOX_ONNXRUNTIME_LIB_DIR="$EDGEVOX_ONNX_ONNXRUNTIME_ROOT/jni/x86_64/"
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR="$EDGEVOX_ONNX_ONNXRUNTIME_ROOT/headers/"
elif [ "$BUILD_SHARED_LIBS" == ON ]; then
  if [ ! -f $onnxruntime_version/jni/x86_64/libonnxruntime.so ]; then
    mkdir -p $onnxruntime_version
    pushd $onnxruntime_version
    wget -c -q https://github.com/csukuangfj/onnxruntime-libs/releases/download/v${onnxruntime_version}/onnxruntime-android-${onnxruntime_version}.zip
    unzip onnxruntime-android-${onnxruntime_version}.zip
    rm onnxruntime-android-${onnxruntime_version}.zip
    popd
  fi

  export EDGEVOX_ONNXRUNTIME_LIB_DIR=$dir/$onnxruntime_version/jni/x86_64/
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$dir/$onnxruntime_version/headers/
else
  if [ ! -f ${onnxruntime_version}-static/lib/libonnxruntime.a ]; then
    wget -c -q https://github.com/csukuangfj/onnxruntime-libs/releases/download/v${onnxruntime_version}/onnxruntime-android-x86_64-static_lib-${onnxruntime_version}.zip
    unzip onnxruntime-android-x86_64-static_lib-${onnxruntime_version}.zip
    rm onnxruntime-android-x86_64-static_lib-${onnxruntime_version}.zip
    mv onnxruntime-android-x86_64-static_lib-${onnxruntime_version} ${onnxruntime_version}-static
  fi

  export EDGEVOX_ONNXRUNTIME_LIB_DIR=$dir/$onnxruntime_version-static/lib/
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$dir/$onnxruntime_version-static/include/
fi

echo "EDGEVOX_ONNXRUNTIME_LIB_DIR: $EDGEVOX_ONNXRUNTIME_LIB_DIR"
echo "EDGEVOX_ONNXRUNTIME_INCLUDE_DIR $EDGEVOX_ONNXRUNTIME_INCLUDE_DIR"

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
    -DCMAKE_INSTALL_PREFIX=./install \
    -DANDROID_ABI="x86_64" \
    -DEDGEVOX_ONNX_ENABLE_C_API=$EDGEVOX_ONNX_ENABLE_C_API \
    -DEDGEVOX_ONNX_ENABLE_WEBSOCKET=OFF \
    -DANDROID_PLATFORM=android-21 ..

    # By default, it links to libc++_static.a
    # -DANDROID_STL=c++_shared \

# make VERBOSE=1 -j4
make -j4
make install/strip

if [ "$BUILD_SHARED_LIBS" == ON ]; then
  cp -fv "$EDGEVOX_ONNXRUNTIME_LIB_DIR/libonnxruntime.so" install/lib
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
