// edgevox-onnx/csrc/ort-env.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ORT_ENV_H_
#define EDGEVOX_ONNX_CSRC_ORT_ENV_H_

#include "onnxruntime_cxx_api.h"  // NOLINT

namespace edgevox_onnx {

// Create an Ort::Env with appropriate threading configuration.
// In WASM builds, onnxruntime's default thread pool creation can cause
// abort() from background pthreads. Using CreateEnvWithGlobalThreadPools
// with single-threaded pools avoids this. Per-session threading is
// configured separately via session.cc SetIntraOpNumThreads.
inline Ort::Env CreateOrtEnv() {
#if EDGEVOX_ONNX_ENABLE_WASM
  Ort::ThreadingOptions tp;
  auto &api = Ort::GetApi();
  Ort::ThrowOnError(api.SetGlobalIntraOpNumThreads(tp, 1));
  Ort::ThrowOnError(api.SetGlobalInterOpNumThreads(tp, 1));
  OrtEnv *env = nullptr;
  Ort::ThrowOnError(api.CreateEnvWithGlobalThreadPools(
      ORT_LOGGING_LEVEL_ERROR, "edgevox-onnx", tp, &env));
  return Ort::Env(env);
#else
  return Ort::Env(ORT_LOGGING_LEVEL_ERROR);
#endif
}

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ORT_ENV_H_
