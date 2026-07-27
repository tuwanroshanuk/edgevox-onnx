// edgevox-onnx/jni/online-speech-denoiser.cc
//
// Copyright (c)  2026  Xiaomi Corporation
#include "edgevox-onnx/csrc/online-speech-denoiser.h"

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/jni/common.h"
#include "edgevox-onnx/jni/speech-denoiser.h"

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_newFromAsset(
    JNIEnv *env, jobject /*obj*/, jobject asset_manager, jobject _config) {
#if __ANDROID_API__ >= 9
  AAssetManager *mgr = AAssetManager_fromJava(env, asset_manager);
  if (!mgr) {
    EDGEVOX_ONNX_LOGE("Failed to get asset manager: %p", mgr);
    return 0;
  }
#endif

  bool ok = false;
  auto config = edgevox_onnx::GetOnlineSpeechDenoiserConfig(env, _config, &ok);
  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

  auto speech_denoiser = new edgevox_onnx::OnlineSpeechDenoiser(
#if __ANDROID_API__ >= 9
      mgr,
#endif
      config);

  return reinterpret_cast<jlong>(speech_denoiser);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_newFromFile(JNIEnv *env,
                                                            jobject /*obj*/,
                                                            jobject _config) {
  return SafeJNI(
      env, "OnlineSpeechDenoiser_newFromFile",
      [&]() -> jlong {
        bool ok = false;
        auto config =
            edgevox_onnx::GetOnlineSpeechDenoiserConfig(env, _config, &ok);

        if (!ok) {
          EDGEVOX_ONNX_LOGE("Please read the error message carefully");
          return 0;
        }

        EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

        if (!config.Validate()) {
          EDGEVOX_ONNX_LOGE("Errors found in config!");
          return 0;
        }

        auto speech_denoiser = new edgevox_onnx::OnlineSpeechDenoiser(config);
        return reinterpret_cast<jlong>(speech_denoiser);
      },
      static_cast<jlong>(0));
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_delete(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jint JNICALL
Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_getSampleRate(JNIEnv * /*env*/,
                                                              jobject /*obj*/,
                                                              jlong ptr) {
  return reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr)
      ->GetSampleRate();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jint JNICALL
Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_getFrameShiftInSamples(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  return reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr)
      ->GetFrameShiftInSamples();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jobject JNICALL Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_run(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jfloatArray samples,
    jint sample_rate) {
  auto speech_denoiser =
      reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr);

  jfloat *p = env->GetFloatArrayElements(samples, nullptr);
  jsize n = env->GetArrayLength(samples);
  auto denoised = speech_denoiser->Run(p, n, sample_rate);
  env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);

  return edgevox_onnx::NewDenoisedAudio(env, denoised);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jobject JNICALL Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_flush(
    JNIEnv *env, jobject /*obj*/, jlong ptr) {
  auto speech_denoiser =
      reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr);
  auto denoised = speech_denoiser->Flush();
  return edgevox_onnx::NewDenoisedAudio(env, denoised);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OnlineSpeechDenoiser_reset(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  reinterpret_cast<edgevox_onnx::OnlineSpeechDenoiser *>(ptr)->Reset();
}
