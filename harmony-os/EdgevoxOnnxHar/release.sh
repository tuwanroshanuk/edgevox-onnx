#!/usr/bin/env bash
set -ex

export PATH=/Users/fangjun/software/command-line-tools/bin:$PATH

cp -v ../../CHANGELOG.md ./edgevox_onnx

hvigorw clean --no-daemon
hvigorw --mode module -p product=default -p module=edgevox_onnx@default assembleHar --analyze=normal --parallel --incremental --no-daemon

ohpm publish ./edgevox_onnx/build/default/outputs/default/edgevox_onnx.har
