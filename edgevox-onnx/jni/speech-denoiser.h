// edgevox-onnx/jni/speech-denoiser.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_JNI_SPEECH_DENOISER_H_
#define EDGEVOX_ONNX_JNI_SPEECH_DENOISER_H_

#include "edgevox-onnx/csrc/offline-speech-denoiser.h"
#include "edgevox-onnx/csrc/online-speech-denoiser.h"
#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

OfflineSpeechDenoiserModelConfig GetOfflineSpeechDenoiserModelConfig(
    JNIEnv *env, jobject model, bool *ok);

OfflineSpeechDenoiserConfig GetOfflineSpeechDenoiserConfig(
    JNIEnv *env, jobject config, bool *ok);

OnlineSpeechDenoiserConfig GetOnlineSpeechDenoiserConfig(
    JNIEnv *env, jobject config, bool *ok);

jobject NewDenoisedAudio(JNIEnv *env, const DenoisedAudio &denoised);

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_JNI_SPEECH_DENOISER_H_
