# Introduction

This file keeps some notes about how packages in this directory
are created.

1. Create `edgevox_onnx`.

```bash
flutter create --template plugin edgevox_onnx
```

2. Create `edgevox_onnx_macos`

```bash
flutter create --template plugin_ffi --platforms macos edgevox_onnx_macos
```

3. Create `edgevox_onnx_linux`

```bash
flutter create --template plugin_ffi --platforms linux edgevox_onnx_linux
```

4. Create `edgevox_onnx_windows`

```bash
flutter create --template plugin_ffi --platforms linux edgevox_onnx_windows
```

5. Create `edgevox_onnx_android_arm64`, `edgevox_onnx_android_armeabi`, `edgevox_onnx_android_x86`, `edgevox_onnx_android_x86_64`

```bash
flutter create --template plugin_ffi --platforms android --org com.k2fsa.sherpa.onnx.arm64 edgevox_onnx_android_arm64
flutter create --template plugin_ffi --platforms android --org com.k2fsa.sherpa.onnx.armeabi edgevox_onnx_android_armeabi
flutter create --template plugin_ffi --platforms android --org com.k2fsa.sherpa.onnx.x86 edgevox_onnx_android_x86
flutter create --template plugin_ffi --platforms android --org com.k2fsa.sherpa.onnx.x86_64 edgevox_onnx_android_x86_64
```

6. Create `edgevox_onnx_ios`

```bash
flutter create --template plugin_ffi --platforms ios edgevox_onnx_ios
```
