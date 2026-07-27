//go:build windows && 386

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/i686-pc-windows-gnu -ledgevox-onnx-c-api -lonnxruntime
import "C"
