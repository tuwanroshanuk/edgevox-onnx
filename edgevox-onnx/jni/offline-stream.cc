// edgevox-onnx/jni/offline-stream.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-stream.h"

#include "edgevox-onnx/jni/common.h"

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OfflineStream_delete(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::OfflineStream *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OfflineStream_acceptWaveform(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jfloatArray samples,
    jint sample_rate) {
  auto stream = reinterpret_cast<edgevox_onnx::OfflineStream *>(ptr);

  jfloat *p = env->GetFloatArrayElements(samples, nullptr);
  jsize n = env->GetArrayLength(samples);
  stream->AcceptWaveform(sample_rate, p, n);
  env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OfflineStream_setOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key, jstring value) {
  auto stream = reinterpret_cast<edgevox_onnx::OfflineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  const char *p_value = env->GetStringUTFChars(value, nullptr);
  stream->SetOption(p_key, p_value);
  env->ReleaseStringUTFChars(key, p_key);
  env->ReleaseStringUTFChars(value, p_value);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL Java_com_k2fsa_edgevox_onnx_OfflineStream_getOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key) {
  auto stream = reinterpret_cast<edgevox_onnx::OfflineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  const std::string &value = stream->GetOption(p_key);
  env->ReleaseStringUTFChars(key, p_key);
  return SafeNewStringUTF(env, value);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jboolean JNICALL Java_com_k2fsa_edgevox_onnx_OfflineStream_hasOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key) {
  auto stream = reinterpret_cast<edgevox_onnx::OfflineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  jboolean result = stream->HasOption(p_key);
  env->ReleaseStringUTFChars(key, p_key);
  return result;
}
