// edgevox-onnx/csrc/voice-activity-detector.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include "edgevox-onnx/csrc/voice-activity-detector.h"

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

static VadModelConfig GetVadModelConfig(JNIEnv *env, jobject config, bool *ok) {
  VadModelConfig ans;

  jclass cls = env->GetObjectClass(config);
  jfieldID fid;

  // silero_vad
  fid = env->GetFieldID(cls, "sileroVadModelConfig",
                        "Lcom/nexus/edgevox/onnx/SileroVadModelConfig;");
  jobject silero_vad_config = env->GetObjectField(config, fid);
  jclass silero_vad_config_cls = env->GetObjectClass(silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.silero_vad.model, model,
                              silero_vad_config_cls, silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.silero_vad.threshold, threshold,
                             silero_vad_config_cls, silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.silero_vad.min_silence_duration,
                             minSilenceDuration, silero_vad_config_cls,
                             silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.silero_vad.min_speech_duration,
                             minSpeechDuration, silero_vad_config_cls,
                             silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.silero_vad.window_size, windowSize,
                           silero_vad_config_cls, silero_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.silero_vad.max_speech_duration,
                             maxSpeechDuration, silero_vad_config_cls,
                             silero_vad_config);

  fid = env->GetFieldID(cls, "tenVadModelConfig",
                        "Lcom/nexus/edgevox/onnx/TenVadModelConfig;");
  jobject ten_vad_config = env->GetObjectField(config, fid);
  jclass ten_vad_config_cls = env->GetObjectClass(ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.ten_vad.model, model, ten_vad_config_cls,
                              ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.ten_vad.threshold, threshold,
                             ten_vad_config_cls, ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.ten_vad.min_silence_duration,
                             minSilenceDuration, ten_vad_config_cls,
                             ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.ten_vad.min_speech_duration, minSpeechDuration,
                             ten_vad_config_cls, ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.ten_vad.window_size, windowSize,
                           ten_vad_config_cls, ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_FLOAT(ans.ten_vad.max_speech_duration, maxSpeechDuration,
                             ten_vad_config_cls, ten_vad_config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.sample_rate, sampleRate, cls, config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.num_threads, numThreads, cls, config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.provider, provider, cls, config);

  EDGEVOX_ONNX_JNI_READ_BOOL(ans.debug, debug, cls, config);

  *ok = true;
  return ans;
}

}  // namespace edgevox_onnx

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL Java_com_nexus_edgevox_onnx_Vad_newFromAsset(
    JNIEnv *env, jobject /*obj*/, jobject asset_manager, jobject _config) {
#if __ANDROID_API__ >= 9
  AAssetManager *mgr = AAssetManager_fromJava(env, asset_manager);
  if (!mgr) {
    EDGEVOX_ONNX_LOGE("Failed to get asset manager: %p", mgr);
    return 0;
  }
#endif

  bool ok = false;
  auto config = edgevox_onnx::GetVadModelConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

  auto model = new edgevox_onnx::VoiceActivityDetector(
#if __ANDROID_API__ >= 9
      mgr,
#endif
      config);

  return (jlong)model;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL Java_com_nexus_edgevox_onnx_Vad_newFromFile(
    JNIEnv *env, jobject /*obj*/, jobject _config) {
  bool ok = false;
  auto config = edgevox_onnx::GetVadModelConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

  if (!config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors found in config!");
    return 0;
  }

  auto model = new edgevox_onnx::VoiceActivityDetector(config);

  return (jlong)model;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_delete(JNIEnv * /*env*/,
                                                             jobject /*obj*/,
                                                             jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_acceptWaveform(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jfloatArray samples) {
  SafeJNI(env, "Vad_acceptWaveform", [&] {
    if (!ValidatePointer(env, ptr, "Vad_acceptWaveform",
                         "VoiceActivityDetector pointer is null.")) {
      return;
    }

    auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
    jfloat *p = env->GetFloatArrayElements(samples, nullptr);
    jsize n = env->GetArrayLength(samples);

    model->AcceptWaveform(p, n);

    env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);
  });
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jboolean JNICALL Java_com_nexus_edgevox_onnx_Vad_empty(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  return model->Empty();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_pop(JNIEnv * /*env*/,
                                                          jobject /*obj*/,
                                                          jlong ptr) {
  auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  model->Pop();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_clear(JNIEnv * /*env*/,
                                                            jobject /*obj*/,
                                                            jlong ptr) {
  auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  model->Clear();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jobject JNICALL Java_com_nexus_edgevox_onnx_Vad_front(JNIEnv *env,
                                                               jobject /*obj*/,
                                                               jlong ptr) {
  auto vad = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  if (!vad) {
    return nullptr;
  }

  const auto &front = vad->Front();

  jfloatArray samples_arr =
      env->NewFloatArray(static_cast<jsize>(front.samples.size()));

  if (!samples_arr) {
    EDGEVOX_ONNX_LOGE("Failed to allocate");
    return nullptr;
  }

  env->SetFloatArrayRegion(samples_arr, 0,
                           static_cast<jsize>(front.samples.size()),
                           front.samples.data());

  jclass cls = env->FindClass("com/nexus/edgevox/onnx/SpeechSegment");
  if (!cls) {
    EDGEVOX_ONNX_LOGE("Failed to find com/nexus/edgevox/onnx/SpeechSegment");

    env->DeleteLocalRef(samples_arr);
    return nullptr;
  }

  jmethodID ctor = env->GetMethodID(cls, "<init>", "(I[F)V");
  if (!ctor) {
    EDGEVOX_ONNX_LOGE("failed to get constructor");

    env->DeleteLocalRef(samples_arr);
    env->DeleteLocalRef(cls);
    return nullptr;
  }

  jobject speechSegment =
      env->NewObject(cls, ctor, static_cast<jint>(front.start), samples_arr);

  env->DeleteLocalRef(samples_arr);
  env->DeleteLocalRef(cls);

  return speechSegment;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jboolean JNICALL Java_com_nexus_edgevox_onnx_Vad_isSpeechDetected(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  return model->IsSpeechDetected();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_reset(JNIEnv *env,
                                                            jobject /*obj*/,
                                                            jlong ptr) {
  SafeJNI(env, "Vad_reset", [&] {
    if (!ValidatePointer(env, ptr, "Vad_reset",
                         "VoiceActivityDetector pointer is null.")) {
      return;
    }

    auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
    model->Reset();
  });
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_Vad_flush(JNIEnv * /*env*/,
                                                            jobject /*obj*/,
                                                            jlong ptr) {
  auto model = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
  model->Flush();
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jfloat JNICALL Java_com_nexus_edgevox_onnx_Vad_compute(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jfloatArray samples) {
  return SafeJNI(
      env, "Vad_compute",
      [&]() -> jfloat {
        if (!ValidatePointer(env, ptr, "Vad_compute",
                             "VoiceActivityDetector pointer is null.")) {
          return -1.0f;
        }
        auto vad = reinterpret_cast<edgevox_onnx::VoiceActivityDetector *>(ptr);
        jfloat *p = env->GetFloatArrayElements(samples, nullptr);
        jsize n = env->GetArrayLength(samples);

        float score = vad->Compute(p, n);

        env->ReleaseFloatArrayElements(samples, p, JNI_ABORT);

        return static_cast<jfloat>(score);
      },
      -1.0f);
}
