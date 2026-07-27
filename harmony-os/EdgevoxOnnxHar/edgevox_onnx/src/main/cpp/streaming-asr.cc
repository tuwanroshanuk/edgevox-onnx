// scripts/node-addon-api/src/streaming-asr.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include <memory>
#include <sstream>
#include <string>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"
/*
{
  'featConfig': {
    'sampleRate': 16000,
    'featureDim': 80,
  }
};
 */
EdgevoxOnnxFeatureConfig GetFeatureConfig(Napi::Object obj) {
  EdgevoxOnnxFeatureConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("featConfig") || !obj.Get("featConfig").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("featConfig").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(sample_rate, sampleRate);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(feature_dim, featureDim);

  return c;
}
/*
{
  'transducer': {
    'encoder': './encoder.onnx',
    'decoder': './decoder.onnx',
    'joiner': './joiner.onnx',
  }
}
 */

static EdgevoxOnnxOnlineTransducerModelConfig GetOnlineTransducerModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOnlineTransducerModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("transducer") || !obj.Get("transducer").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("transducer").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(joiner, joiner);

  return c;
}

static EdgevoxOnnxOnlineZipformer2CtcModelConfig
GetOnlineZipformer2CtcModelConfig(Napi::Object obj) {
  EdgevoxOnnxOnlineZipformer2CtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("zipformer2Ctc") || !obj.Get("zipformer2Ctc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("zipformer2Ctc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOnlineNemoCtcModelConfig GetOnlineNemoCtcModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOnlineNemoCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("nemoCtc") || !obj.Get("nemoCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("nemoCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOnlineToneCtcModelConfig GetOnlineToneCtcModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOnlineToneCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("toneCtc") || !obj.Get("toneCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("toneCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOnlineParaformerModelConfig GetOnlineParaformerModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOnlineParaformerModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("paraformer") || !obj.Get("paraformer").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("paraformer").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);

  return c;
}

EdgevoxOnnxOnlineModelConfig GetOnlineModelConfig(Napi::Object obj) {
  EdgevoxOnnxOnlineModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("modelConfig") || !obj.Get("modelConfig").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("modelConfig").As<Napi::Object>();

  c.transducer = GetOnlineTransducerModelConfig(o);
  c.paraformer = GetOnlineParaformerModelConfig(o);
  c.zipformer2_ctc = GetOnlineZipformer2CtcModelConfig(o);
  c.nemo_ctc = GetOnlineNemoCtcModelConfig(o);
  c.t_one_ctc = GetOnlineToneCtcModelConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_threads, numThreads);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(provider, provider);

  if (o.Has("debug") &&
      (o.Get("debug").IsNumber() || o.Get("debug").IsBoolean())) {
    if (o.Get("debug").IsBoolean()) {
      c.debug = o.Get("debug").As<Napi::Boolean>().Value();
    } else {
      c.debug = o.Get("debug").As<Napi::Number>().Int32Value();
    }
  }

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model_type, modelType);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(modeling_unit, modelingUnit);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(bpe_vocab, bpeVocab);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens_buf, tokensBuf);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(tokens_buf_size, tokensBufSize);

  return c;
}

static EdgevoxOnnxOnlineCtcFstDecoderConfig GetCtcFstDecoderConfig(
    Napi::Object obj) {
  EdgevoxOnnxOnlineCtcFstDecoderConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("ctcFstDecoderConfig") ||
      !obj.Get("ctcFstDecoderConfig").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("ctcFstDecoderConfig").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(graph, graph);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_active, maxActive);

  return c;
}

// Also used in ./non-streaming-asr.cc
EdgevoxOnnxHomophoneReplacerConfig GetHomophoneReplacerConfig(Napi::Object obj) {
  EdgevoxOnnxHomophoneReplacerConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("hr") || !obj.Get("hr").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("hr").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lexicon, lexicon);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fsts, ruleFsts);

  return c;
}

static Napi::External<EdgevoxOnnxOnlineRecognizer> CreateOnlineRecognizerWrapper(
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
  EdgevoxOnnxOnlineRecognizerConfig c;
  memset(&c, 0, sizeof(c));
  c.feat_config = GetFeatureConfig(o);
  c.model_config = GetOnlineModelConfig(o);
  c.hr = GetHomophoneReplacerConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoding_method, decodingMethod);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_active_paths, maxActivePaths);

  // enableEndpoint can be either a boolean or an integer
  if (o.Has("enableEndpoint") && (o.Get("enableEndpoint").IsNumber() ||
                                  o.Get("enableEndpoint").IsBoolean())) {
    if (o.Get("enableEndpoint").IsNumber()) {
      c.enable_endpoint =
          o.Get("enableEndpoint").As<Napi::Number>().Int32Value();
    } else {
      c.enable_endpoint = o.Get("enableEndpoint").As<Napi::Boolean>().Value();
    }
  }

  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(rule1_min_trailing_silence,
                                rule1MinTrailingSilence);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(rule2_min_trailing_silence,
                                rule2MinTrailingSilence);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(rule3_min_utterance_length,
                                rule3MinUtteranceLength);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(hotwords_file, hotwordsFile);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(hotwords_score, hotwordsScore);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fsts, ruleFsts);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fars, ruleFars);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(blank_penalty, blankPenalty);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(hotwords_buf, hotwordsBuf);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(hotwords_buf_size, hotwordsBufSize);

  c.ctc_fst_decoder_config = GetCtcFstDecoderConfig(o);

#if __OHOS__
  const EdgevoxOnnxOnlineRecognizer *recognizer = nullptr;

  if (use_resource_manager) {
    std::unique_ptr<NativeResourceManager,
                    decltype(&OH_ResourceManager_ReleaseNativeResourceManager)>
        mgr(OH_ResourceManager_InitNativeResourceManager(env, info[1]),
            &OH_ResourceManager_ReleaseNativeResourceManager);

    recognizer = EdgevoxOnnxCreateOnlineRecognizerOHOS(&c, mgr.get());
  } else {
    recognizer = EdgevoxOnnxCreateOnlineRecognizer(&c);
  }
#else
  const EdgevoxOnnxOnlineRecognizer *recognizer =
      EdgevoxOnnxCreateOnlineRecognizer(&c);
#endif
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.joiner);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.paraformer.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.paraformer.decoder);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.t_one_ctc.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.nemo_ctc.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.zipformer2_ctc.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.tokens);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.provider);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.model_type);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.modeling_unit);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.bpe_vocab);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.tokens_buf);
  EDGEVOX_ONNX_DELETE_C_STR(c.decoding_method);
  EDGEVOX_ONNX_DELETE_C_STR(c.hotwords_file);
  EDGEVOX_ONNX_DELETE_C_STR(c.rule_fsts);
  EDGEVOX_ONNX_DELETE_C_STR(c.rule_fars);
  EDGEVOX_ONNX_DELETE_C_STR(c.hotwords_buf);
  EDGEVOX_ONNX_DELETE_C_STR(c.ctc_fst_decoder_config.graph);

  EDGEVOX_ONNX_DELETE_C_STR(c.hr.lexicon);
  EDGEVOX_ONNX_DELETE_C_STR(c.hr.rule_fsts);

  if (!recognizer) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxOnlineRecognizer>::New(
      env, const_cast<EdgevoxOnnxOnlineRecognizer *>(recognizer),
      [](Napi::Env env, EdgevoxOnnxOnlineRecognizer *recognizer) {
        EdgevoxOnnxDestroyOnlineRecognizer(recognizer);
      });
}

static Napi::External<EdgevoxOnnxOnlineStream> CreateOnlineStreamWrapper(
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
        "You should pass an online recognizer pointer as the only argument")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      EdgevoxOnnxCreateOnlineStream(recognizer);

  return Napi::External<EdgevoxOnnxOnlineStream>::New(
      env, const_cast<EdgevoxOnnxOnlineStream *>(stream),
      [](Napi::Env env, EdgevoxOnnxOnlineStream *stream) {
        EdgevoxOnnxDestroyOnlineStream(stream);
      });
}

static void AcceptWaveformWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOnlineStream *stream =
      info[0].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Argument 1 should be an object")
        .ThrowAsJavaScriptException();

    return;
  }

  Napi::Object obj = info[1].As<Napi::Object>();

  if (!obj.Has("samples")) {
    Napi::TypeError::New(env, "The argument object should have a field samples")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!obj.Get("samples").IsTypedArray()) {
    Napi::TypeError::New(env, "The object['samples'] should be a typed array")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!obj.Has("sampleRate")) {
    Napi::TypeError::New(env,
                         "The argument object should have a field sampleRate")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!obj.Get("sampleRate").IsNumber()) {
    Napi::TypeError::New(env, "The object['samples'] should be a number")
        .ThrowAsJavaScriptException();

    return;
  }

  Napi::Float32Array samples = obj.Get("samples").As<Napi::Float32Array>();
  int32_t sample_rate = obj.Get("sampleRate").As<Napi::Number>().Int32Value();

#if __OHOS__
  EdgevoxOnnxOnlineStreamAcceptWaveform(stream, sample_rate, samples.Data(),
                                       samples.ElementLength() / sizeof(float));
#else
  EdgevoxOnnxOnlineStreamAcceptWaveform(stream, sample_rate, samples.Data(),
                                       samples.ElementLength());
#endif
}

static Napi::Boolean IsOnlineStreamReadyWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an online recognizer pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  int32_t is_ready = EdgevoxOnnxIsOnlineStreamReady(recognizer, stream);

  return Napi::Boolean::New(env, is_ready);
}

static void DecodeOnlineStreamWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an online recognizer pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  EdgevoxOnnxDecodeOnlineStream(recognizer, stream);
}

static Napi::String GetOnlineStreamResultAsJsonWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an online recognizer pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  const char *json = EdgevoxOnnxGetOnlineStreamResultAsJson(recognizer, stream);
  Napi::String s = Napi::String::New(env, json);

  EdgevoxOnnxDestroyOnlineStreamResultJson(json);

  return s;
}

static void InputFinishedWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOnlineStream *stream =
      info[0].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  EdgevoxOnnxOnlineStreamInputFinished(stream);
}

static void ResetOnlineStreamWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an online recognizer pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  EdgevoxOnnxOnlineStreamReset(recognizer, stream);
}

static Napi::Boolean IsEndpointWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an online recognizer pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOnlineRecognizer>>().Data();

  const EdgevoxOnnxOnlineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOnlineStream>>().Data();

  int32_t is_endpoint = EdgevoxOnnxOnlineStreamIsEndpoint(recognizer, stream);

  return Napi::Boolean::New(env, is_endpoint);
}

static Napi::External<EdgevoxOnnxDisplay> CreateDisplayWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Expect a number as the argument")
        .ThrowAsJavaScriptException();

    return {};
  }
  int32_t max_word_per_line = info[0].As<Napi::Number>().Int32Value();

  const EdgevoxOnnxDisplay *display = EdgevoxOnnxCreateDisplay(max_word_per_line);

  return Napi::External<EdgevoxOnnxDisplay>::New(
      env, const_cast<EdgevoxOnnxDisplay *>(display),
      [](Napi::Env env, EdgevoxOnnxDisplay *display) {
        EdgevoxOnnxDestroyDisplay(display);
      });
}

static void PrintWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    std::ostringstream os;
    os << "Expect only 3 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Argument 1 should be a number.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[2].IsString()) {
    Napi::TypeError::New(env, "Argument 2 should be a string.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxDisplay *display =
      info[0].As<Napi::External<EdgevoxOnnxDisplay>>().Data();

  int32_t idx = info[1].As<Napi::Number>().Int32Value();

  Napi::String text = info[2].As<Napi::String>();
  std::string s = text.Utf8Value();
  EdgevoxOnnxPrint(display, idx, s.c_str());
}

void InitStreamingAsr(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createOnlineRecognizer"),
              Napi::Function::New(env, CreateOnlineRecognizerWrapper));

  exports.Set(Napi::String::New(env, "createOnlineStream"),
              Napi::Function::New(env, CreateOnlineStreamWrapper));

  exports.Set(Napi::String::New(env, "acceptWaveformOnline"),
              Napi::Function::New(env, AcceptWaveformWrapper));

  exports.Set(Napi::String::New(env, "isOnlineStreamReady"),
              Napi::Function::New(env, IsOnlineStreamReadyWrapper));

  exports.Set(Napi::String::New(env, "decodeOnlineStream"),
              Napi::Function::New(env, DecodeOnlineStreamWrapper));

  exports.Set(Napi::String::New(env, "getOnlineStreamResultAsJson"),
              Napi::Function::New(env, GetOnlineStreamResultAsJsonWrapper));

  exports.Set(Napi::String::New(env, "inputFinished"),
              Napi::Function::New(env, InputFinishedWrapper));

  exports.Set(Napi::String::New(env, "reset"),
              Napi::Function::New(env, ResetOnlineStreamWrapper));

  exports.Set(Napi::String::New(env, "isEndpoint"),
              Napi::Function::New(env, IsEndpointWrapper));

  exports.Set(Napi::String::New(env, "createDisplay"),
              Napi::Function::New(env, CreateDisplayWrapper));

  exports.Set(Napi::String::New(env, "print"),
              Napi::Function::New(env, PrintWrapper));
}
