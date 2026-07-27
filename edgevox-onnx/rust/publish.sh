#!/usr/bin/env bash

pushd edgevox-onnx-sys

cp -v ../../../README.md ./
cp -v ../../../LICENSE ./

popd

pushd edgevox-onnx

cp -v ../../../README.md ./
cp -v ../../../LICENSE ./

popd
