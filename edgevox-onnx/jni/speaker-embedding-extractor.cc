// edgevox-onnx/jni/speaker-embedding-extractor.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include "edgevox-onnx/csrc/speaker-embedding-extractor.h"

#include <memory>
#include <vector>

#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

static SpeakerEmbeddingExtractorConfig GetSpeakerEmbeddingExtractorConfig(
    JNIEnv *env, jobject config, bool *ok) {
  SpeakerEmbeddingExtractorConfig ans;

  jclass cls = env->GetObjectClass(config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model, model, cls, config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.num_threads, numThreads, cls, config);

  EDGEVOX_ONNX_JNI_READ_BOOL(ans.debug, debug, cls, config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.provider, provider, cls, config);

  *ok = true;

  return ans;
}

}  // namespace edgevox_onnx

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_newFromAsset(
    JNIEnv *env, jobject /*obj*/, jobject asset_manager, jobject _config) {
#if __ANDROID_API__ >= 9
  AAssetManager *mgr = AAssetManager_fromJava(env, asset_manager);
  if (!mgr) {
    EDGEVOX_ONNX_LOGE("Failed to get asset manager: %p", mgr);
    return 0;
  }
#endif
  bool ok = false;
  auto config =
      edgevox_onnx::GetSpeakerEmbeddingExtractorConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("new config:\n%s", config.ToString().c_str());

  auto extractor = new edgevox_onnx::SpeakerEmbeddingExtractor(
#if __ANDROID_API__ >= 9
      mgr,
#endif
      config);

  return (jlong)extractor;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_newFromFile(
    JNIEnv *env, jobject /*obj*/, jobject _config) {
  bool ok = false;
  auto config =
      edgevox_onnx::GetSpeakerEmbeddingExtractorConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("newFromFile config:\n%s", config.ToString().c_str());

  if (!config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors found in config!");
    return 0;
  }

  auto extractor = new edgevox_onnx::SpeakerEmbeddingExtractor(config);

  return (jlong)extractor;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_delete(JNIEnv * /*env*/,
                                                            jobject /*obj*/,
                                                            jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::SpeakerEmbeddingExtractor *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_createStream(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  std::unique_ptr<edgevox_onnx::OnlineStream> s =
      reinterpret_cast<edgevox_onnx::SpeakerEmbeddingExtractor *>(ptr)
          ->CreateStream();

  // The user is responsible to free the returned pointer.
  //
  // See Java_com_nexus_edgevox_onnx_OnlineStream_delete() from
  // ./online-stream.cc
  edgevox_onnx::OnlineStream *p = s.release();
  return (jlong)p;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jboolean JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_isReady(JNIEnv * /*env*/,
                                                             jobject /*obj*/,
                                                             jlong ptr,
                                                             jlong stream_ptr) {
  auto extractor =
      reinterpret_cast<edgevox_onnx::SpeakerEmbeddingExtractor *>(ptr);
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(stream_ptr);
  return extractor->IsReady(stream);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jfloatArray JNICALL
Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_compute(JNIEnv *env,
                                                             jobject /*obj*/,
                                                             jlong ptr,
                                                             jlong stream_ptr) {
  auto extractor =
      reinterpret_cast<edgevox_onnx::SpeakerEmbeddingExtractor *>(ptr);
  auto stream = reinterpret_cast<edgevox_onnx::OnlineStream *>(stream_ptr);

  std::vector<float> embedding = extractor->Compute(stream);
  jfloatArray embedding_arr = env->NewFloatArray(embedding.size());
  env->SetFloatArrayRegion(embedding_arr, 0, embedding.size(),
                           embedding.data());
  return embedding_arr;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jint JNICALL Java_com_nexus_edgevox_onnx_SpeakerEmbeddingExtractor_dim(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  auto extractor =
      reinterpret_cast<edgevox_onnx::SpeakerEmbeddingExtractor *>(ptr);
  return extractor->Dim();
}
