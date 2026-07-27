// scripts/node-addon-api/src/non-streaming-tts.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include <algorithm>
#include <atomic>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"

#define EDGEVOX_ONNX_ASSIGN_TTS_ATTR()                                  \
  do {                                                                 \
    EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fsts, ruleFsts);                  \
    EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_num_sentences, maxNumSentences); \
    EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fars, ruleFars);                  \
    EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(silence_scale, silenceScale);        \
  } while (0)

#define EDGEVOX_ONNX_DELETE_TTS_C_STR()                          \
  do {                                                          \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.vits.model);               \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.vits.lexicon);             \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.vits.tokens);              \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.vits.data_dir);            \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.matcha.acoustic_model);    \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.matcha.vocoder);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.matcha.lexicon);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.matcha.tokens);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.matcha.data_dir);          \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kitten.model);             \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kitten.voices);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kitten.tokens);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kitten.data_dir);          \
                                                                 \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.tokens);          \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.encoder);         \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.decoder);         \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.vocoder);         \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.data_dir);        \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.zipvoice.lexicon);         \
                                                                 \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.model);             \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.voices);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.tokens);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.data_dir);          \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.lexicon);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.kokoro.lang);              \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.lm_flow);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.lm_main);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.encoder);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.decoder);           \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.text_conditioner);  \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.vocab_json);        \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.pocket.token_scores_json); \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.duration_predictor);  \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.text_encoder);        \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.vector_estimator);    \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.vocoder);             \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.tts_json);            \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.unicode_indexer);     \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.supertonic.voice_style);         \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.model.provider);                 \
                                                                \
    EDGEVOX_ONNX_DELETE_C_STR(c.rule_fsts);                      \
    EDGEVOX_ONNX_DELETE_C_STR(c.rule_fars);                      \
  } while (0)

#define EDGEVOX_ONNX_DELETE_GENERATION_C_STR(c)  \
  do {                                          \
    EDGEVOX_ONNX_DELETE_C_STR(c.reference_text); \
    EDGEVOX_ONNX_DELETE_C_STR(c.extra);          \
    if (c.reference_audio) {                    \
      delete[] c.reference_audio;               \
    }                                           \
  } while (0)

static std::string JsObjectToJson(Napi::Env env, const Napi::Object &obj) {
  Napi::Object json = env.Global().Get("JSON").As<Napi::Object>();
  Napi::Function stringify = json.Get("stringify").As<Napi::Function>();
  return stringify.Call(json, {obj}).As<Napi::String>().Utf8Value();
}

static EdgevoxOnnxGenerationConfig GetGenerationConfig(Napi::Object o) {
  EdgevoxOnnxGenerationConfig c;
  memset(&c, 0, sizeof(c));

  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(silence_scale, silenceScale);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(speed, speed);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(sid, sid);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(num_steps, numSteps);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(reference_text, referenceText);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(reference_sample_rate, referenceSampleRate);

  if (o.Has("referenceAudio") && o.Get("referenceAudio").IsTypedArray()) {
    auto arr = o.Get("referenceAudio").As<Napi::Float32Array>();
    int32_t n = arr.ElementLength();

    if (n > 0) {
      float *buf = new float[n];
      std::copy(arr.Data(), arr.Data() + n, buf);

      c.reference_audio = buf;
      c.reference_audio_len = n;
    }
  }

  if (o.Has("extra") && o.Get("extra").IsObject()) {
    std::string s = JsObjectToJson(o.Env(), o.Get("extra").As<Napi::Object>());

    char *p = new char[s.size() + 1];
    std::copy(s.begin(), s.end(), p);
    p[s.size()] = '\0';

    c.extra = p;
  }

  return c;
}

static EdgevoxOnnxOfflineTtsVitsModelConfig GetOfflineTtsVitsModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsVitsModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("vits") || !obj.Get("vits").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("vits").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lexicon, lexicon);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(data_dir, dataDir);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(noise_scale, noiseScale);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(noise_scale_w, noiseScaleW);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(length_scale, lengthScale);

  return c;
}

static EdgevoxOnnxOfflineTtsMatchaModelConfig GetOfflineTtsMatchaModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsMatchaModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("matcha") || !obj.Get("matcha").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("matcha").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(acoustic_model, acousticModel);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(vocoder, vocoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lexicon, lexicon);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(data_dir, dataDir);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(noise_scale, noiseScale);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(length_scale, lengthScale);

  return c;
}

static EdgevoxOnnxOfflineTtsKokoroModelConfig GetOfflineTtsKokoroModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsKokoroModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("kokoro") || !obj.Get("kokoro").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("kokoro").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(voices, voices);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(data_dir, dataDir);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(length_scale, lengthScale);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lexicon, lexicon);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lang, lang);

  return c;
}

static EdgevoxOnnxOfflineTtsKittenModelConfig GetOfflineTtsKittenModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsKittenModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("kitten") || !obj.Get("kitten").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("kitten").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(voices, voices);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(data_dir, dataDir);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(length_scale, lengthScale);

  return c;
}

static EdgevoxOnnxOfflineTtsZipvoiceModelConfig
GetOfflineTtsZipvoiceModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineTtsZipvoiceModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("zipvoice") || !obj.Get("zipvoice").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("zipvoice").As<Napi::Object>();
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(vocoder, vocoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(data_dir, dataDir);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lexicon, lexicon);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(espeak_voice, espeakVoice);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(feat_scale, featScale);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(t_shift, tShift);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(target_rms, targetRms);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(guidance_scale, guidanceScale);

  return c;
}

static EdgevoxOnnxOfflineTtsPocketModelConfig GetOfflineTtsPocketModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsPocketModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("pocket") || !obj.Get("pocket").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("pocket").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lm_flow, lmFlow);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(lm_main, lmMain);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(text_conditioner, textConditioner);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(vocab_json, vocabJson);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(token_scores_json, tokenScoresJson);

  if (o.Has("voiceEmbeddingCacheCapacity")) {
    c.voice_embedding_cache_capacity =
        o.Get("voiceEmbeddingCacheCapacity").As<Napi::Number>().Int32Value();
  } else {
    c.voice_embedding_cache_capacity = 50;
  }

  return c;
}

static EdgevoxOnnxOfflineTtsSupertonicModelConfig
GetOfflineTtsSupertonicModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineTtsSupertonicModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("supertonic") || !obj.Get("supertonic").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("supertonic").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(duration_predictor, durationPredictor);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(text_encoder, textEncoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(vector_estimator, vectorEstimator);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(vocoder, vocoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tts_json, ttsJson);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(unicode_indexer, unicodeIndexer);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(voice_style, voiceStyle);

  return c;
}

static EdgevoxOnnxOfflineTtsModelConfig GetOfflineTtsModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTtsModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("model") || !obj.Get("model").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("model").As<Napi::Object>();

  c.vits = GetOfflineTtsVitsModelConfig(o);
  c.matcha = GetOfflineTtsMatchaModelConfig(o);
  c.kokoro = GetOfflineTtsKokoroModelConfig(o);
  c.kitten = GetOfflineTtsKittenModelConfig(o);
  c.zipvoice = GetOfflineTtsZipvoiceModelConfig(o);
  c.pocket = GetOfflineTtsPocketModelConfig(o);
  c.supertonic = GetOfflineTtsSupertonicModelConfig(o);

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

  return c;
}

// Async worker for creating OfflineTts
class CreateOfflineTtsAsyncWorker : public Napi::AsyncWorker {
 public:
  CreateOfflineTtsAsyncWorker(Napi::Env env,
                              const EdgevoxOnnxOfflineTtsConfig &config)
      : Napi::AsyncWorker(env),
        deferred_(Napi::Promise::Deferred::New(env)),
        config_(config) {}

  Napi::Promise Promise() { return deferred_.Promise(); }

 protected:
  void Execute() override {
    // Create OfflineTts
    tts_ = EdgevoxOnnxCreateOfflineTts(&config_);
    if (!tts_) {
      SetError("Failed to create OfflineTts. Check your config!");
    }
  }

  void OnOK() override {
    Napi::Env env = Env();
    deferred_.Resolve(Napi::External<EdgevoxOnnxOfflineTts>::New(
        env, const_cast<EdgevoxOnnxOfflineTts *>(tts_),
        [](Napi::Env, EdgevoxOnnxOfflineTts *ptr) {
          EdgevoxOnnxDestroyOfflineTts(ptr);
        }));
  }

  void OnError(const Napi::Error &e) override { deferred_.Reject(e.Value()); }

  ~CreateOfflineTtsAsyncWorker() override {
    EdgevoxOnnxOfflineTtsConfig &c = config_;

    EDGEVOX_ONNX_DELETE_TTS_C_STR();
  }

 private:
  EdgevoxOnnxOfflineTtsConfig config_;
  const EdgevoxOnnxOfflineTts *tts_ = nullptr;
  Napi::Promise::Deferred deferred_;
};

// JS wrapper
static Napi::Value CreateOfflineTtsAsyncWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Expect 1 object argument for config")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object o = info[0].As<Napi::Object>();

  EdgevoxOnnxOfflineTtsConfig c;
  memset(&c, 0, sizeof(c));

  c.model = GetOfflineTtsModelConfig(o);
  EDGEVOX_ONNX_ASSIGN_TTS_ATTR();

  auto *worker = new CreateOfflineTtsAsyncWorker(env, c);
  worker->Queue();
  return worker->Promise();
}

static Napi::External<EdgevoxOnnxOfflineTts> CreateOfflineTtsWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
#if __OHOS__
  // the last argument is the NativeResourceManager
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

  EdgevoxOnnxOfflineTtsConfig c;
  memset(&c, 0, sizeof(c));

  c.model = GetOfflineTtsModelConfig(o);

  EDGEVOX_ONNX_ASSIGN_TTS_ATTR();

#if __OHOS__
  const EdgevoxOnnxOfflineTts *tts = nullptr;

  if (use_resource_manager) {
    std::unique_ptr<NativeResourceManager,
                    decltype(&OH_ResourceManager_ReleaseNativeResourceManager)>
        mgr(OH_ResourceManager_InitNativeResourceManager(env, info[1]),
            &OH_ResourceManager_ReleaseNativeResourceManager);
    tts = EdgevoxOnnxCreateOfflineTtsOHOS(&c, mgr.get());
  } else {
    tts = EdgevoxOnnxCreateOfflineTts(&c);
  }
#else
  const EdgevoxOnnxOfflineTts *tts = EdgevoxOnnxCreateOfflineTts(&c);
#endif

  EDGEVOX_ONNX_DELETE_TTS_C_STR();

  if (!tts) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxOfflineTts>::New(
      env, const_cast<EdgevoxOnnxOfflineTts *>(tts),
      [](Napi::Env env, EdgevoxOnnxOfflineTts *tts) {
        EdgevoxOnnxDestroyOfflineTts(tts);
      });
}

static Napi::Number OfflineTtsSampleRateWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline tts pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();

  int32_t sample_rate = EdgevoxOnnxOfflineTtsSampleRate(tts);

  return Napi::Number::New(env, sample_rate);
}

static Napi::Number OfflineTtsNumSpeakersWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline tts pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();

  int32_t num_speakers = EdgevoxOnnxOfflineTtsNumSpeakers(tts);

  return Napi::Number::New(env, num_speakers);
}

// synchronous version
static Napi::Object OfflineTtsGenerateWithConfigWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::TypeError::New(env, "Expect 2 arguments")
        .ThrowAsJavaScriptException();
    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 must be OfflineTts handle")
        .ThrowAsJavaScriptException();
    return {};
  }

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Argument 1 must be an object")
        .ThrowAsJavaScriptException();
    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();

  Napi::Object obj = info[1].As<Napi::Object>();

  if (!obj.Has("text") || !obj.Get("text").IsString()) {
    Napi::TypeError::New(env, "obj.text must be a string")
        .ThrowAsJavaScriptException();
    return {};
  }

  std::string text = obj.Get("text").As<Napi::String>().Utf8Value();

  bool enable_external_buffer = true;
  if (obj.Has("enableExternalBuffer") &&
      obj.Get("enableExternalBuffer").IsBoolean()) {
    enable_external_buffer =
        obj.Get("enableExternalBuffer").As<Napi::Boolean>().Value();
  }

  Napi::Object genObj =
      obj.Has("generationConfig") && obj.Get("generationConfig").IsObject()
          ? obj.Get("generationConfig").As<Napi::Object>()
          : Napi::Object::New(env);

  EdgevoxOnnxGenerationConfig gen_config = GetGenerationConfig(genObj);

  const EdgevoxOnnxGeneratedAudio *audio =
      EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text.c_str(), &gen_config,
                                             nullptr, nullptr);

  EDGEVOX_ONNX_DELETE_GENERATION_C_STR(gen_config);

  if (!audio) {
    Napi::Error::New(env, "TTS generation failed").ThrowAsJavaScriptException();
    return {};
  }

  Napi::Object result = Napi::Object::New(env);
  int32_t sample_rate = audio->sample_rate;

  if (enable_external_buffer) {
    Napi::ArrayBuffer buffer = Napi::ArrayBuffer::New(
        env, const_cast<float *>(audio->samples), sizeof(float) * audio->n,
        [](Napi::Env, void *, const EdgevoxOnnxGeneratedAudio *hint) {
          EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(hint);
        },
        audio);

    result.Set("samples", Napi::Float32Array::New(env, audio->n, buffer, 0));
  } else {
    Napi::ArrayBuffer buffer =
        Napi::ArrayBuffer::New(env, sizeof(float) * audio->n);

    auto arr = Napi::Float32Array::New(env, audio->n, buffer, 0);

    std::copy(audio->samples, audio->samples + audio->n, arr.Data());

    EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);

    result.Set("samples", arr);
  }

  result.Set("sampleRate", sample_rate);
  return result;
}

static Napi::Object OfflineTtsGenerateWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline tts pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Argument 1 should be an object")
        .ThrowAsJavaScriptException();

    return {};
  }

  Napi::Object obj = info[1].As<Napi::Object>();

  if (!obj.Has("text")) {
    Napi::TypeError::New(env, "The argument object should have a field text")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("text").IsString()) {
    Napi::TypeError::New(env, "The object['text'] should be a string")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Has("sid")) {
    Napi::TypeError::New(env, "The argument object should have a field sid")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("sid").IsNumber()) {
    Napi::TypeError::New(env, "The object['sid'] should be a number")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Has("speed")) {
    Napi::TypeError::New(env, "The argument object should have a field speed")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("speed").IsNumber()) {
    Napi::TypeError::New(env, "The object['speed'] should be a number")
        .ThrowAsJavaScriptException();

    return {};
  }

  bool enable_external_buffer = true;
  if (obj.Has("enableExternalBuffer") &&
      obj.Get("enableExternalBuffer").IsBoolean()) {
    enable_external_buffer =
        obj.Get("enableExternalBuffer").As<Napi::Boolean>().Value();
  }

  Napi::String _text = obj.Get("text").As<Napi::String>();
  std::string text = _text.Utf8Value();
  int32_t sid = obj.Get("sid").As<Napi::Number>().Int32Value();
  float speed = obj.Get("speed").As<Napi::Number>().FloatValue();

  const EdgevoxOnnxGeneratedAudio *audio;
  EdgevoxOnnxGenerationConfig gen_config;
  memset(&gen_config, 0, sizeof(gen_config));
  gen_config.sid = sid;
  gen_config.speed = speed;
  audio = EdgevoxOnnxOfflineTtsGenerateWithConfig(tts, text.c_str(), &gen_config,
                                                 nullptr, nullptr);

  if (enable_external_buffer) {
    Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(
        env, const_cast<float *>(audio->samples), sizeof(float) * audio->n,
        [](Napi::Env /*env*/, void * /*data*/,
           const EdgevoxOnnxGeneratedAudio *hint) {
          EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(hint);
        },
        audio);
    Napi::Float32Array float32Array =
        Napi::Float32Array::New(env, audio->n, arrayBuffer, 0);

    Napi::Object ans = Napi::Object::New(env);
    ans.Set(Napi::String::New(env, "samples"), float32Array);
    ans.Set(Napi::String::New(env, "sampleRate"), audio->sample_rate);
    return ans;
  } else {
    // don't use external buffer
    Napi::ArrayBuffer arrayBuffer =
        Napi::ArrayBuffer::New(env, sizeof(float) * audio->n);

    Napi::Float32Array float32Array =
        Napi::Float32Array::New(env, audio->n, arrayBuffer, 0);

    std::copy(audio->samples, audio->samples + audio->n, float32Array.Data());

    Napi::Object ans = Napi::Object::New(env);
    ans.Set(Napi::String::New(env, "samples"), float32Array);
    ans.Set(Napi::String::New(env, "sampleRate"), audio->sample_rate);
    EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);
    return ans;
  }
}

// Cancellation (JS callback returning 0/false) is best-effort: the producer
// checks the flag before synthesizing the next chunk and already-queued
// chunks are suppressed before reaching JS, but audio synthesized before the
// flag was observed is not rolled back from the final result.
struct TtsCallbackData {
  std::vector<float> samples;
  float progress;
  std::shared_ptr<std::atomic<bool>> cancelled;
  // FIFO drain marker queued by the producer after the last real chunk; it is
  // processed on the main thread with a valid env (unlike the TSFN finalizer,
  // which may run during environment teardown where JS must not execute)
  bool done_sentinel = false;
};

// Settlement state shared between the AsyncWorker (generation_done) and the
// FIFO done-sentinel processed by InvokeJsCallback (callbacks_drained). Both
// signals fire on the main thread with a valid env, so plain fields need no
// synchronization and no JS runs from the TSFN finalizer (unsafe during
// environment teardown). The promise settles only when the SECOND signal
// arrives, guaranteeing every queued progress callback ran — and any error it
// recorded is visible — before resolve/reject.
struct TtsSettleState {
  explicit TtsSettleState(Napi::Env env) : deferred(env) {}
  Napi::Promise::Deferred deferred;
  bool generation_done = false;
  bool callbacks_drained = false;
  bool settled = false;
  bool use_external_buffer = false;
  const EdgevoxOnnxGeneratedAudio *audio = nullptr;
  std::string error;
};

static void SettleIfReady(Napi::Env env,
                          const std::shared_ptr<TtsSettleState> &state) {
  if (state->settled || !state->generation_done || !state->callbacks_drained) {
    return;
  }

  if (!state->error.empty()) {
    if (state->audio) {
      EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(state->audio);
      state->audio = nullptr;
    }
    state->deferred.Reject(Napi::Error::New(env, state->error).Value());
    state->settled = true;
    return;
  }

  if (state->audio == nullptr) {
    state->deferred.Reject(
        Napi::Error::New(env, "TTS generation produced no audio").Value());
    state->settled = true;
    return;
  }

  Napi::Object ans = Napi::Object::New(env);
  if (state->use_external_buffer) {
    const EdgevoxOnnxGeneratedAudio *audio = state->audio;
    Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(
        env, const_cast<float *>(audio->samples), sizeof(float) * audio->n,
        [](Napi::Env /*env*/, void * /*data*/,
           const EdgevoxOnnxGeneratedAudio *hint) {
          EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(hint);
        },
        audio);
    // The ArrayBuffer finalizer owns the audio from here on
    state->audio = nullptr;
    Napi::Float32Array float32Array =
        Napi::Float32Array::New(env, audio->n, arrayBuffer, 0);

    ans.Set(Napi::String::New(env, "samples"), float32Array);
    ans.Set(Napi::String::New(env, "sampleRate"), audio->sample_rate);
  } else {
    Napi::ArrayBuffer arrayBuffer =
        Napi::ArrayBuffer::New(env, sizeof(float) * state->audio->n);

    Napi::Float32Array float32Array =
        Napi::Float32Array::New(env, state->audio->n, arrayBuffer, 0);

    std::copy(state->audio->samples, state->audio->samples + state->audio->n,
              float32Array.Data());

    ans.Set(Napi::String::New(env, "samples"), float32Array);
    ans.Set(Napi::String::New(env, "sampleRate"), state->audio->sample_rate);
    EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(state->audio);
    state->audio = nullptr;
  }

  state->deferred.Resolve(ans);
  state->settled = true;
}

// Settlement must never fail silently: if building the result or settling
// throws, free the audio and reject so the promise cannot stay pending.
static void SettleOrFail(Napi::Env env,
                         const std::shared_ptr<TtsSettleState> &state) {
  try {
    SettleIfReady(env, state);
  } catch (...) {
    if (state->audio) {
      EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(state->audio);
      state->audio = nullptr;
    }
    if (!state->settled) {
      try {
        state->deferred.Reject(
            Napi::Error::New(env, "TTS settlement failed").Value());
        state->settled = true;
      } catch (...) {
      }
    }
  }
}

// Context owned by the TSFN: the JS receiver plus the settlement state, so
// the FIFO done-sentinel processed in InvokeJsCallback can deliver the
// second settle signal (the finalizer only destroys the context).
struct TtsTsfnContext {
  TtsTsfnContext(Napi::Env env, Napi::Value self)
      : self(Napi::Persistent(self)),
        state(std::make_shared<TtsSettleState>(env)) {}
  Napi::Reference<Napi::Value> self;
  std::shared_ptr<TtsSettleState> state;
};

// see
// https://github.com/nodejs/node-addon-examples/blob/main/src/6-threadsafe-function/typed_threadsafe_function/node-addon-api/clock.cc
//
// Ownership contract: each TtsCallbackData is heap-allocated by the producer
// (TTS worker thread) and freed exactly once here, on the main thread, after
// the JS callback runs. N-API guarantees every queued item is delivered to
// this function (with env == nullptr during teardown), so no other cleanup
// path may touch these pointers.
static void InvokeJsCallback(Napi::Env env, Napi::Function callback,
                             TtsTsfnContext *context, TtsCallbackData *data) {
  // RAII so the chunk is freed exactly once even if the JS callback throws
  std::unique_ptr<TtsCallbackData> owned(data);

  if (data->done_sentinel) {
    // Queue is FIFO, so every real chunk was already delivered. During
    // environment teardown (env == nullptr) only the sentinel is freed.
    if (env != nullptr && context != nullptr) {
      context->state->callbacks_drained = true;
      SettleOrFail(env, context->state);
    }
    return;
  }

  if (env == nullptr || callback == nullptr ||
      (data->cancelled && data->cancelled->load())) {
    return;
  }

  Napi::ArrayBuffer arrayBuffer =
      Napi::ArrayBuffer::New(env, sizeof(float) * data->samples.size());

  Napi::Float32Array float32Array =
      Napi::Float32Array::New(env, data->samples.size(), arrayBuffer, 0);

  std::copy(data->samples.begin(), data->samples.end(), float32Array.Data());

  Napi::Object arg = Napi::Object::New(env);
  arg.Set(Napi::String::New(env, "samples"), float32Array);
  arg.Set(Napi::String::New(env, "progress"), data->progress);

  // A JS exception must not unwind through the N-API callback boundary nor be
  // left pending on the env (either corrupts later native calls). A throwing
  // callback cancels the generation and records the error; the settle path
  // waits for the queue to drain, so the rejection is deterministic. Both
  // node-addon-api exception modes are covered.
  bool cancel_requested = false;
  std::string error_message;
  try {
    auto v = callback.Call(context->self.Value(), {arg});

    if (env.IsExceptionPending()) {
      Napi::Error e = env.GetAndClearPendingException();
      error_message = e.Message();
      cancel_requested = true;
    } else if ((v.IsBoolean() && !v.As<Napi::Boolean>().Value()) ||
               (v.IsNumber() && v.As<Napi::Number>().Int32Value() == 0)) {
      cancel_requested = true;
    }
  } catch (const Napi::Error &e) {
    error_message = e.Message();
    cancel_requested = true;
  } catch (...) {
    error_message = "onProgress callback threw";
    cancel_requested = true;
  }

  if (cancel_requested && data->cancelled) {
    data->cancelled->store(true);
  }
  if (!error_message.empty() && context->state->error.empty()) {
    context->state->error = "onProgress callback threw: " + error_message;
  }
}

using TSFN = Napi::TypedThreadSafeFunction<TtsTsfnContext, TtsCallbackData,
                                           InvokeJsCallback>;

// Chunks, not bytes: each entry is one progress callback's samples. The
// producer runs on a worker thread, so a full queue blocks synthesis (never
// the event loop) — backpressure instead of unbounded sample copies. 16
// chunks of ~1s audio is comfortably above any real consumer lag.
constexpr size_t kMaxPendingTtsChunks = 16;

class TtsGenerateWorker : public Napi::AsyncWorker {
 public:
  TtsGenerateWorker(const Napi::Env &env, TSFN tsfn,
                    std::shared_ptr<TtsSettleState> state,
                    const EdgevoxOnnxOfflineTts *tts, const std::string &text,
                    float speed, int32_t sid, bool use_external_buffer)
      : tsfn_(tsfn),
        Napi::AsyncWorker{env, "TtsGenerateWorker"},
        state_(std::move(state)),
        tts_(tts),
        text_(text),
        speed_(speed),
        sid_(sid),
        use_external_buffer_(use_external_buffer) {}

  Napi::Promise Promise() { return state_->deferred.Promise(); }

  ~TtsGenerateWorker() = default;

 protected:
  void Execute() override {
    auto callback = [](const float *samples, int32_t n, float progress,
                       void *arg) -> int32_t {
      TtsGenerateWorker *_this = reinterpret_cast<TtsGenerateWorker *>(arg);

      if (_this->cancelled_->load()) {
#if __OHOS__
        OH_LOG_INFO(LOG_APP, "TtsGenerate is cancelled");
#endif
        return 0;
      }

      auto data = new TtsCallbackData;
      data->samples = std::vector<float>{samples, samples + n};
      data->progress = progress;
      data->cancelled = _this->cancelled_;

      const auto status = _this->tsfn_.BlockingCall(data);
      if (status != napi_ok) {
        delete data;
        if (status == napi_closing) {
          _this->tsfn_closing_ = true;
        }
        return 0;
      }

      return 1;
    };
    EdgevoxOnnxGenerationConfig gen_config;
    memset(&gen_config, 0, sizeof(gen_config));
    gen_config.sid = sid_;
    gen_config.speed = speed_;
    audio_ = EdgevoxOnnxOfflineTtsGenerateWithConfig(
        tts_, text_.c_str(), &gen_config, callback, this);

    if (!tsfn_closing_) {
      auto *done = new TtsCallbackData;
      done->done_sentinel = true;
      const auto status = tsfn_.BlockingCall(done);
      if (status != napi_ok) {
        delete done;
        if (status == napi_closing) {
          tsfn_closing_ = true;
        } else {
          sentinel_failed_ = true;
        }
      }
    }
    if (!tsfn_closing_) {
      tsfn_.Release();
    }
  }

  void OnOK() override {
    state_->generation_done = true;
    state_->audio = audio_;
    state_->use_external_buffer = use_external_buffer_;
    if (sentinel_failed_ || tsfn_closing_) {
      // No drain signal will ever arrive; fail the generation instead of
      // leaving the promise pending forever
      if (state_->error.empty()) {
        state_->error = "TTS progress queue failed";
      }
      state_->callbacks_drained = true;
    }
    SettleOrFail(Env(), state_);
  }

 private:
  TSFN tsfn_;
  std::shared_ptr<TtsSettleState> state_;
  const EdgevoxOnnxOfflineTts *tts_;
  std::string text_;
  float speed_;
  int32_t sid_;
  bool use_external_buffer_;

  const EdgevoxOnnxGeneratedAudio *audio_;

  std::shared_ptr<std::atomic<bool>> cancelled_ =
      std::make_shared<std::atomic<bool>>(false);
  // Worker-thread only: set when BlockingCall observes napi_closing, after
  // which the TSFN must not be touched again (not even Release)
  bool tsfn_closing_ = false;
  // Worker-thread write in Execute, main-thread read in OnOK (sequenced by
  // AsyncWorker): the done-sentinel could not be queued, so OnOK must not
  // wait for a drain signal that will never arrive
  bool sentinel_failed_ = false;
};

static Napi::Object OfflineTtsGenerateAsyncWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline tts pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Argument 1 should be an object")
        .ThrowAsJavaScriptException();

    return {};
  }

  Napi::Object obj = info[1].As<Napi::Object>();

  if (!obj.Has("text")) {
    Napi::TypeError::New(env, "The argument object should have a field text")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("text").IsString()) {
    Napi::TypeError::New(env, "The object['text'] should be a string")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Has("sid")) {
    Napi::TypeError::New(env, "The argument object should have a field sid")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("sid").IsNumber()) {
    Napi::TypeError::New(env, "The object['sid'] should be a number")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Has("speed")) {
    Napi::TypeError::New(env, "The argument object should have a field speed")
        .ThrowAsJavaScriptException();

    return {};
  }

  if (!obj.Get("speed").IsNumber()) {
    Napi::TypeError::New(env, "The object['speed'] should be a number")
        .ThrowAsJavaScriptException();

    return {};
  }

  bool enable_external_buffer = true;
  if (obj.Has("enableExternalBuffer") &&
      obj.Get("enableExternalBuffer").IsBoolean()) {
    enable_external_buffer =
        obj.Get("enableExternalBuffer").As<Napi::Boolean>().Value();
  }

  Napi::String _text = obj.Get("text").As<Napi::String>();
  std::string text = _text.Utf8Value();
  int32_t sid = obj.Get("sid").As<Napi::Number>().Int32Value();
  float speed = obj.Get("speed").As<Napi::Number>().FloatValue();

  Napi::Function cb;
  if (obj.Has("callback") && obj.Get("callback").IsFunction()) {
    cb = obj.Get("callback").As<Napi::Function>();
  }

  auto context = new TtsTsfnContext(env, info.This());

  TSFN tsfn = TSFN::New(
      env,
      cb,                 // JavaScript function called asynchronously
      "TtsGenerateFunc",  // Name
      kMaxPendingTtsChunks,
      1,                  // Only one thread will use this initially
      context,
      [](Napi::Env, void *, TtsTsfnContext *ctx) { delete ctx; });

  TtsGenerateWorker *worker = new TtsGenerateWorker(
      env, tsfn, context->state, tts, text, speed, sid,
      enable_external_buffer);
  worker->Queue();
  return worker->Promise();
}

// Async worker for TTS generation with generationConfig
class TtsGenerateWithConfigWorker : public Napi::AsyncWorker {
 public:
  TtsGenerateWithConfigWorker(const Napi::Env &env, TSFN tsfn,
                              std::shared_ptr<TtsSettleState> state,
                              const EdgevoxOnnxOfflineTts *tts,
                              const std::string &text,
                              const EdgevoxOnnxGenerationConfig &gen_config,
                              bool use_external_buffer)
      : tsfn_(tsfn),
        Napi::AsyncWorker(env, "TtsGenerateWithConfigWorker"),
        state_(std::move(state)),
        tts_(tts),
        text_(text),
        gen_config_(gen_config),
        use_external_buffer_(use_external_buffer) {}

  Napi::Promise Promise() { return state_->deferred.Promise(); }

  ~TtsGenerateWithConfigWorker() {
    EDGEVOX_ONNX_DELETE_GENERATION_C_STR(gen_config_);
  }

 protected:
  void Execute() override {
    auto callback = [](const float *samples, int32_t n, float progress,
                       void *arg) -> int32_t {
      TtsGenerateWithConfigWorker *_this =
          reinterpret_cast<TtsGenerateWithConfigWorker *>(arg);

      if (_this->cancelled_->load()) {
        return 0;
      }

      auto data = new TtsCallbackData;
      data->samples = std::vector<float>{samples, samples + n};
      data->progress = progress;
      data->cancelled = _this->cancelled_;

      const auto status = _this->tsfn_.BlockingCall(data);
      if (status != napi_ok) {
        delete data;
        if (status == napi_closing) {
          _this->tsfn_closing_ = true;
        }
        return 0;
      }

      return 1;
    };

    audio_ = EdgevoxOnnxOfflineTtsGenerateWithConfig(
        tts_, text_.c_str(), &gen_config_, callback, this);

    if (!tsfn_closing_) {
      auto *done = new TtsCallbackData;
      done->done_sentinel = true;
      const auto status = tsfn_.BlockingCall(done);
      if (status != napi_ok) {
        delete done;
        if (status == napi_closing) {
          tsfn_closing_ = true;
        } else {
          sentinel_failed_ = true;
        }
      }
    }
    if (!tsfn_closing_) {
      tsfn_.Release();
    }
  }

  void OnOK() override {
    state_->generation_done = true;
    state_->audio = audio_;
    state_->use_external_buffer = use_external_buffer_;
    if (sentinel_failed_ || tsfn_closing_) {
      // No drain signal will ever arrive; fail the generation instead of
      // leaving the promise pending forever
      if (state_->error.empty()) {
        state_->error = "TTS progress queue failed";
      }
      state_->callbacks_drained = true;
    }
    SettleOrFail(Env(), state_);
  }

 private:
  TSFN tsfn_;
  std::shared_ptr<TtsSettleState> state_;
  const EdgevoxOnnxOfflineTts *tts_;
  std::string text_;
  EdgevoxOnnxGenerationConfig gen_config_;
  bool use_external_buffer_;
  const EdgevoxOnnxGeneratedAudio *audio_;
  std::shared_ptr<std::atomic<bool>> cancelled_ =
      std::make_shared<std::atomic<bool>>(false);
  bool tsfn_closing_ = false;
  bool sentinel_failed_ = false;
};

static Napi::Object OfflineTtsGenerateAsyncWithConfigWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2 || !info[0].IsExternal() || !info[1].IsObject()) {
    Napi::TypeError::New(env, "Expect (External<OfflineTts>, Object)")
        .ThrowAsJavaScriptException();
    return {};
  }

  const EdgevoxOnnxOfflineTts *tts =
      info[0].As<Napi::External<EdgevoxOnnxOfflineTts>>().Data();
  Napi::Object obj = info[1].As<Napi::Object>();

  if (!obj.Has("text") || !obj.Get("text").IsString()) {
    Napi::TypeError::New(env, "obj.text must be a string")
        .ThrowAsJavaScriptException();
    return {};
  }

  std::string text = obj.Get("text").As<Napi::String>().Utf8Value();

  bool enable_external_buffer = true;
  if (obj.Has("enableExternalBuffer") &&
      obj.Get("enableExternalBuffer").IsBoolean()) {
    enable_external_buffer =
        obj.Get("enableExternalBuffer").As<Napi::Boolean>().Value();
  }

  Napi::Function cb;
  if (obj.Has("callback") && obj.Get("callback").IsFunction()) {
    cb = obj.Get("callback").As<Napi::Function>();
  }

  auto context = new TtsTsfnContext(env, info.This());
  TSFN tsfn = TSFN::New(
      env, cb, "TtsGenerateWithConfig", kMaxPendingTtsChunks, 1, context,
      [](Napi::Env, void *, TtsTsfnContext *ctx) { delete ctx; });

  EdgevoxOnnxGenerationConfig gen_config;
  memset(&gen_config, 0, sizeof(gen_config));
  if (obj.Has("generationConfig") && obj.Get("generationConfig").IsObject()) {
    gen_config =
        GetGenerationConfig(obj.Get("generationConfig").As<Napi::Object>());
  }

  TtsGenerateWithConfigWorker *worker = new TtsGenerateWithConfigWorker(
      env, tsfn, context->state, tts, text, gen_config,
      enable_external_buffer);
  worker->Queue();
  return worker->Promise();
}

void InitNonStreamingTts(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createOfflineTts"),
              Napi::Function::New(env, CreateOfflineTtsWrapper));

  exports.Set(Napi::String::New(env, "createOfflineTtsAsync"),
              Napi::Function::New(env, CreateOfflineTtsAsyncWrapper));

  exports.Set(Napi::String::New(env, "getOfflineTtsSampleRate"),
              Napi::Function::New(env, OfflineTtsSampleRateWrapper));

  exports.Set(Napi::String::New(env, "getOfflineTtsNumSpeakers"),
              Napi::Function::New(env, OfflineTtsNumSpeakersWrapper));

  exports.Set(Napi::String::New(env, "offlineTtsGenerate"),
              Napi::Function::New(env, OfflineTtsGenerateWrapper));

  exports.Set(Napi::String::New(env, "offlineTtsGenerateWithConfig"),
              Napi::Function::New(env, OfflineTtsGenerateWithConfigWrapper));

  exports.Set(Napi::String::New(env, "offlineTtsGenerateAsync"),
              Napi::Function::New(env, OfflineTtsGenerateAsyncWrapper));

  exports.Set(
      Napi::String::New(env, "offlineTtsGenerateAsyncWithConfig"),
      Napi::Function::New(env, OfflineTtsGenerateAsyncWithConfigWrapper));
}
