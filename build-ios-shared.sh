#!/usr/bin/env  bash
#
# Note: This script is to build edgevox-onnx for flutter/dart, which requires
# us to use shared libraries for edgevox-onnx.
#
# Note: We still use static libraries for onnxruntime.

set -e

dir=build-ios-shared
mkdir -p $dir
cd $dir
onnxruntime_version=${EDGEVOX_ONNX_ONNXRUNTIME_VERSION:-1.27.0}
onnxruntime_dir=ios-onnxruntime/$onnxruntime_version

EDGEVOX_ONNX_GITHUB=github.com

if [ "$EDGEVOX_ONNX_GITHUB_MIRROW" == true ]; then
    EDGEVOX_ONNX_GITHUB=hub.nuaa.cf
fi

if [ ! -z CMAKE_VERBOSE_MAKEFILE ]; then
  CMAKE_VERBOSE_MAKEFILE=ON
else
  CMAKE_VERBOSE_MAKEFILE=OFF
fi

if [ ! -f $onnxruntime_dir/onnxruntime.xcframework/ios-arm64/onnxruntime.framework/onnxruntime ]; then
  mkdir -p $onnxruntime_dir
  pushd $onnxruntime_dir
  wget -c https://${EDGEVOX_ONNX_GITHUB}/csukuangfj/onnxruntime-libs/releases/download/v${onnxruntime_version}/onnxruntime-ios-static-xcframework-${onnxruntime_version}.zip
  unzip onnxruntime-ios-static-xcframework-${onnxruntime_version}.zip
  rm onnxruntime-ios-static-xcframework-${onnxruntime_version}.zip
  mv onnxruntime-ios-static-xcframework-${onnxruntime_version}/onnxruntime.xcframework .
  rmdir onnxruntime-ios-static-xcframework-${onnxruntime_version}
  cd ..
  ln -sf $onnxruntime_version/onnxruntime.xcframework .
  popd
fi

# First, for simulator
echo "Building for simulator (x86_64)"

export EDGEVOX_ONNXRUNTIME_LIB_DIR=$PWD/ios-onnxruntime/onnxruntime.xcframework/ios-arm64_x86_64-simulator
export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$PWD/ios-onnxruntime/onnxruntime.xcframework/ios-arm64_x86_64-simulator/onnxruntime.framework/Headers

echo "EDGEVOX_ONNXRUNTIME_LIB_DIR: $EDGEVOX_ONNXRUNTIME_LIB_DIR"
echo "EDGEVOX_ONNXRUNTIME_INCLUDE_DIR $EDGEVOX_ONNXRUNTIME_INCLUDE_DIR"

# Note: We use -DENABLE_ARC=1 here to fix the linking error:
#
# The symbol _NSLog is not defined
#
if [[ ! -f build/simulator_x86_64/install/lib/libedgevox-onnx-c-api.dylib ]]; then
  cmake \
    -DEDGEVOX_ONNX_ENABLE_BINARY=OFF \
    -DBUILD_PIPER_PHONMIZE_EXE=OFF \
    -DBUILD_PIPER_PHONMIZE_TESTS=OFF \
    -DBUILD_ESPEAK_NG_EXE=OFF \
    -DBUILD_ESPEAK_NG_TESTS=OFF \
    -S .. -D CMAKE_VERBOSE_MAKEFILE=$CMAKE_VERBOSE_MAKEFILE \
    -DCMAKE_TOOLCHAIN_FILE=./toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATOR64 \
    -DENABLE_BITCODE=0 \
    -DENABLE_ARC=1 \
    -DENABLE_VISIBILITY=1 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./build/simulator_x86_64/install \
    -DBUILD_SHARED_LIBS=ON \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    -DEDGEVOX_ONNX_ENABLE_JNI=OFF \
    -DEDGEVOX_ONNX_ENABLE_C_API=ON \
    -DEDGEVOX_ONNX_ENABLE_WEBSOCKET=OFF \
    -DDEPLOYMENT_TARGET=13.0 \
    -B build/simulator_x86_64

  cmake --build build/simulator_x86_64 -j 4 --target install
else
  echo "Skip building for simulator (x86_64)"
fi

echo "Building for simulator (arm64)"

if [[ ! -f build/simulator_arm64/install/lib/libedgevox-onnx-c-api.dylib ]]; then
  cmake \
    -DEDGEVOX_ONNX_ENABLE_BINARY=OFF \
    -DBUILD_PIPER_PHONMIZE_EXE=OFF \
    -DBUILD_PIPER_PHONMIZE_TESTS=OFF \
    -DBUILD_ESPEAK_NG_EXE=OFF \
    -DBUILD_ESPEAK_NG_TESTS=OFF \
    -S .. -D CMAKE_VERBOSE_MAKEFILE=$CMAKE_VERBOSE_MAKEFILE \
    -DCMAKE_TOOLCHAIN_FILE=./toolchains/ios.toolchain.cmake \
    -DPLATFORM=SIMULATORARM64 \
    -DENABLE_BITCODE=0 \
    -DENABLE_ARC=1 \
    -DENABLE_VISIBILITY=1 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./build/simulator_arm64/install \
    -DBUILD_SHARED_LIBS=ON \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    -DEDGEVOX_ONNX_ENABLE_JNI=OFF \
    -DEDGEVOX_ONNX_ENABLE_C_API=ON \
    -DEDGEVOX_ONNX_ENABLE_WEBSOCKET=OFF \
    -DDEPLOYMENT_TARGET=13.0 \
    -B build/simulator_arm64

  cmake --build build/simulator_arm64 -j 4 --target install
else
  echo "Skip building for simulator (arm64)"
fi

echo "Building for arm64"

if [[ ! -f build/os64/install/lib/libedgevox-onnx-c-api.dylib ]]; then
  export EDGEVOX_ONNXRUNTIME_LIB_DIR=$PWD/ios-onnxruntime/onnxruntime.xcframework/ios-arm64
  export EDGEVOX_ONNXRUNTIME_INCLUDE_DIR=$PWD/ios-onnxruntime/onnxruntime.xcframework/ios-arm64/onnxruntime.framework/Headers

  cmake \
    -DEDGEVOX_ONNX_ENABLE_BINARY=OFF \
    -DBUILD_PIPER_PHONMIZE_EXE=OFF \
    -DBUILD_PIPER_PHONMIZE_TESTS=OFF \
    -DBUILD_ESPEAK_NG_EXE=OFF \
    -DBUILD_ESPEAK_NG_TESTS=OFF \
    -S .. -D CMAKE_VERBOSE_MAKEFILE=$CMAKE_VERBOSE_MAKEFILE \
    -DCMAKE_TOOLCHAIN_FILE=./toolchains/ios.toolchain.cmake \
    -DPLATFORM=OS64 \
    -DENABLE_BITCODE=0 \
    -DENABLE_ARC=1 \
    -DENABLE_VISIBILITY=1 \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./build/os64/install \
    -DBUILD_SHARED_LIBS=ON \
    -DEDGEVOX_ONNX_ENABLE_PYTHON=OFF \
    -DEDGEVOX_ONNX_ENABLE_TESTS=OFF \
    -DEDGEVOX_ONNX_ENABLE_CHECK=OFF \
    -DEDGEVOX_ONNX_ENABLE_PORTAUDIO=OFF \
    -DEDGEVOX_ONNX_ENABLE_JNI=OFF \
    -DEDGEVOX_ONNX_ENABLE_C_API=ON \
    -DEDGEVOX_ONNX_ENABLE_WEBSOCKET=OFF \
    -DDEPLOYMENT_TARGET=13.0 \
    -B build/os64

  cmake --build build/os64 -j 4 --target install
else
  echo "Skip building for arm64"
fi

echo "Collect dynamic libraries "
mkdir -p ios-arm64 ios-arm64-simulator ios-x86_64-simulator

cp -v ./build/os64/install/lib/libedgevox-onnx-c-api.dylib ios-arm64/
cp -v ./build/simulator_arm64/install/lib/libedgevox-onnx-c-api.dylib ios-arm64-simulator/
cp -v .//build/simulator_x86_64/install/lib/libedgevox-onnx-c-api.dylib ios-x86_64-simulator/

# see https://github.com/k2-fsa/sherpa-onnx/issues/1172#issuecomment-2439662662
rm -rf ios-arm64_x86_64-simulator
mkdir ios-arm64_x86_64-simulator

lipo \
  -create \
    ios-arm64-simulator/libedgevox-onnx-c-api.dylib \
    ios-x86_64-simulator/libedgevox-onnx-c-api.dylib \
  -output \
    ios-arm64_x86_64-simulator/libedgevox-onnx-c-api.dylib

pushd ios-arm64
rm -rf edgevox_onnx.framework
mkdir edgevox_onnx.framework

lipo \
  -create \
    libedgevox-onnx-c-api.dylib \
  -output \
    edgevox_onnx

mv edgevox_onnx edgevox_onnx.framework/
cd edgevox_onnx.framework

install_name_tool \
  -change @rpath/libedgevox-onnx-c-api.dylib @rpath/edgevox_onnx.framework/edgevox_onnx \
  edgevox_onnx

install_name_tool \
  -id "@rpath/edgevox_onnx.framework/edgevox_onnx" \
  edgevox_onnx

chmod +x edgevox_onnx
strip -x edgevox_onnx
popd

pushd ios-arm64_x86_64-simulator
rm -rf edgevox_onnx.framework
mkdir edgevox_onnx.framework

lipo \
  -create \
    libedgevox-onnx-c-api.dylib \
  -output \
    edgevox_onnx

mv edgevox_onnx edgevox_onnx.framework/
cd edgevox_onnx.framework
install_name_tool \
  -change @rpath/libedgevox-onnx-c-api.dylib @rpath/edgevox_onnx.framework/edgevox_onnx \
  edgevox_onnx

install_name_tool \
  -id "@rpath/edgevox_onnx.framework/edgevox_onnx" \
  edgevox_onnx

chmod +x edgevox_onnx
strip -x edgevox_onnx
popd

for d in ios-arm64_x86_64-simulator ios-arm64; do
  dst=$d/edgevox_onnx.framework

  # The Info.plist is modified from
  # https://github.com/Spicely/flutter_openim_sdk_ffi/blob/main/ios/openim_sdk_ffi.framework/Info.plist
  cat >$dst/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleName</key>
	<string>edgevox_onnx</string>
	<key>DTSDKName</key>
	<string>iphoneos17.0</string>
	<key>DTXcode</key>
	<string>1501</string>
	<key>DTSDKBuild</key>
	<string>21A326</string>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleVersion</key>
	<string>1</string>
	<key>BuildMachineOSBuild</key>
	<string>23B81</string>
	<key>DTPlatformName</key>
	<string>iphoneos</string>
	<key>CFBundlePackageType</key>
	<string>FMWK</string>
	<key>CFBundleShortVersionString</key>
	<string>1.13.4</string>
	<key>CFBundleSupportedPlatforms</key>
	<array>
		<string>iPhoneOS</string>
	</array>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleExecutable</key>
	<string>edgevox_onnx</string>
	<key>DTCompiler</key>
	<string>com.apple.compilers.llvm.clang.1_0</string>
	<key>UIRequiredDeviceCapabilities</key>
	<array>
		<string>arm64</string>
	</array>
	<key>MinimumOSVersion</key>
	<string>13.0</string>
	<key>CFBundleIdentifier</key>
	<string>com.nexus.edgevox.onnx</string>
	<key>UIDeviceFamily</key>
	<array>
		<integer>1</integer>
		<integer>2</integer>
	</array>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>DTPlatformVersion</key>
	<string>17.0</string>
	<key>DTXcodeBuild</key>
	<string>15A507</string>
	<key>DTPlatformBuild</key>
	<string>21A326</string>
	<key>SupportedArchitectures</key>
	<array>
		<string>arm64</string>
		<string>x86_64</string>
	</array>
	<key>SupportedPlatform</key>
	<string>ios</string>
</dict>
</plist>
EOF
done

rm -rf edgevox_onnx.xcframework
xcodebuild -create-xcframework \
  -framework ios-arm64/edgevox_onnx.framework \
  -framework ios-arm64_x86_64-simulator/edgevox_onnx.framework \
  -output edgevox_onnx.xcframework

cd edgevox_onnx.xcframework
echo "PWD: $PWD"
ls -lh
echo "---"
ls -lh */*
