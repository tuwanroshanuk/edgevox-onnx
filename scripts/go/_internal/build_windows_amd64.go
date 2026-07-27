//go:build windows && amd64

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/x86_64-pc-windows-gnu -ledgevox-onnx-c-api -lonnxruntime
import "C"
