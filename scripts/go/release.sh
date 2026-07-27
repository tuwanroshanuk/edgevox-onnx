#!/usr/bin/env bash

set -ex

git config --global user.email "csukuangfj@gmail.com"
git config --global user.name "Fangjun Kuang"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
EDGEVOX_ONNX_DIR=$(realpath $SCRIPT_DIR/../..)
echo "SCRIPT_DIR: $SCRIPT_DIR"
echo "EDGEVOX_ONNX_DIR: $EDGEVOX_ONNX_DIR"


EDGEVOX_ONNX_VERSION=$(grep "EDGEVOX_ONNX_VERSION" $EDGEVOX_ONNX_DIR/CMakeLists.txt  | cut -d " " -f 2  | cut -d '"' -f 2)
echo "EDGEVOX_ONNX_VERSION $EDGEVOX_ONNX_VERSION"

GO_PROXY_WAIT_SECS=30
GO_PROXY_MAX_RETRIES=40

# Proactively tell the Go module proxy to fetch a specific version.
# Requesting the .info endpoint forces proxy.golang.org to fetch and cache
# the module rather than waiting for its periodic indexing crawl.
kick_go_proxy() {
  local pkg="$1"
  local version="$2"
  echo "Kicking Go proxy to fetch $pkg@$version ..."
  curl -sS "https://proxy.golang.org/${pkg}/@v/${version}.info" || true
  echo ""
}

# Wait for Go proxy to index newly published packages.
# Uses the .info endpoint which is a direct, reliable check.
wait_for_go_proxy() {
  local pkg="$1"
  local version="$2"
  local i

  kick_go_proxy "$pkg" "$version"

  for i in $(seq 1 $GO_PROXY_MAX_RETRIES); do
    echo "Attempt $i/$GO_PROXY_MAX_RETRIES: checking $pkg@$version ..."
    if curl -sS -o /dev/null -w "%{http_code}" "https://proxy.golang.org/${pkg}/@v/${version}.info" | grep -q "200"; then
      echo "  -> $pkg@$version is available on Go proxy"
      return 0
    fi
    echo "  -> not ready yet, sleeping ${GO_PROXY_WAIT_SECS}s ..."
    sleep $GO_PROXY_WAIT_SECS
  done
  echo "ERROR: $pkg@$version not available after $GO_PROXY_MAX_RETRIES attempts"
  return 1
}

# Run go mod tidy with retries. Sometimes the proxy has the module metadata
# but the zip download is still being processed.
run_go_mod_tidy() {
  local i
  for i in $(seq 1 $GO_PROXY_MAX_RETRIES); do
    echo "Attempt $i/$GO_PROXY_MAX_RETRIES: running go mod tidy ..."
    if go mod tidy 2>&1; then
      echo "  -> go mod tidy succeeded"
      return 0
    fi
    echo "  -> go mod tidy failed, sleeping ${GO_PROXY_WAIT_SECS}s ..."
    sleep $GO_PROXY_WAIT_SECS
  done
  echo "ERROR: go mod tidy failed after $GO_PROXY_MAX_RETRIES attempts"
  return 1
}

function linux() {
  echo "Process linux"
  git clone git@github.com:k2-fsa/sherpa-onnx-go-linux.git

  rm -v ./edgevox-onnx-go-linux/*.go

  cp -v ./edgevox_onnx.go ./edgevox-onnx-go-linux/
  cp -v ./_internal/c-api.h ./edgevox-onnx-go-linux/
  cp -v ./_internal/build_linux_*.go ./edgevox-onnx-go-linux/

  rm -rf edgevox-onnx-go-linux/lib/x86_64-unknown-linux-gnu/lib*
  dst=$(realpath edgevox-onnx-go-linux/lib/x86_64-unknown-linux-gnu)
  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-manylinux2014_x86_64.whl
  unzip edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-manylinux2014_x86_64.whl

  rm -fv $dst/_sherpa*.so
  cp -v edgevox_onnx/lib/lib*.so* $dst

  cd ..
  rm -rf t

  rm -rf edgevox-onnx-go-linux/lib/aarch64-unknown-linux-gnu/lib*
  dst=$(realpath edgevox-onnx-go-linux/lib/aarch64-unknown-linux-gnu)
  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-manylinux2014_aarch64.whl
  unzip ./edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-manylinux2014_aarch64.whl

  rm -fv $dst/_sherpa*.so
  cp -v edgevox_onnx/lib/lib*.so* $dst

  cd ..
  rm -rf t

  rm -rf edgevox-onnx-go-linux/lib/arm-unknown-linux-gnueabihf/lib*
  dst=$(realpath edgevox-onnx-go-linux/lib/arm-unknown-linux-gnueabihf)
  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-$EDGEVOX_ONNX_VERSION-py3-none-manylinux_2_35_armv7l.whl
  unzip ./edgevox_onnx_core-$EDGEVOX_ONNX_VERSION-py3-none-manylinux_2_35_armv7l.whl

  rm -fv $dst/_sherpa*.so
  cp -v edgevox_onnx/lib/lib*.so* $dst

  cd ..
  rm -rf t

  echo "------------------------------"
  cd edgevox-onnx-go-linux
  git status
  git add .
  git commit -m "Release v$EDGEVOX_ONNX_VERSION" && \
  git push && \
  git tag v$EDGEVOX_ONNX_VERSION && \
  git push origin v$EDGEVOX_ONNX_VERSION || true
  cd ..
  kick_go_proxy "github.com/k2-fsa/sherpa-onnx-go-linux" "v$SHERPA_ONNX_VERSION"
  rm -rf edgevox-onnx-go-linux
}

function osx() {
  echo "Process osx-x64"
  git clone git@github.com:k2-fsa/sherpa-onnx-go-macos.git
  rm -v ./edgevox-onnx-go-macos/*.go
  cp -v ./edgevox_onnx.go ./edgevox-onnx-go-macos/
  cp -v ./_internal/c-api.h ./edgevox-onnx-go-macos/
  cp -v ./_internal/build_darwin_*.go ./edgevox-onnx-go-macos/

  rm -rf edgevox-onnx-go-macos/lib/x86_64-apple-darwin/lib*
  dst=$(realpath edgevox-onnx-go-macos/lib/x86_64-apple-darwin/)

  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-macosx_10_15_x86_64.whl
  unzip ./edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-macosx_10_15_x86_64.whl

  cp -v edgevox_onnx/lib/*.dylib $dst/

  pushd $dst
  cp -v libonnxruntime.*.dylib libonnxruntime.dylib
  popd

  cd ..
  rm -rf t

  echo "process macos arm64"
  rm -rf edgevox-onnx-go-macos/lib/aarch64-apple-darwin/lib*
  dst=$(realpath edgevox-onnx-go-macos/lib/aarch64-apple-darwin)

  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-macosx_11_0_arm64.whl
  unzip ./edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-macosx_11_0_arm64.whl

  cp -v edgevox_onnx/lib/*.dylib $dst/

  pushd $dst
  cp -v libonnxruntime.*.dylib libonnxruntime.dylib
  popd

  cd ..
  rm -rf t
  echo "------------------------------"
  cd edgevox-onnx-go-macos
  git status
  git add .
  git commit -m "Release v$EDGEVOX_ONNX_VERSION" && \
  git push && \
  git tag v$EDGEVOX_ONNX_VERSION && \
  git push origin v$EDGEVOX_ONNX_VERSION || true
  cd ..
  kick_go_proxy "github.com/k2-fsa/sherpa-onnx-go-macos" "v$SHERPA_ONNX_VERSION"
  rm -rf edgevox-onnx-go-macos
}

function windows() {
  echo "Process windows"
  git clone git@github.com:k2-fsa/sherpa-onnx-go-windows.git
  rm -v ./edgevox-onnx-go-windows/*.go
  cp -v ./edgevox_onnx.go ./edgevox-onnx-go-windows/
  cp -v ./_internal/c-api.h ./edgevox-onnx-go-windows/
  cp -v ./_internal/build_windows_*.go ./edgevox-onnx-go-windows/

  rm -fv edgevox-onnx-go-windows/lib/x86_64-pc-windows-gnu/*
  dst=$(realpath edgevox-onnx-go-windows/lib/x86_64-pc-windows-gnu)
  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-win_amd64.whl
  unzip ./edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-win_amd64.whl

  cp -v edgevox_onnx/lib/*.dll $dst

  cd ..
  rm -rf t

  rm -fv edgevox-onnx-go-windows/lib/i686-pc-windows-gnu/*
  dst=$(realpath edgevox-onnx-go-windows/lib/i686-pc-windows-gnu)
  mkdir t
  cd t
  wget -q https://huggingface.co/csukuangfj2/edgevox-onnx-wheels/resolve/main/cpu/$EDGEVOX_ONNX_VERSION/edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-win32.whl
  unzip ./edgevox_onnx_core-${EDGEVOX_ONNX_VERSION}-py3-none-win32.whl

  cp -v edgevox_onnx/lib/*.dll $dst

  cd ..
  rm -rf t
  echo "------------------------------"
  cd edgevox-onnx-go-windows
  git status
  git add .
  git commit -m "Release v$EDGEVOX_ONNX_VERSION" && \
  git push && \
  git tag v$EDGEVOX_ONNX_VERSION && \
  git push origin v$EDGEVOX_ONNX_VERSION || true
  cd ..
  kick_go_proxy "github.com/k2-fsa/sherpa-onnx-go-windows" "v$SHERPA_ONNX_VERSION"
  rm -rf edgevox-onnx-go-windows
}

function basic() {
  echo "Process edgevox-onnx-go"
  git clone git@github.com:k2-fsa/sherpa-onnx-go.git

  python3 ./generate.py -s ./edgevox_onnx.go -o ./edgevox-onnx-go

  cd edgevox-onnx-go

  # Update go.mod to reference the new platform package versions.
  # The platform packages (linux/macos/windows) have already been published
  # and tagged with v$EDGEVOX_ONNX_VERSION.
  local ver="v$EDGEVOX_ONNX_VERSION"
  sed -i.bak \
    -e "s|github.com/k2-fsa/sherpa-onnx-go-linux .*|github.com/k2-fsa/sherpa-onnx-go-linux $ver|" \
    -e "s|github.com/k2-fsa/sherpa-onnx-go-macos .*|github.com/k2-fsa/sherpa-onnx-go-macos $ver|" \
    -e "s|github.com/k2-fsa/sherpa-onnx-go-windows .*|github.com/k2-fsa/sherpa-onnx-go-windows $ver|" \
    go.mod
  rm -f go.mod.bak

  echo "--- Updated go.mod ---"
  cat go.mod
  echo "--- end go.mod ---"

  # Wait for the Go module proxy to index all three platform packages,
  # then regenerate go.sum. The proxy (proxy.golang.org) may take
  # several minutes after a git tag push before the module is downloadable.
  local pkg
  for pkg in edgevox-onnx-go-linux edgevox-onnx-go-macos edgevox-onnx-go-windows; do
    wait_for_go_proxy "github.com/k2-fsa/$pkg" "$ver"
  done

  # go remove stale go.sum entries, then re-resolve with the new versions
  rm -f go.sum
  run_go_mod_tidy

  echo "--- Updated go.sum ---"
  cat go.sum
  echo "--- end go.sum ---"

  cd ..

  echo "------------------------------"
  cd edgevox-onnx-go
  git status
  git add .
  git commit -m "Release v$EDGEVOX_ONNX_VERSION" && \
    git push && \
    git tag v$EDGEVOX_ONNX_VERSION && \
    git push origin v$EDGEVOX_ONNX_VERSION
  cd ..
  rm -rf edgevox-onnx-go
}

# Publishing order matters:
#   1. Platform packages first (linux, windows, osx) — they have no inter-dependencies
#   2. Wait for Go proxy to index them
#   3. edgevox-onnx-go last — it depends on all three platform packages
linux
windows
osx
basic

rm -fv ~/.ssh/github
