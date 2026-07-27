// edgevox-onnx/jni/offline-diacritization.cc
//
// Copyright (c)  2026  Matias Lin

#include "edgevox-onnx/csrc/offline-diacritization.h"

#include <string>

#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/jni/common.h"

namespace edgevox_onnx {

static OfflineDiacritizationConfig GetOfflineDiacritizationConfig(
    JNIEnv *env, jobject config, bool *ok) {
  OfflineDiacritizationConfig ans;

  jclass cls = env->GetObjectClass(config);
  jfieldID fid;

  fid = env->GetFieldID(
      cls, "model", "Lcom/k2fsa/sherpa/onnx/OfflineDiacritizationModelConfig;");
  jobject model_config = env->GetObjectField(config, fid);
  jclass model_config_cls = env->GetObjectClass(model_config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.catt_encoder, cattEncoder,
                              model_config_cls, model_config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.catt_decoder, cattDecoder,
                              model_config_cls, model_config);

  EDGEVOX_ONNX_JNI_READ_INT(ans.model.num_threads, numThreads, model_config_cls,
                           model_config);

  EDGEVOX_ONNX_JNI_READ_BOOL(ans.model.debug, debug, model_config_cls,
                            model_config);

  EDGEVOX_ONNX_JNI_READ_STRING(ans.model.provider, provider, model_config_cls,
                              model_config);

  *ok = true;
  return ans;
}

}  // namespace edgevox_onnx

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_k2fsa_edgevox_onnx_OfflineDiacritization_newFromAsset(
    JNIEnv *env, jobject /*obj*/, jobject asset_manager, jobject _config) {
#if __ANDROID_API__ >= 9
  AAssetManager *mgr = AAssetManager_fromJava(env, asset_manager);
  if (!mgr) {
    EDGEVOX_ONNX_LOGE("Failed to get asset manager: %p", mgr);
    return 0;
  }
#endif
  bool ok = false;
  auto config = edgevox_onnx::GetOfflineDiacritizationConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

  auto model = new edgevox_onnx::OfflineDiacritization(
#if __ANDROID_API__ >= 9
      mgr,
#endif
      config);

  return (jlong)model;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jlong JNICALL
Java_com_k2fsa_edgevox_onnx_OfflineDiacritization_newFromFile(JNIEnv *env,
                                                             jobject /*obj*/,
                                                             jobject _config) {
  bool ok = false;
  auto config = edgevox_onnx::GetOfflineDiacritizationConfig(env, _config, &ok);

  if (!ok) {
    EDGEVOX_ONNX_LOGE("Please read the error message carefully");
    return 0;
  }

  EDGEVOX_ONNX_LOGE("config:\n%s", config.ToString().c_str());

  if (!config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors found in config!");
    return 0;
  }

  auto model = new edgevox_onnx::OfflineDiacritization(config);

  return (jlong)model;
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT void JNICALL Java_com_k2fsa_edgevox_onnx_OfflineDiacritization_delete(
    JNIEnv * /*env*/, jobject /*obj*/, jlong ptr) {
  delete reinterpret_cast<edgevox_onnx::OfflineDiacritization *>(ptr);
}

EDGEVOX_ONNX_EXTERN_C
JNIEXPORT jstring JNICALL
Java_com_k2fsa_edgevox_onnx_OfflineDiacritization_addDiacritics(JNIEnv *env,
                                                               jobject /*obj*/,
                                                               jlong ptr,
                                                               jstring text) {
  auto diacrt =
      reinterpret_cast<const edgevox_onnx::OfflineDiacritization *>(ptr);

  const char *ptext = env->GetStringUTFChars(text, nullptr);

  std::string result = diacrt->AddDiacritics(ptext);

  env->ReleaseStringUTFChars(text, ptext);

  return SafeNewStringUTF(env, result);
}
