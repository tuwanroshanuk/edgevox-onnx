#!/usr/bin/env bash

set -ex

source ./setup.sh

java \
  -Djava.library.path=$PWD/../build/lib \
  -cp ../edgevox-onnx/java-api/target/edgevox-onnx-jvm-*.jar \
  ./VersionTest.java

