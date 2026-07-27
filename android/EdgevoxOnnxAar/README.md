# Usage of this project

```
git clone https://github.com/k2-fsa/edgevox-onnx
cd edgevox-onnx

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/v1.13.4/edgevox-onnx-v1.13.4-android.tar.bz2
tar xvf edgevox-onnx-v1.13.4-android.tar.bz2

cp -v jniLibs/arm64-v8a/* android/EdgevoxOnnxAar/edgevox_onnx/src/main/jniLibs/arm64-v8a/
cp -v jniLibs/armeabi-v7a/* android/EdgevoxOnnxAar/edgevox_onnx/src/main/jniLibs/armeabi-v7a/
cp -v jniLibs/x86/* android/EdgevoxOnnxAar/edgevox_onnx/src/main/jniLibs/x86/
cp -v jniLibs/x86_64/* android/EdgevoxOnnxAar/edgevox_onnx/src/main/jniLibs/x86_64/

cd android/EdgevoxOnnxAar

./gradlew :edgevox_onnx:assembleRelease
ls -lh ./edgevox_onnx/build/outputs/aar/edgevox_onnx-release.aar
cp ./edgevox_onnx/build/outputs/aar/edgevox_onnx-release.aar ../../edgevox-onnx-1.13.4.aar
```
