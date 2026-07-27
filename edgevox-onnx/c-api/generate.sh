#!/usr/bin/env bash
set -ex

nm -g ../../build/lib/libedgevox-onnx-c-api.dylib | awk '$2=="T" && $3 ~ /^_Sherpa/ {print $3}' | sort  > ./edgevox-onnx-symbols-c.exp

