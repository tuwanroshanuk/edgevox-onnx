//go:build linux && arm64

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/aarch64-unknown-linux-gnu -ledgevox-onnx-c-api -lonnxruntime -Wl,-rpath,${SRCDIR}/lib/aarch64-unknown-linux-gnu
import "C"
