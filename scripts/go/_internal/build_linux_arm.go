//go:build linux && arm && !arm7

package edgevox_onnx

// #cgo LDFLAGS: -L ${SRCDIR}/lib/arm-unknown-linux-gnueabihf -ledgevox-onnx-c-api -lonnxruntime -Wl,-rpath,${SRCDIR}/lib/arm-unknown-linux-gnueabihf
import "C"
