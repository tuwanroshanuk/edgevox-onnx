#!/usr/bin/env bash

set -ex

cd "$(dirname "$0")"

old_version_code=20260608
new_version_code=20260707

old_version="1\.13\.3"
new_version="1\.13\.4"

replace_str="s/$old_version/$new_version/g"

sed -i.bak "$replace_str" ./CMakeLists.txt

sed -i.bak "$replace_str" ./edgevox-onnx/csrc/version.cc
sha1=$(git describe --match=NeVeRmAtCh --always --abbrev=8)
date=$(git log -1 --format=%ad --date=local)

find android -name "build.gradle" -type f -exec sed -i.bak "s/versionName \"$old_version\"/versionName \"$new_version\"/g" {} \;
find android -name "build.gradle.kts" -type f -exec sed -i.bak "s/versionName = \"$old_version\"/versionName = \"$new_version\"/g" {} \;

find android -name "build.gradle" -type f -exec sed -i.bak "s/versionCode $old_version_code/versionCode $new_version_code/g" {} \;
find android -name "build.gradle.kts" -type f -exec sed -i.bak "s/versionCode = $old_version_code/versionCode = $new_version_code/g" {} \;

sed -i.bak "s/  static const char \*sha1.*/  static const char \*sha1 = \"$sha1\";/g" ./edgevox-onnx/csrc/version.cc
sed -i.bak "s/  static const char \*date.*/  static const char \*date = \"$date\";/g" ./edgevox-onnx/csrc/version.cc


find scripts/wheel -name "setup.py" -type f -exec sed -i.bak "$replace_str" {} \;
sed -i.bak "$replace_str" ./setup.py

sed -i.bak "$replace_str" ./build-ios-shared.sh
sed -i.bak "$replace_str" ./pom.xml
sed -i.bak "$replace_str" ./edgevox-onnx/java-api/pom.xml
sed -i.bak "$replace_str" ./edgevox-onnx/java-api/README.md
sed -i.bak "$replace_str" ./jitpack.yml
sed -i.bak "$replace_str" ./android/EdgevoxOnnxAar/README.md

sed -i.bak "$replace_str" ./rust-api-examples/Cargo.toml
sed -i.bak "$replace_str" ./rust-api-examples/for-advanced-users.md
sed -i.bak "$replace_str" ./rust-api-examples/README.md
sed -i.bak "$replace_str" ./edgevox-onnx/rust/edgevox-onnx-sys/Cargo.toml
sed -i.bak "$replace_str" ./edgevox-onnx/rust/edgevox-onnx/Cargo.toml
sed -i.bak "$replace_str" ./edgevox-onnx/rust/edgevox-onnx/src/lib.rs
sed -i.bak "$replace_str" ./edgevox-onnx/rust/edgevox-onnx/README.md

sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-file/package.json
sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-file/src-tauri/Cargo.toml
sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-file/src-tauri/tauri.conf.json

sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-microphone/package.json
sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-microphone/src-tauri/Cargo.toml
sed -i.bak "$replace_str" ./tauri-examples/non-streaming-speech-recognition-from-microphone/src-tauri/tauri.conf.json

find android -name build.gradle -type f -exec sed -i.bak "s/edgevox-onnx:v$old_version/edgevox-onnx:v$new_version/g" {} \;
find android -name build.gradle.kts -type f -exec sed -i.bak "s/edgevox-onnx:v$old_version/edgevox-onnx:v$new_version/g" {} \;

find flutter -name "*.yaml" -type f -exec sed -i.bak "$replace_str" {} \;
find dart-api-examples -name "*.yaml" -type f -exec sed -i.bak "$replace_str" {} \;
find flutter-examples -name "*.yaml" -type f -exec sed -i.bak "$replace_str" {} \;
find flutter -name "*.podspec" -type f -exec sed -i.bak "$replace_str" {} \;
find nodejs-addon-examples -name package.json -type f -exec sed -i.bak "$replace_str" {} \;
find nodejs-examples -name package.json -type f -exec sed -i.bak "$replace_str" {} \;

find harmony-os -name "README.md" -type f -exec sed -i.bak "$replace_str" {} \;
find harmony-os -name oh-package.json5 -type f -exec sed -i.bak "$replace_str" {} \;
find harmony-os -name BuildProfile.ets -type f -exec sed -i.bak "$replace_str" {} \;

find mfc-examples -name "README.md" -type f -exec sed -i.bak "$replace_str" {} \;

find . -name "*.bak" -exec rm {} \;
