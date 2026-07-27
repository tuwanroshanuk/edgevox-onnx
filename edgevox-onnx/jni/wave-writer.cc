// edgevox-onnx/jni/wave-writer.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include "edgevox-onnx/csrc/wave-writer.h"

#include "edgevox-onnx/jni/common.h"

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT bool JNICALL Java_com_k2fsa_edgevox_onnx_WaveWriter_writeWaveToFile(
    JNIEnv *env, jclass /*obj*/, jstring filename, jfloatArray samples,
    jint sample_rate) {
  jfloat *p = env->GetFloatArrayElements(samples, nullptr);
  jsize n = env->GetArrayLength(samples);

  const char *p_filename = env->GetStringUTFChars(filename, nullptr);

  bool ok = edgevox_onnx::WriteWave(p_filename, sample_rate, p, n);

  env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);
  env->ReleaseStringUTFChars(filename, p_filename);

  return ok;
}
