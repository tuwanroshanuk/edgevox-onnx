//go:build darwin && amd64 && !ios

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/x86_64-apple-darwin -ledgevox-onnx-c-api -lonnxruntime -Wl,-rpath,${SRCDIR}/lib/x86_64-apple-darwin
import "C"
