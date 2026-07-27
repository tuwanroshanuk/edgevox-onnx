// scripts/node-addon-api/src/non-streaming-asr.cc
//
// Copyright (c)  2024  Xiaomi Corporation
#include <memory>
#include <sstream>

#include "macros.h"  // NOLINT
#include "napi.h"    // NOLINT
#include "edgevox-onnx/c-api/c-api.h"

// defined in ./streaming-asr.cc
EdgevoxOnnxFeatureConfig GetFeatureConfig(Napi::Object obj);
EdgevoxOnnxHomophoneReplacerConfig GetHomophoneReplacerConfig(Napi::Object obj);

static EdgevoxOnnxOfflineTransducerModelConfig GetOfflineTransducerModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTransducerModelConfig c;
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

static EdgevoxOnnxOfflineParaformerModelConfig GetOfflineParaformerModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineParaformerModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("paraformer") || !obj.Get("paraformer").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("paraformer").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineZipformerCtcModelConfig
GetOfflineZipformerCtcModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineZipformerCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("zipformerCtc") || !obj.Get("zipformerCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("zipformerCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineWenetCtcModelConfig GetOfflineWenetCtcModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineWenetCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("wenetCtc") || !obj.Get("wenetCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("wenetCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig
GetOfflineOmnilingualAsrCtcModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("omnilingual") || !obj.Get("omnilingual").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("omnilingual").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineMedAsrCtcModelConfig GetOfflineMedAsrCtcModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineMedAsrCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("medasr") || !obj.Get("medasr").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("medasr").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineFireRedAsrCtcModelConfig
GetOfflineFireRedAsrCtcModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineFireRedAsrCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("fireRedAsrCtc") || !obj.Get("fireRedAsrCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("fireRedAsrCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineFunASRNanoModelConfig GetOfflineFunAsrNanoModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineFunASRNanoModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("funasrNano") || !obj.Get("funasrNano").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("funasrNano").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder_adaptor, encoderAdaptor);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(llm, llm);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(embedding, embedding);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokenizer, tokenizer);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(system_prompt, systemPrompt);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(user_prompt, userPrompt);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_new_tokens, maxNewTokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(temperature, temperature);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(top_p, topP);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(seed, seed);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(language, language);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(itn, itn);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(hotwords, hotwords);

  return c;
}

static EdgevoxOnnxOfflineQwen3ASRModelConfig GetOfflineQwen3AsrModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineQwen3ASRModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("qwen3Asr") || !obj.Get("qwen3Asr").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("qwen3Asr").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(conv_frontend, convFrontend);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokenizer, tokenizer);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_total_len, maxTotalLen);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_new_tokens, maxNewTokens);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(temperature, temperature);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(top_p, topP);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(seed, seed);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(hotwords, hotwords);

  return c;
}

static EdgevoxOnnxOfflineCohereTranscribeModelConfig
GetOfflineCohereTranscribeModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineCohereTranscribeModelConfig c;
  memset(&c, 0, sizeof(c));
  c.use_punct = 1;
  c.use_itn = 1;

  if (!obj.Has("cohereTranscribe") || !obj.Get("cohereTranscribe").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("cohereTranscribe").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(language, language);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(use_punct, usePunct);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(use_itn, useItn);

  return c;
}

static EdgevoxOnnxOfflineDolphinModelConfig GetOfflineDolphinModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineDolphinModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("dolphin") || !obj.Get("dolphin").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("dolphin").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineNemoEncDecCtcModelConfig GetOfflineNeMoCtcModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineNemoEncDecCtcModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("nemoCtc") || !obj.Get("nemoCtc").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("nemoCtc").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineCanaryModelConfig GetOfflineCanaryModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineCanaryModelConfig c;
  memset(&c, 0, sizeof(c));
  c.use_pnc = 1;  // Align default with JS default

  if (!obj.Has("canary") || !obj.Get("canary").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("canary").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(src_lang, srcLang);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tgt_lang, tgtLang);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(use_pnc, usePnc);

  return c;
}

static EdgevoxOnnxOfflineWhisperModelConfig GetOfflineWhisperModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineWhisperModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("whisper") || !obj.Get("whisper").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("whisper").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(language, language);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(task, task);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(tail_paddings, tailPaddings);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(enable_token_timestamps, enableTokenTimestamps);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(enable_segment_timestamps,
                                enableSegmentTimestamps);

  return c;
}

static EdgevoxOnnxOfflineFireRedAsrModelConfig GetOfflineFireRedAsrModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineFireRedAsrModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("fireRedAsr") || !obj.Get("fireRedAsr").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("fireRedAsr").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoder, decoder);

  return c;
}

static EdgevoxOnnxOfflineMoonshineModelConfig GetOfflineMoonshineModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineMoonshineModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("moonshine") || !obj.Get("moonshine").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("moonshine").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(preprocessor, preprocessor);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(encoder, encoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(uncached_decoder, uncachedDecoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(cached_decoder, cachedDecoder);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(merged_decoder, mergedDecoder);

  return c;
}

static EdgevoxOnnxOfflineTdnnModelConfig GetOfflineTdnnModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineTdnnModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("tdnn") || !obj.Get("tdnn").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("tdnn").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);

  return c;
}

static EdgevoxOnnxOfflineSenseVoiceModelConfig GetOfflineSenseVoiceModelConfig(
    Napi::Object obj) {
  EdgevoxOnnxOfflineSenseVoiceModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("senseVoice") || !obj.Get("senseVoice").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("senseVoice").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(language, language);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(use_itn, useInverseTextNormalization);

  return c;
}

static EdgevoxOnnxOfflineModelConfig GetOfflineModelConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineModelConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("modelConfig") || !obj.Get("modelConfig").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("modelConfig").As<Napi::Object>();

  c.transducer = GetOfflineTransducerModelConfig(o);
  c.paraformer = GetOfflineParaformerModelConfig(o);
  c.nemo_ctc = GetOfflineNeMoCtcModelConfig(o);
  c.whisper = GetOfflineWhisperModelConfig(o);
  c.tdnn = GetOfflineTdnnModelConfig(o);
  c.sense_voice = GetOfflineSenseVoiceModelConfig(o);
  c.moonshine = GetOfflineMoonshineModelConfig(o);
  c.fire_red_asr = GetOfflineFireRedAsrModelConfig(o);
  c.dolphin = GetOfflineDolphinModelConfig(o);
  c.zipformer_ctc = GetOfflineZipformerCtcModelConfig(o);
  c.canary = GetOfflineCanaryModelConfig(o);
  c.wenet_ctc = GetOfflineWenetCtcModelConfig(o);
  c.omnilingual = GetOfflineOmnilingualAsrCtcModelConfig(o);
  c.medasr = GetOfflineMedAsrCtcModelConfig(o);
  c.funasr_nano = GetOfflineFunAsrNanoModelConfig(o);
  c.fire_red_asr_ctc = GetOfflineFireRedAsrCtcModelConfig(o);
  c.qwen3_asr = GetOfflineQwen3AsrModelConfig(o);
  c.cohere_transcribe = GetOfflineCohereTranscribeModelConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(tokens, tokens);
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
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model_type, modelType);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(modeling_unit, modelingUnit);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(bpe_vocab, bpeVocab);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(telespeech_ctc, teleSpeechCtc);

  return c;
}

static EdgevoxOnnxOfflineLMConfig GetOfflineLMConfig(Napi::Object obj) {
  EdgevoxOnnxOfflineLMConfig c;
  memset(&c, 0, sizeof(c));

  if (!obj.Has("lmConfig") || !obj.Get("lmConfig").IsObject()) {
    return c;
  }

  Napi::Object o = obj.Get("lmConfig").As<Napi::Object>();

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(model, model);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(scale, scale);

  return c;
}

static EdgevoxOnnxOfflineRecognizerConfig ParseConfig(Napi::Object o) {
  EdgevoxOnnxOfflineRecognizerConfig c;
  memset(&c, 0, sizeof(c));
  c.feat_config = GetFeatureConfig(o);
  c.model_config = GetOfflineModelConfig(o);
  c.lm_config = GetOfflineLMConfig(o);
  c.hr = GetHomophoneReplacerConfig(o);

  EDGEVOX_ONNX_ASSIGN_ATTR_STR(decoding_method, decodingMethod);
  EDGEVOX_ONNX_ASSIGN_ATTR_INT32(max_active_paths, maxActivePaths);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(hotwords_file, hotwordsFile);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(hotwords_score, hotwordsScore);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fsts, ruleFsts);
  EDGEVOX_ONNX_ASSIGN_ATTR_STR(rule_fars, ruleFars);
  EDGEVOX_ONNX_ASSIGN_ATTR_FLOAT(blank_penalty, blankPenalty);

  return c;
}

static void FreeConfig(const EdgevoxOnnxOfflineRecognizerConfig &c) {
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.transducer.joiner);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.paraformer.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.nemo_ctc.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.whisper.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.whisper.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.whisper.language);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.whisper.task);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.tdnn.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.sense_voice.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.sense_voice.language);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.moonshine.preprocessor);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.moonshine.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.moonshine.uncached_decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.moonshine.cached_decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.moonshine.merged_decoder);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.fire_red_asr.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.fire_red_asr.decoder);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.dolphin.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.zipformer_ctc.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.canary.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.canary.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.canary.src_lang);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.canary.tgt_lang);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.wenet_ctc.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.omnilingual.model);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.medasr.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.hotwords);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.language);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.user_prompt);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.system_prompt);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.tokenizer);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.embedding);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.llm);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.funasr_nano.encoder_adaptor);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.qwen3_asr.conv_frontend);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.qwen3_asr.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.qwen3_asr.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.qwen3_asr.tokenizer);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.qwen3_asr.hotwords);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.cohere_transcribe.encoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.cohere_transcribe.decoder);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.cohere_transcribe.language);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.fire_red_asr_ctc.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.tokens);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.provider);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.model_type);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.modeling_unit);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.bpe_vocab);
  EDGEVOX_ONNX_DELETE_C_STR(c.model_config.telespeech_ctc);

  EDGEVOX_ONNX_DELETE_C_STR(c.lm_config.model);

  EDGEVOX_ONNX_DELETE_C_STR(c.decoding_method);
  EDGEVOX_ONNX_DELETE_C_STR(c.hotwords_file);
  EDGEVOX_ONNX_DELETE_C_STR(c.rule_fsts);
  EDGEVOX_ONNX_DELETE_C_STR(c.rule_fars);
  EDGEVOX_ONNX_DELETE_C_STR(c.hr.lexicon);
  EDGEVOX_ONNX_DELETE_C_STR(c.hr.rule_fsts);
}

static Napi::External<EdgevoxOnnxOfflineRecognizer>
CreateOfflineRecognizerWrapper(const Napi::CallbackInfo &info) {
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

  EdgevoxOnnxOfflineRecognizerConfig c = ParseConfig(o);

#if __OHOS__
  const EdgevoxOnnxOfflineRecognizer *recognizer = nullptr;

  if (use_resource_manager) {
    std::unique_ptr<NativeResourceManager,
                    decltype(&OH_ResourceManager_ReleaseNativeResourceManager)>
        mgr(OH_ResourceManager_InitNativeResourceManager(env, info[1]),
            &OH_ResourceManager_ReleaseNativeResourceManager);

    recognizer = EdgevoxOnnxCreateOfflineRecognizerOHOS(&c, mgr.get());
  } else {
    recognizer = EdgevoxOnnxCreateOfflineRecognizer(&c);
  }
#else
  const EdgevoxOnnxOfflineRecognizer *recognizer =
      EdgevoxOnnxCreateOfflineRecognizer(&c);
#endif

  FreeConfig(c);

  if (!recognizer) {
    Napi::TypeError::New(env, "Please check your config!")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxOfflineRecognizer>::New(
      env, const_cast<EdgevoxOnnxOfflineRecognizer *>(recognizer),
      [](Napi::Env env, EdgevoxOnnxOfflineRecognizer *recognizer) {
        EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
      });
}

class CreateRecognizerAsyncWorker : public Napi::AsyncWorker {
 public:
  CreateRecognizerAsyncWorker(const Napi::Env &env,
                              const EdgevoxOnnxOfflineRecognizerConfig &cfg,
                              const Napi::Promise::Deferred &deferred)
      : Napi::AsyncWorker(env), cfg_(cfg), deferred_(deferred) {}

  void Execute() override {
    recognizer_ = EdgevoxOnnxCreateOfflineRecognizer(&cfg_);
    FreeConfig(cfg_);

    if (!recognizer_) {
      SetError("Failed to create offline recognizer");
    }
  }

  void OnOK() override {
    Napi::Env env = Env();

    deferred_.Resolve(Napi::External<EdgevoxOnnxOfflineRecognizer>::New(
        env, const_cast<EdgevoxOnnxOfflineRecognizer *>(recognizer_),
        [](Napi::Env /*env*/, EdgevoxOnnxOfflineRecognizer *r) {
          EdgevoxOnnxDestroyOfflineRecognizer(r);
        }));
  }

  void OnError(const Napi::Error &e) override { deferred_.Reject(e.Value()); }

 private:
  EdgevoxOnnxOfflineRecognizerConfig cfg_;
  const EdgevoxOnnxOfflineRecognizer *recognizer_ = nullptr;
  Napi::Promise::Deferred deferred_;
};

Napi::Value CreateOfflineRecognizerAsyncWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Expected config object")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  EdgevoxOnnxOfflineRecognizerConfig cfg =
      ParseConfig(info[0].As<Napi::Object>());

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  auto *worker = new CreateRecognizerAsyncWorker(env, cfg, deferred);
  worker->Queue();

  return deferred.Promise();
}

static Napi::External<EdgevoxOnnxOfflineStream> CreateOfflineStreamWrapper(
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
        env,
        "You should pass an offline recognizer pointer as the first argument")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOfflineRecognizer>>().Data();

  const EdgevoxOnnxOfflineStream *stream = nullptr;
  if (info.Length() == 2) {
    // Optional per-stream hotwords for contextual biasing. Note that only
    // transducer models decoded with modified_beam_search support hotwords.
    if (!info[1].IsString()) {
      Napi::TypeError::New(env, "Argument 2 should be a string.")
          .ThrowAsJavaScriptException();

      return {};
    }

    std::string hotwords = info[1].As<Napi::String>().Utf8Value();
    stream =
        EdgevoxOnnxCreateOfflineStreamWithHotwords(recognizer, hotwords.c_str());
  } else {
    stream = EdgevoxOnnxCreateOfflineStream(recognizer);
  }

  if (!stream) {
    Napi::TypeError::New(env,
                         "Failed to create offline stream. Please check if "
                         "your model and decoding method support hotwords.")
        .ThrowAsJavaScriptException();

    return {};
  }

  return Napi::External<EdgevoxOnnxOfflineStream>::New(
      env, const_cast<EdgevoxOnnxOfflineStream *>(stream),
      [](Napi::Env env, EdgevoxOnnxOfflineStream *stream) {
        EdgevoxOnnxDestroyOfflineStream(stream);
      });
}

static void AcceptWaveformOfflineWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOfflineStream *stream =
      info[0].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

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
  // Note(fangjun): For unknown reasons on HarmonyOS, we need to divide it by
  // sizeof(float) here
  EdgevoxOnnxAcceptWaveformOffline(stream, sample_rate, samples.Data(),
                                  samples.ElementLength() / sizeof(float));
#else
  EdgevoxOnnxAcceptWaveformOffline(stream, sample_rate, samples.Data(),
                                  samples.ElementLength());
#endif
}

static void OfflineStreamSetOptionWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 3) {
    std::ostringstream os;
    os << "Expect 3 arguments. Given: " << info.Length();
    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an offline stream pointer.")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[1].IsString() || !info[2].IsString()) {
    Napi::TypeError::New(env, "Arguments 1 and 2 should be strings.")
        .ThrowAsJavaScriptException();
    return;
  }

  const EdgevoxOnnxOfflineStream *stream =
      info[0].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

  std::string key = info[1].As<Napi::String>().Utf8Value();
  std::string value = info[2].As<Napi::String>().Utf8Value();
  EdgevoxOnnxOfflineStreamSetOption(stream, key.c_str(), value.c_str());
}

static void OfflineRecognizerSetConfigWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an offline recognizer pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "Expect an object as the second argument")
        .ThrowAsJavaScriptException();

    return;
  }

  Napi::Object o = info[1].As<Napi::Object>();
  EdgevoxOnnxOfflineRecognizerConfig c = ParseConfig(o);

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOfflineRecognizer>>().Data();

  EdgevoxOnnxOfflineRecognizerSetConfig(recognizer, &c);

  FreeConfig(c);
}

class DecodeOfflineStreamAsyncWorker : public Napi::AsyncWorker {
 public:
  DecodeOfflineStreamAsyncWorker(Napi::Env env,
                                 const EdgevoxOnnxOfflineRecognizer *recognizer,
                                 const EdgevoxOnnxOfflineStream *stream,
                                 Napi::Promise::Deferred deferred)
      : Napi::AsyncWorker(env),
        recognizer_(recognizer),
        stream_(stream),
        deferred_(deferred) {}

  void Execute() override {
    try {
      EdgevoxOnnxDecodeOfflineStream(recognizer_, stream_);
    } catch (const std::exception &e) {
      SetError(e.what());
    }
  }

  void OnOK() override {
    const char *json = EdgevoxOnnxGetOfflineStreamResultAsJson(stream_);
    Napi::String s = Napi::String::New(Env(), json);
    EdgevoxOnnxDestroyOfflineStreamResultJson(json);
    deferred_.Resolve(s);
  }

  void OnError(const Napi::Error &e) override { deferred_.Reject(e.Value()); }

 private:
  const EdgevoxOnnxOfflineRecognizer *recognizer_;
  const EdgevoxOnnxOfflineStream *stream_;
  Napi::Promise::Deferred deferred_;
};

static Napi::Value DecodeOfflineStreamAsyncWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect 2 arguments. Given: " << info.Length();
    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsExternal() || !info[1].IsExternal()) {
    Napi::TypeError::New(env,
                         "Expected recognizer and stream as external pointers")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOfflineRecognizer>>().Data();

  const EdgevoxOnnxOfflineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

  // no need to free worker by ourselves
  auto worker =
      new DecodeOfflineStreamAsyncWorker(env, recognizer, stream, deferred);

  worker->Queue();

  return deferred.Promise();
}

static void DecodeOfflineStreamWrapper(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 2) {
    std::ostringstream os;
    os << "Expect only 2 arguments. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return;
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env,
                         "Argument 0 should be an offline recognizer pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  if (!info[1].IsExternal()) {
    Napi::TypeError::New(env, "Argument 1 should be an offline stream pointer.")
        .ThrowAsJavaScriptException();

    return;
  }

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      info[0].As<Napi::External<EdgevoxOnnxOfflineRecognizer>>().Data();

  const EdgevoxOnnxOfflineStream *stream =
      info[1].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

  EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
}

static Napi::String GetOfflineStreamResultAsJsonWrapper(
    const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    std::ostringstream os;
    os << "Expect only 1 argument. Given: " << info.Length();

    Napi::TypeError::New(env, os.str()).ThrowAsJavaScriptException();

    return {};
  }

  if (!info[0].IsExternal()) {
    Napi::TypeError::New(env, "Argument 0 should be an online stream pointer.")
        .ThrowAsJavaScriptException();

    return {};
  }

  const EdgevoxOnnxOfflineStream *stream =
      info[0].As<Napi::External<EdgevoxOnnxOfflineStream>>().Data();

  const char *json = EdgevoxOnnxGetOfflineStreamResultAsJson(stream);
  Napi::String s = Napi::String::New(env, json);

  EdgevoxOnnxDestroyOfflineStreamResultJson(json);

  return s;
}

void InitNonStreamingAsr(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createOfflineRecognizer"),
              Napi::Function::New(env, CreateOfflineRecognizerWrapper));

  exports.Set(Napi::String::New(env, "createOfflineRecognizerAsync"),
              Napi::Function::New(env, CreateOfflineRecognizerAsyncWrapper));

  exports.Set(Napi::String::New(env, "createOfflineStream"),
              Napi::Function::New(env, CreateOfflineStreamWrapper));

  exports.Set(Napi::String::New(env, "acceptWaveformOffline"),
              Napi::Function::New(env, AcceptWaveformOfflineWrapper));

  exports.Set(Napi::String::New(env, "offlineStreamSetOption"),
              Napi::Function::New(env, OfflineStreamSetOptionWrapper));

  exports.Set(Napi::String::New(env, "decodeOfflineStream"),
              Napi::Function::New(env, DecodeOfflineStreamWrapper));

  exports.Set(Napi::String::New(env, "decodeOfflineStreamAsync"),
              Napi::Function::New(env, DecodeOfflineStreamAsyncWrapper));

  exports.Set(Napi::String::New(env, "offlineRecognizerSetConfig"),
              Napi::Function::New(env, OfflineRecognizerSetConfigWrapper));

  exports.Set(Napi::String::New(env, "getOfflineStreamResultAsJson"),
              Napi::Function::New(env, GetOfflineStreamResultAsJsonWrapper));
}
