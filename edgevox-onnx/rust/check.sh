#!/usr/bin/env bash
set -euo pipefail

echo "=== Building edgevox-onnx ==="
cargo build -p edgevox-onnx

echo "=== Checking code with cargo check ==="
cargo check -p edgevox-onnx

echo "=== Running clippy for lints ==="
cargo clippy -p edgevox-onnx -- -D warnings

echo "=== Running tests ==="
cargo test -p edgevox-onnx

echo "All checks passed for edgevox-onnx ✅"
