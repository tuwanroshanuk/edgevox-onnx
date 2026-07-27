// edgevox-onnx/jni/online-stream.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include "edgevox-onnx/csrc/online-stream.h"

#include "edgevox-onnx/jni/common.h"

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_delete(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_acceptWaveform(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jfloatArray samples,
    jint sample_rate) {
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);

  jfloat *p = env->GetFloatArrayElements(samples, nullptr);
  jsize n = env->GetArrayLength(samples);
  stream->AcceptWaveform(sample_rate, p, n);
  env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_inputFinished(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);
  stream->InputFinished();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_setOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key, jstring value) {
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  const char *p_value = env->GetStringUTFChars(value, nullptr);
  stream->SetOption(p_key, p_value);
  env->ReleaseStringUTFChars(key, p_key);
  env->ReleaseStringUTFChars(value, p_value);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_getOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key) {
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  const std::string &value = stream->GetOption(p_key);
  env->ReleaseStringUTFChars(key, p_key);
  return SafeNewStringUTF(env, value);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jboolean JNICALL Java_com_nexus_edgevox_onnx_OnlineStream_hasOption(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jstring key) {
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(ptr);
  const char *p_key = env->GetStringUTFChars(key, nullptr);
  jboolean result = stream->HasOption(p_key);
  env->ReleaseStringUTFChars(key, p_key);
  return result;
}
