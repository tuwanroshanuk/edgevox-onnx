//go:build darwin && arm64 && !ios

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/aarch64-apple-darwin -ledgevox-onnx-c-api -lonnxruntime -Wl,-rpath,${SRCDIR}/lib/aarch64-apple-darwin
import "C"
