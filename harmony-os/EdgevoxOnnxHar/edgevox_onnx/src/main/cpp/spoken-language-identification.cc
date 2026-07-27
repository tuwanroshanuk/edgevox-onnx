// scripts/node-addon-api/src/spoken-language-identification.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include <sstream>
#include <string>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"

static EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig
GetSpokenLanguageIdentificationWhisperConfig(Napi::Object obj) {
  EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("whisper") || !obj.Get("whisper").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("whisper").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(tail_paddings, tailPaddings);

  return c;
}

static Napi::External<EdgevoxOnnxSpokenLanguageIdentification>
CreateSpokenLanguageIdentificationWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "You should pass an object as the only argument.")
        .ThrowAsJavaScriptException();

    return {};
  }

  Napi::Object o = info[0].As<Napi::Object>();

  EdgevoxOnnxSpokenLanguageIdentificationConfig c;
  memset(&c, 0, sizeof(c));
  c.whisper = GetSpokenLanguageIdentificationWhisperConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_threads, numThreads);

  if (o.Has("debug") &&
      (o.Get("debug").IsNumber() || o.Get("debug").IsBoolean())) {
    if (o.Get("debug").IsBoolean()) {
      c.debug = o.Get("debug").As<Napi::Boolean>().Value();
    } else {
      c.debug = o.Get("debug").As<Napi::Number>().Int32Value();
    }
  }
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(provider, provider);

  const EdgevoxOnnxSpokenLanguageIdentification *slid =
      EdgevoxOnnxCreateSpokenLanguageIdentification(&c);

  EDGEVOX_ONNX_DELETE_C_STR(c.whisper.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.whisper.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.provider);

  if (!slid) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxSpokenLanguageIdentification>::New(
      env, const_cast<EdgevoxOnnxSpokenLanguageIdentification *>(slid),
      [](Napi::Env env, EdgevoxOnnxSpokenLanguageIdentification *slid) {
        EdgevoxOnnxDestroySpokenLanguageIdentification(slid);
      });
}

static Napi::External<EdgevoxOnnxOfflineStream>
SpokenLanguageIdentificationCreateOfflineStreamWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env,
        "You should pass an offline language ID pointer as the only argument")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxSpokenLanguageIdentification *slid =
      info[0]
          .As<Napi::External<EdgevoxOnnxSpokenLanguageIdentification>>()
          .Data();

  const EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(slid);

  return Napi::External<EdgevoxOnnxOfflineStream>::New(
      env, const_cast<EdgevoxOnnxOfflineStream *>(stream),
      [](Napi::Env env, EdgevoxOnnxOfflineStream *stream) {
        EdgevoxOnnxDestroyOfflineStream(stream);
      });
}

static Napi::String SpokenLanguageIdentificationComputeWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(
        env, "Argument 0 should be an offline spoken language ID pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an offline stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxSpokenLanguageIdentification *slid =
      info[0]
          .As<Napi::External<EdgevoxOnnxSpokenLanguageIdentification>>()
          .Data();

  const EdgevoxOnnxOfflineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

  const EdgevoxOnnxSpokenLanguageIdentificationResult *r =
      EdgevoxOnnxSpokenLanguageIdentificationCompute(slid, stream);

  std::string lang = r->lang;
  EdgevoxOnnxDestroySpokenLanguageIdentificationResult(r);

  return Napi::String::New(env, lang);
}

void InitSpokenLanguageID(Napi::Env env, Napi::Object exports) {
  exports.Set(
      Napi::String::New(env, "createSpokenLanguageIdentification"),
      Napi::Function::New(env, CreateSpokenLanguageIdentificationWrapper));

  exports.Set(
      Napi::String::New(env, "createSpokenLanguageIdentificationOfflineStream"),
      Napi::Function::New(
          env, SpokenLanguageIdentificationCreateOfflineStreamWrapper));

  exports.Set(
      Napi::String::New(env, "spokenLanguageIdentificationCompute"),
      Napi::Function::New(env, SpokenLanguageIdentificationComputeWrapper));
}
