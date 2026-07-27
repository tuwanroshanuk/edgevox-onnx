// edgevox-onnx/jni/version.cc
//
// Copyright (c)  2025  Xiaomi Corporation
#include "edgevox-onnx/csrc/version.h"

#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL
Java_com_k2fsa_edgevox_onnx_VersionInfo_00024Companion_getVersionStr2(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetVersionStr());
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL
Java_com_k2fsa_edgevox_onnx_VersionInfo_00024Companion_getGitSha12(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetGitSha1());
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL
Java_com_k2fsa_edgevox_onnx_VersionInfo_00024Companion_getGitDate2(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetGitDate());
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL Java_com_k2fsa_edgevox_onnx_VersionInfo_getVersionStr2(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetVersionStr());
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL Java_com_k2fsa_edgevox_onnx_VersionInfo_getGitSha12(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetGitSha1());
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL Java_com_k2fsa_edgevox_onnx_VersionInfo_getGitDate2(
    JNIEnv *env, jclass /*cls*/) {
  return SafeNewStringUTF(env, GetGitDate());
}

}  // namespace edgevox_onnx
