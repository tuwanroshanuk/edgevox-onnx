#!/usr/bin/env bash
set -ex

nm -g ../../build/lib/libedgevox-onnx-jni.dylib | awk '$2=="T" && $3 ~ /^_Java_com_nexus/ {print $3}' | sort  > ./edgevox-onnx-symbols.exp

