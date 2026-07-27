// edgevox-onnx/jni/audio-tagging.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include "edgevox-onnx/csrc/audio-tagging.h"

#include <memory>
#include <vector>

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

static AudioTaggingConfig GetAudioTaggingConfig(JNIEnv *env, jobject config,
                                                bool *ok) {
  AudioTaggingConfig ans;

  jclass cls = env->GetObjectClass(config);

  jfieldID fid = env->GetFieldID(
      cls, "model", "Lcom/nexus/edgevox/onnx/AudioTaggingModelConfig;");
  jobject model = env->GetObjectField(config, fid);
  jclass model_cls = env->GetObjectClass(model);

  fid = env->GetFieldID(
      model_cls, "zipformer",
      "Lcom/nexus/edgevox/onnx/OfflineZipformerAudioTaggingModelConfig;");
  jobject zipformer = env->GetObjectField(model, fid);
  jclass zipformer_cls = env->GetObjectClass(zipformer);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.zipformer.model, model, zipformer_cls,
                              zipformer);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.ced, ced, model_cls, model);

  EDGEVOX_ONNX_JNI_READ_INT(ans.model.num_threads, numThreads, model_cls, model);

  EDGEVOX_ONNX_JNI_READ_BOOL(ans.model.debug, debug, model_cls, model);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.provider, provider, model_cls, model);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.labels, labels, cls, config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.top_k, topK, cls, config);

  *ok = true;
  return ans;
}

}  // namespace edgevox_onnx

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL Java_com_nexus_edgevox_onnx_AudioTagging_newFromAsset(
    JNIEnv *env, jobject /*obj*/, jobject asset_manager, jobject _config) {
#if __ANDROID_API__ >= 9
  AAssetManager *mgr = AAssetManager_fromJava(env, asset_manager);
  if (!mgr) {
    EDGEVOX_ONNX_LOGE("Failed to get asset manager: %p", mgr);
    return 0;
  }
#endif

  bool ok = false;
  auto config = edgevox_onnx::GetAudioTaggingConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("audio tagging newFromAsset config:\n%s",
                   config.ToString().c_str());

  auto tagger = new edgevox_onnx::AudioTagging(
#if __ANDROID_API__ >= 9
      mgr,
#endif
      config);

  return (jlong)tagger;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL Java_com_nexus_edgevox_onnx_AudioTagging_newFromFile(
    JNIEnv *env, jobject /*obj*/, jobject _config) {
  bool ok = false;

  auto config = edgevox_onnx::GetAudioTaggingConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("audio tagging newFromFile config:\n%s",
                   config.ToString().c_str());

  if (!config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors found in config!");
    return 0;
  }

  auto tagger = new edgevox_onnx::AudioTagging(config);

  return (jlong)tagger;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_nexus_edgevox_onnx_AudioTagging_delete(
    JNIEnv *env, jobject /*obj*/, jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::AudioTagging *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL Java_com_nexus_edgevox_onnx_AudioTagging_createStream(
    JNIEnv *env, jobject /*obj*/, jlong ptr) {
  auto tagger = reinterpret_cast<edgevox_onnx::AudioTagging *>(ptr);
  std::unique_ptr<edgevox_onnx::OfflineStream> s = tagger->CreateStream();

  // The user is responsible to free the returned pointer.
  //
  // See Java_com_nexus_edgevox_onnx_OfflineStream_delete() from
  // ./offline-stream.cc
  edgevox_onnx::OfflineStream *p = s.release();
  return (jlong)p;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jobjectArray JNICALL Java_com_nexus_edgevox_onnx_AudioTagging_compute(
    JNIEnv *env, jobject /*obj*/, jlong ptr, jlong streamPtr, jint top_k) {
  auto tagger = reinterpret_cast<edgevox_onnx::AudioTagging *>(ptr);
  auto stream = reinterpret_cast<edgevox_onnx::OfflineStream *>(streamPtr);
  std::vector<edgevox_onnx::AudioEvent> events = tagger->Compute(stream, top_k);

  // Find the AudioEvent class
  jclass cls = env->FindClass("com/nexus/edgevox/onnx/AudioEvent");
  if (cls == nullptr) {
    EDGEVOX_ONNX_LOGE("Failed to find class com/nexus/edgevox/onnx/AudioEvent");
    return nullptr;
  }

  // Get the constructor: AudioEvent(String name, int index, float prob)
  jmethodID ctor = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;IF)V");
  if (ctor == nullptr) {
    EDGEVOX_ONNX_LOGE("Failed to get AudioEvent constructor");
    env->DeleteLocalRef(cls);
    return nullptr;
  }

  // Create a jobjectArray of AudioEvent
  jobjectArray obj_arr = env->NewObjectArray(events.size(), cls, nullptr);

  for (size_t i = 0; i < events.size(); ++i) {
    const auto &e = events[i];

    jstring name = SafeNewStringUTF(env, e.name);
    jobject event_obj = env->NewObject(cls, ctor, name, e.index, e.prob);

    env->SetObjectArrayElement(obj_arr, i, event_obj);

    env->DeleteLocalRef(name);
    env->DeleteLocalRef(event_obj);
  }

  env->DeleteLocalRef(cls);

  return obj_arr;
}
