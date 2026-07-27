// scripts/node-addon-api/src/keyword-spotting.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include <memory>
#include <sstream>
#include <string>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"

// defined ./streaming-asr.cc
EdgevoxOnnxFeatureConfig GetFeatureConfig(Napi::Object obj);

// defined ./streaming-asr.cc
EdgevoxOnnxOnlineModelConfig GetOnlineModelConfig(Napi::Object obj);

static Napi::External<EdgevoxOnnxKeywordSpotter> CreateKeywordSpotterWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
#if __OHOS__
  if (info.Length() != 1 && info.Length() != 2) {
    std::ostringstream os;
    os << "Expect 1 or 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }
#else
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }
#endif
  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "Expect an object as the argument")
        .ThrowAsJavaScriptException();

    return {};
  }

#if __OHOS__
  bool use_resource_manager =
      info.Length() == 2 && !info[1].IsUndefined() && !info[1].IsNull();
  if (use_resource_manager && !info[1].IsObject()) {
    Napi::TypeError::New(
        env, "You should pass a resource manager as the second argument.")
        .ThrowAsJavaScriptException();

    return {};
  }
#endif

  Napi::Object o = info[0].As<Napi::Object>();
  EdgevoxOnnxKeywordSpotterConfig c;
  memset(&c, 0, sizeof(c));
  c.feat_config = GetFeatureConfig(o);
  c.model_config = GetOnlineModelConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_active_paths, maxActivePaths);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_trailing_blanks, numTrailingBlanks);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(keywords_score, keywordsScore);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(keywords_threshold, keywordsThreshold);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(keywords_file, keywordsFile);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(keywords_buf, keywordsBuf);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(keywords_buf_size, keywordsBufSize);

#if __OHOS__
  const EdgevoxOnnxKeywordSpotter *kws = nullptr;

  if (use_resource_manager) {
    std::unique_ptr<NativeResourceManager,
                    decltype(&OH_ResourceManager_ReleaseNativeResourceManager)>
        mgr(OH_ResourceManager_InitNativeResourceManager(env, info[1]),
            &OH_ResourceManager_ReleaseNativeResourceManager);

    kws = EdgevoxOnnxCreateKeywordSpotterOHOS(&c, mgr.get());
  } else {
    kws = EdgevoxOnnxCreateKeywordSpotter(&c);
  }
#else
  const EdgevoxOnnxKeywordSpotter *kws = EdgevoxOnnxCreateKeywordSpotter(&c);
#endif

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.joiner);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.paraformer.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.paraformer.decoder);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.zipformer2_ctc.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.tokens);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.provider);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.model_type);
  EDGEVOX_ONNX_DELETE_C_STR(c.keywords_file);
  EDGEVOX_ONNX_DELETE_C_STR(c.keywords_buf);

  if (!kws) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxKeywordSpotter>::New(
      env, const_cast<EdgevoxOnnxKeywordSpotter *>(kws),
      [](Napi::Env env, EdgevoxOnnxKeywordSpotter *kws) {
        EdgevoxOnnxDestroyKeywordSpotter(kws);
      });
}

static Napi::External<EdgevoxOnnxOnlineStream> CreateKeywordStreamWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1 && info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 1 or 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "You should pass a keyword spotter pointer as the only argument")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (info.Length() == 2 && !info[1].IsString()) {
    std::ostringstream os;
    os << "Argument 2 should be a string.";
    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();
    return {};
  }

  const EdgevoxOnnxKeywordSpotter *kws =
      info[0].As<Napi::External<EdgevoxOnnxKeywordSpotter>>().Data();

  const EdgevoxOnnxOnlineStream *stream;
  if (info.Length() == 1) {
    stream = EdgevoxOnnxCreateKeywordStream(kws);
  } else {
    Napi::String js_keywords = info[1].As<Napi::String>();
    std::string keywords = js_keywords.Utf8Value();
    stream = EdgevoxOnnxCreateKeywordStreamWithKeywords(kws, keywords.c_str());
  }

  return Napi::External<EdgevoxOnnxOnlineStream>::New(
      env, const_cast<EdgevoxOnnxOnlineStream *>(stream),
      [](Napi::Env env, EdgevoxOnnxOnlineStream *stream) {
        EdgevoxOnnxDestroyOnlineStream(stream);
      });
}

static Napi::Boolean IsKeywordStreamReadyWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be a keyword spotter pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxKeywordSpotter *kws =
      info[0].As<Napi::External<EdgevoxOnnxKeywordSpotter>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  int32_t is_ready = EdgevoxOnnxIsKeywordStreamReady(kws, stream);

  return Napi::Boolean::New(env, is_ready);
}

static void DecodeKeywordStreamWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be a keyword spotter pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxKeywordSpotter *kws =
      info[0].As<Napi::External<EdgevoxOnnxKeywordSpotter>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  EdgevoxOnnxDecodeKeywordStream(kws, stream);
}

static void ResetKeywordStreamWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be a keyword spotter pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxKeywordSpotter *kws =
      info[0].As<Napi::External<EdgevoxOnnxKeywordSpotter>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  EdgevoxOnnxResetKeywordStream(kws, stream);
}

static Napi::String GetKeywordResultAsJsonWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be a keyword spotter pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxKeywordSpotter *kws =
      info[0].As<Napi::External<EdgevoxOnnxKeywordSpotter>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  const char *json = EdgevoxOnnxGetKeywordResultAsJson(kws, stream);

  Napi::String s = Napi::String::New(env, json);

  EdgevoxOnnxFreeKeywordResultJson(json);

  return s;
}

void InitKeywordSpotting(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createKeywordSpotter"),
              Napi::Function::New(env, CreateKeywordSpotterWrapper));

  exports.Set(Napi::String::New(env, "createKeywordStream"),
              Napi::Function::New(env, CreateKeywordStreamWrapper));

  exports.Set(Napi::String::New(env, "isKeywordStreamReady"),
              Napi::Function::New(env, IsKeywordStreamReadyWrapper));

  exports.Set(Napi::String::New(env, "decodeKeywordStream"),
              Napi::Function::New(env, DecodeKeywordStreamWrapper));

  exports.Set(Napi::String::New(env, "resetKeywordStream"),
              Napi::Function::New(env, ResetKeywordStreamWrapper));

  exports.Set(Napi::String::New(env, "getKeywordResultAsJson"),
              Napi::Function::New(env, GetKeywordResultAsJsonWrapper));
}
