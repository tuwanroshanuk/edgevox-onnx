// edgevox-onnx/c-api/c-api.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/c-api/c-api.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

#if __OHOS__
#include "rawfile/raw_file_manager.h"
#endif

#include "edgevox-onnx/csrc/audio-tagging.h"
#include "edgevox-onnx/csrc/circular-buffer.h"
#include "edgevox-onnx/csrc/display.h"
#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/keyword-spotter.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/offline-diacritization.h"
#include "edgevox-onnx/csrc/offline-punctuation.h"
#include "edgevox-onnx/csrc/offline-recognizer.h"
#include "edgevox-onnx/csrc/offline-source-separation.h"
#include "edgevox-onnx/csrc/offline-speech-denoiser.h"
#include "edgevox-onnx/csrc/online-punctuation.h"
#include "edgevox-onnx/csrc/online-recognizer.h"
#include "edgevox-onnx/csrc/online-speech-denoiser.h"
#include "edgevox-onnx/csrc/resample.h"
#include "edgevox-onnx/csrc/speaker-embedding-extractor.h"
#include "edgevox-onnx/csrc/speaker-embedding-manager.h"
#include "edgevox-onnx/csrc/spoken-language-identification.h"
#include "edgevox-onnx/csrc/text-utils.h"
#include "edgevox-onnx/csrc/version.h"
#include "edgevox-onnx/csrc/voice-activity-detector.h"
#include "edgevox-onnx/csrc/wave-reader.h"
#include "edgevox-onnx/csrc/wave-writer.h"

#if EDGEVOX_ONNX_ENABLE_TTS == 1
#include "edgevox-onnx/csrc/offline-tts.h"
#endif

#if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1
#include "edgevox-onnx/csrc/offline-speaker-diarization.h"
#endif

const char *EdgevoxOnnxGetVersionStr() { return edgevox_onnx::GetVersionStr(); }
const char *EdgevoxOnnxGetGitSha1() { return edgevox_onnx::GetGitSha1(); }
const char *EdgevoxOnnxGetGitDate() { return edgevox_onnx::GetGitDate(); }

struct EdgevoxOnnxOnlineRecognizer {
  std::unique_ptr<edgevox_onnx::OnlineRecognizer> impl;
};

struct EdgevoxOnnxOnlineStream {
  std::unique_ptr<edgevox_onnx::OnlineStream> impl;
  explicit EdgevoxOnnxOnlineStream(std::unique_ptr<edgevox_onnx::OnlineStream> p)
      : impl(std::move(p)) {}
};

struct EdgevoxOnnxDisplay {
  std::unique_ptr<edgevox_onnx::Display> impl;
};

#define EDGEVOX_ONNX_OR(x, y) (x ? x : y)

static edgevox_onnx::OnlineRecognizerConfig GetOnlineRecognizerConfig(
    const EdgevoxOnnxOnlineRecognizerConfig *config) {
  edgevox_onnx::OnlineRecognizerConfig recognizer_config;

  recognizer_config.feat_config.sampling_rate =
      EDGEVOX_ONNX_OR(config->feat_config.sample_rate, 16000);
  recognizer_config.feat_config.feature_dim =
      EDGEVOX_ONNX_OR(config->feat_config.feature_dim, 80);

  recognizer_config.model_config.transducer.encoder =
      EDGEVOX_ONNX_OR(config->model_config.transducer.encoder, "");
  recognizer_config.model_config.transducer.decoder =
      EDGEVOX_ONNX_OR(config->model_config.transducer.decoder, "");
  recognizer_config.model_config.transducer.joiner =
      EDGEVOX_ONNX_OR(config->model_config.transducer.joiner, "");

  recognizer_config.model_config.paraformer.encoder =
      EDGEVOX_ONNX_OR(config->model_config.paraformer.encoder, "");
  recognizer_config.model_config.paraformer.decoder =
      EDGEVOX_ONNX_OR(config->model_config.paraformer.decoder, "");

  recognizer_config.model_config.zipformer2_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.zipformer2_ctc.model, "");

  recognizer_config.model_config.tokens =
      EDGEVOX_ONNX_OR(config->model_config.tokens, "");
  if (config->model_config.tokens_buf &&
      config->model_config.tokens_buf_size > 0) {
    recognizer_config.model_config.tokens_buf = std::string(
        config->model_config.tokens_buf, config->model_config.tokens_buf_size);
  }

  recognizer_config.model_config.nemo_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.nemo_ctc.model, "");

  recognizer_config.model_config.t_one_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.t_one_ctc.model, "");

  recognizer_config.model_config.num_threads =
      EDGEVOX_ONNX_OR(config->model_config.num_threads, 1);
  recognizer_config.model_config.provider_config.provider =
      EDGEVOX_ONNX_OR(config->model_config.provider, "cpu");

  if (recognizer_config.model_config.provider_config.provider.empty()) {
    recognizer_config.model_config.provider_config.provider = "cpu";
  }

  recognizer_config.model_config.model_type =
      EDGEVOX_ONNX_OR(config->model_config.model_type, "");
  recognizer_config.model_config.debug = config->model_config.debug;
  recognizer_config.model_config.modeling_unit =
      EDGEVOX_ONNX_OR(config->model_config.modeling_unit, "cjkchar");

  if (recognizer_config.model_config.modeling_unit.empty()) {
    recognizer_config.model_config.modeling_unit = "cjkchar";
  }

  recognizer_config.model_config.bpe_vocab =
      EDGEVOX_ONNX_OR(config->model_config.bpe_vocab, "");

  recognizer_config.decoding_method =
      EDGEVOX_ONNX_OR(config->decoding_method, "greedy_search");
  if (recognizer_config.decoding_method.empty()) {
    recognizer_config.decoding_method = "greedy_search";
  }

  recognizer_config.max_active_paths =
      EDGEVOX_ONNX_OR(config->max_active_paths, 4);

  recognizer_config.enable_endpoint =
      EDGEVOX_ONNX_OR(config->enable_endpoint, 0);

  recognizer_config.endpoint_config.rule1.min_trailing_silence =
      EDGEVOX_ONNX_OR(config->rule1_min_trailing_silence, 2.4);

  recognizer_config.endpoint_config.rule2.min_trailing_silence =
      EDGEVOX_ONNX_OR(config->rule2_min_trailing_silence, 1.2);

  recognizer_config.endpoint_config.rule3.min_utterance_length =
      EDGEVOX_ONNX_OR(config->rule3_min_utterance_length, 20);

  recognizer_config.hotwords_file = EDGEVOX_ONNX_OR(config->hotwords_file, "");
  recognizer_config.hotwords_score =
      EDGEVOX_ONNX_OR(config->hotwords_score, 1.5);
  if (config->hotwords_buf && config->hotwords_buf_size > 0) {
    recognizer_config.hotwords_buf =
        std::string(config->hotwords_buf, config->hotwords_buf_size);
  }

  recognizer_config.blank_penalty = config->blank_penalty;

  recognizer_config.ctc_fst_decoder_config.graph =
      EDGEVOX_ONNX_OR(config->ctc_fst_decoder_config.graph, "");
  recognizer_config.ctc_fst_decoder_config.max_active =
      EDGEVOX_ONNX_OR(config->ctc_fst_decoder_config.max_active, 3000);

  recognizer_config.rule_fsts = EDGEVOX_ONNX_OR(config->rule_fsts, "");
  recognizer_config.rule_fars = EDGEVOX_ONNX_OR(config->rule_fars, "");

  recognizer_config.hr.lexicon = EDGEVOX_ONNX_OR(config->hr.lexicon, "");
  recognizer_config.hr.rule_fsts = EDGEVOX_ONNX_OR(config->hr.rule_fsts, "");

  if (config->model_config.debug) {
#if __OHOS__
    auto str_vec = edgevox_onnx::SplitString(recognizer_config.ToString(), 128);
    for (const auto &s : str_vec) {
      EDGEVOX_ONNX_LOGE("%{public}s\n", s.c_str());
      EDGEVOX_ONNX_LOGE("%s\n", s.c_str());
    }
#else
    EDGEVOX_ONNX_LOGE("%s", recognizer_config.ToString().c_str());
#endif
  }

  return recognizer_config;
}

const EdgevoxOnnxOnlineRecognizer *EdgevoxOnnxCreateOnlineRecognizer(
    const EdgevoxOnnxOnlineRecognizerConfig *config) {
  edgevox_onnx::OnlineRecognizerConfig recognizer_config =
      GetOnlineRecognizerConfig(config);

  if (!recognizer_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config!");
    return nullptr;
  }

  EdgevoxOnnxOnlineRecognizer *recognizer = new EdgevoxOnnxOnlineRecognizer;

  recognizer->impl =
      std::make_unique<edgevox_onnx::OnlineRecognizer>(recognizer_config);

  return recognizer;
}

void EdgevoxOnnxDestroyOnlineRecognizer(
    const EdgevoxOnnxOnlineRecognizer *recognizer) {
  if (!recognizer) return;
  delete recognizer;
}

const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateOnlineStream(
    const EdgevoxOnnxOnlineRecognizer *recognizer) {
  if (!recognizer) return nullptr;
  EdgevoxOnnxOnlineStream *stream =
      new EdgevoxOnnxOnlineStream(recognizer->impl->CreateStream());
  return stream;
}

const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateOnlineStreamWithHotwords(
    const EdgevoxOnnxOnlineRecognizer *recognizer, const char *hotwords) {
  if (!recognizer) return nullptr;
  EdgevoxOnnxOnlineStream *stream =
      new EdgevoxOnnxOnlineStream(recognizer->impl->CreateStream(hotwords));
  return stream;
}

void EdgevoxOnnxDestroyOnlineStream(const EdgevoxOnnxOnlineStream *stream) {
  if (!stream) return;
  delete stream;
}

void EdgevoxOnnxOnlineStreamAcceptWaveform(const EdgevoxOnnxOnlineStream *stream,
                                          int32_t sample_rate,
                                          const float *samples, int32_t n) {
  if (!stream) return;
  stream->impl->AcceptWaveform(sample_rate, samples, n);
}

int32_t EdgevoxOnnxIsOnlineStreamReady(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream) {
  if (!recognizer || !stream) return 0;
  return recognizer->impl->IsReady(stream->impl.get());
}

void EdgevoxOnnxDecodeOnlineStream(const EdgevoxOnnxOnlineRecognizer *recognizer,
                                  const EdgevoxOnnxOnlineStream *stream) {
  if (!recognizer || !stream) return;
  recognizer->impl->DecodeStream(stream->impl.get());
}

void EdgevoxOnnxDecodeMultipleOnlineStreams(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream **streams, int32_t n) {
  if (!recognizer || !streams) return;
  std::vector<edgevox_onnx::OnlineStream *> ss(n);
  for (int32_t i = 0; i != n; ++i) {
    ss[i] = streams[i]->impl.get();
  }
  recognizer->impl->DecodeStreams(ss.data(), n);
}

const EdgevoxOnnxOnlineRecognizerResult *EdgevoxOnnxGetOnlineStreamResult(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream) {
  if (!recognizer || !stream) return nullptr;
  edgevox_onnx::OnlineRecognizerResult result =
      recognizer->impl->GetResult(stream->impl.get());
  const auto &text = result.text;

  auto r = new EdgevoxOnnxOnlineRecognizerResult;
  memset(r, 0, sizeof(EdgevoxOnnxOnlineRecognizerResult));

  // copy text
  char *pText = new char[text.size() + 1];
  std::copy(text.begin(), text.end(), pText);
  pText[text.size()] = 0;
  r->text = pText;

  // copy json
  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  r->json = pJson;

  // copy tokens
  auto count = result.tokens.size();
  if (count > 0) {
    size_t total_length = 0;
    for (const auto &token : result.tokens) {
      // +1 for the null character at the end of each token
      total_length += token.size() + 1;
    }

    r->count = count;
    // Each word ends with nullptr
    char *tokens = new char[total_length]{};
    char **tokens_temp = new char *[r->count];
    int32_t pos = 0;
    for (int32_t i = 0; i < r->count; ++i) {
      tokens_temp[i] = tokens + pos;
      memcpy(tokens + pos, result.tokens[i].c_str(), result.tokens[i].size());
      // +1 to move past the null character
      pos += result.tokens[i].size() + 1;
    }
    r->tokens_arr = tokens_temp;

    if (!result.timestamps.empty() && result.timestamps.size() == r->count) {
      r->timestamps = new float[r->count];
      std::copy(result.timestamps.begin(), result.timestamps.end(),
                r->timestamps);
    } else {
      r->timestamps = nullptr;
    }

    r->tokens = tokens;
  } else {
    r->count = 0;
    r->timestamps = nullptr;
    r->tokens = nullptr;
    r->tokens_arr = nullptr;
  }

  return r;
}

void EdgevoxOnnxDestroyOnlineRecognizerResult(
    const EdgevoxOnnxOnlineRecognizerResult *r) {
  if (r) {
    delete[] r->text;
    delete[] r->json;
    delete[] r->tokens;
    delete[] r->tokens_arr;
    delete[] r->timestamps;
    delete r;
  }
}

const char *EdgevoxOnnxGetOnlineStreamResultAsJson(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream) {
  edgevox_onnx::OnlineRecognizerResult result =
      recognizer->impl->GetResult(stream->impl.get());
  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  return pJson;
}

void EdgevoxOnnxDestroyOnlineStreamResultJson(const char *s) {
  if (!s) return;
  delete[] s;
}

void EdgevoxOnnxOnlineStreamReset(const EdgevoxOnnxOnlineRecognizer *recognizer,
                                 const EdgevoxOnnxOnlineStream *stream) {
  if (!recognizer || !stream) return;
  recognizer->impl->Reset(stream->impl.get());
}

void EdgevoxOnnxOnlineStreamInputFinished(const EdgevoxOnnxOnlineStream *stream) {
  if (!stream) return;
  stream->impl->InputFinished();
}

void EdgevoxOnnxOnlineStreamSetOption(const EdgevoxOnnxOnlineStream *stream,
                                     const char *key, const char *value) {
  if (!stream || !key || !value) return;
  stream->impl->SetOption(key, value);
}

const char *EdgevoxOnnxOnlineStreamGetOption(
    const EdgevoxOnnxOnlineStream *stream, const char *key) {
  if (!stream || !key) return nullptr;
  return stream->impl->GetOption(key).c_str();
}

int32_t EdgevoxOnnxOnlineStreamHasOption(const EdgevoxOnnxOnlineStream *stream,
                                        const char *key) {
  if (!stream || !key) return 0;
  return stream->impl->HasOption(key);
}

int32_t EdgevoxOnnxOnlineStreamIsEndpoint(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream) {
  if (!recognizer || !stream) return 0;
  return recognizer->impl->IsEndpoint(stream->impl.get());
}

const EdgevoxOnnxDisplay *EdgevoxOnnxCreateDisplay(int32_t max_word_per_line) {
  EdgevoxOnnxDisplay *ans = new EdgevoxOnnxDisplay;
  ans->impl = std::make_unique<edgevox_onnx::Display>(max_word_per_line);
  return ans;
}

void EdgevoxOnnxDestroyDisplay(const EdgevoxOnnxDisplay *display) {
  if (!display) return;
  delete display;
}

void EdgevoxOnnxPrint(const EdgevoxOnnxDisplay *display, int32_t idx,
                     const char *s) {
  display->impl->Print(idx, s);
}

// ============================================================
// For offline ASR (i.e., non-streaming ASR)
// ============================================================
//
struct EdgevoxOnnxOfflineRecognizer {
  std::unique_ptr<edgevox_onnx::OfflineRecognizer> impl;
};

struct EdgevoxOnnxOfflineStream {
  std::unique_ptr<edgevox_onnx::OfflineStream> impl;
  explicit EdgevoxOnnxOfflineStream(
      std::unique_ptr<edgevox_onnx::OfflineStream> p)
      : impl(std::move(p)) {}
};

static edgevox_onnx::OfflineRecognizerConfig GetOfflineRecognizerConfig(
    const EdgevoxOnnxOfflineRecognizerConfig *config) {
  edgevox_onnx::OfflineRecognizerConfig recognizer_config;

  recognizer_config.feat_config.sampling_rate =
      EDGEVOX_ONNX_OR(config->feat_config.sample_rate, 16000);

  recognizer_config.feat_config.feature_dim =
      EDGEVOX_ONNX_OR(config->feat_config.feature_dim, 80);

  recognizer_config.model_config.transducer.encoder_filename =
      EDGEVOX_ONNX_OR(config->model_config.transducer.encoder, "");

  recognizer_config.model_config.transducer.decoder_filename =
      EDGEVOX_ONNX_OR(config->model_config.transducer.decoder, "");

  recognizer_config.model_config.transducer.joiner_filename =
      EDGEVOX_ONNX_OR(config->model_config.transducer.joiner, "");

  recognizer_config.model_config.paraformer.model =
      EDGEVOX_ONNX_OR(config->model_config.paraformer.model, "");

  recognizer_config.model_config.nemo_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.nemo_ctc.model, "");

  recognizer_config.model_config.whisper.encoder =
      EDGEVOX_ONNX_OR(config->model_config.whisper.encoder, "");

  recognizer_config.model_config.whisper.decoder =
      EDGEVOX_ONNX_OR(config->model_config.whisper.decoder, "");

  recognizer_config.model_config.whisper.language =
      EDGEVOX_ONNX_OR(config->model_config.whisper.language, "");

  recognizer_config.model_config.whisper.task =
      EDGEVOX_ONNX_OR(config->model_config.whisper.task, "transcribe");
  if (recognizer_config.model_config.whisper.task.empty()) {
    recognizer_config.model_config.whisper.task = "transcribe";
  }

  recognizer_config.model_config.whisper.tail_paddings =
      EDGEVOX_ONNX_OR(config->model_config.whisper.tail_paddings, -1);

  recognizer_config.model_config.whisper.enable_token_timestamps =
      config->model_config.whisper.enable_token_timestamps;

  recognizer_config.model_config.whisper.enable_segment_timestamps =
      config->model_config.whisper.enable_segment_timestamps;

  recognizer_config.model_config.tdnn.model =
      EDGEVOX_ONNX_OR(config->model_config.tdnn.model, "");

  recognizer_config.model_config.tokens =
      EDGEVOX_ONNX_OR(config->model_config.tokens, "");
  recognizer_config.model_config.num_threads =
      EDGEVOX_ONNX_OR(config->model_config.num_threads, 1);
  recognizer_config.model_config.debug = config->model_config.debug;
  recognizer_config.model_config.provider =
      EDGEVOX_ONNX_OR(config->model_config.provider, "cpu");
  if (recognizer_config.model_config.provider.empty()) {
    recognizer_config.model_config.provider = "cpu";
  }

  recognizer_config.model_config.model_type =
      EDGEVOX_ONNX_OR(config->model_config.model_type, "");
  recognizer_config.model_config.modeling_unit =
      EDGEVOX_ONNX_OR(config->model_config.modeling_unit, "cjkchar");

  if (recognizer_config.model_config.modeling_unit.empty()) {
    recognizer_config.model_config.modeling_unit = "cjkchar";
  }

  recognizer_config.model_config.bpe_vocab =
      EDGEVOX_ONNX_OR(config->model_config.bpe_vocab, "");

  recognizer_config.model_config.telespeech_ctc =
      EDGEVOX_ONNX_OR(config->model_config.telespeech_ctc, "");

  recognizer_config.model_config.sense_voice.model =
      EDGEVOX_ONNX_OR(config->model_config.sense_voice.model, "");

  recognizer_config.model_config.sense_voice.language =
      EDGEVOX_ONNX_OR(config->model_config.sense_voice.language, "");

  recognizer_config.model_config.sense_voice.use_itn =
      config->model_config.sense_voice.use_itn;

  recognizer_config.model_config.moonshine.preprocessor =
      EDGEVOX_ONNX_OR(config->model_config.moonshine.preprocessor, "");

  recognizer_config.model_config.moonshine.encoder =
      EDGEVOX_ONNX_OR(config->model_config.moonshine.encoder, "");

  recognizer_config.model_config.moonshine.uncached_decoder =
      EDGEVOX_ONNX_OR(config->model_config.moonshine.uncached_decoder, "");

  recognizer_config.model_config.moonshine.cached_decoder =
      EDGEVOX_ONNX_OR(config->model_config.moonshine.cached_decoder, "");

  recognizer_config.model_config.moonshine.merged_decoder =
      EDGEVOX_ONNX_OR(config->model_config.moonshine.merged_decoder, "");

  recognizer_config.model_config.fire_red_asr.encoder =
      EDGEVOX_ONNX_OR(config->model_config.fire_red_asr.encoder, "");

  recognizer_config.model_config.fire_red_asr.decoder =
      EDGEVOX_ONNX_OR(config->model_config.fire_red_asr.decoder, "");

  recognizer_config.model_config.dolphin.model =
      EDGEVOX_ONNX_OR(config->model_config.dolphin.model, "");

  recognizer_config.model_config.zipformer_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.zipformer_ctc.model, "");

  recognizer_config.model_config.canary.encoder =
      EDGEVOX_ONNX_OR(config->model_config.canary.encoder, "");

  recognizer_config.model_config.canary.decoder =
      EDGEVOX_ONNX_OR(config->model_config.canary.decoder, "");

  recognizer_config.model_config.canary.src_lang =
      EDGEVOX_ONNX_OR(config->model_config.canary.src_lang, "");

  recognizer_config.model_config.canary.tgt_lang =
      EDGEVOX_ONNX_OR(config->model_config.canary.tgt_lang, "");

  recognizer_config.model_config.canary.use_pnc =
      config->model_config.canary.use_pnc;

  recognizer_config.model_config.cohere_transcribe.encoder =
      EDGEVOX_ONNX_OR(config->model_config.cohere_transcribe.encoder, "");

  recognizer_config.model_config.cohere_transcribe.decoder =
      EDGEVOX_ONNX_OR(config->model_config.cohere_transcribe.decoder, "");

  recognizer_config.model_config.cohere_transcribe.language =
      EDGEVOX_ONNX_OR(config->model_config.cohere_transcribe.language, "");

  recognizer_config.model_config.cohere_transcribe.use_punct =
      config->model_config.cohere_transcribe.use_punct;

  recognizer_config.model_config.cohere_transcribe.use_itn =
      config->model_config.cohere_transcribe.use_itn;

  recognizer_config.model_config.wenet_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.wenet_ctc.model, "");

  recognizer_config.model_config.omnilingual.model =
      EDGEVOX_ONNX_OR(config->model_config.omnilingual.model, "");

  recognizer_config.model_config.medasr.model =
      EDGEVOX_ONNX_OR(config->model_config.medasr.model, "");

  recognizer_config.model_config.funasr_nano.encoder_adaptor =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.encoder_adaptor, "");
  recognizer_config.model_config.funasr_nano.llm =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.llm, "");
  recognizer_config.model_config.funasr_nano.embedding =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.embedding, "");
  recognizer_config.model_config.funasr_nano.tokenizer =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.tokenizer, "");
  recognizer_config.model_config.funasr_nano.system_prompt =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.system_prompt,
                     "You are a helpful assistant.");
  recognizer_config.model_config.funasr_nano.user_prompt = EDGEVOX_ONNX_OR(
      config->model_config.funasr_nano.user_prompt, "语音转写：");
  recognizer_config.model_config.funasr_nano.language =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.language, "");
  recognizer_config.model_config.funasr_nano.itn =
      config->model_config.funasr_nano.itn;
  recognizer_config.model_config.funasr_nano.hotwords =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.hotwords, "");
  recognizer_config.model_config.funasr_nano.max_new_tokens =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.max_new_tokens, 512);
  recognizer_config.model_config.funasr_nano.temperature =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.temperature, 1e-6f);
  recognizer_config.model_config.funasr_nano.top_p =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.top_p, 0.8f);
  recognizer_config.model_config.funasr_nano.seed =
      EDGEVOX_ONNX_OR(config->model_config.funasr_nano.seed, 42);

  recognizer_config.model_config.fire_red_asr_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.fire_red_asr_ctc.model, "");

  recognizer_config.model_config.qwen3_asr.conv_frontend =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.conv_frontend, "");
  recognizer_config.model_config.qwen3_asr.encoder =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.encoder, "");
  recognizer_config.model_config.qwen3_asr.decoder =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.decoder, "");
  recognizer_config.model_config.qwen3_asr.tokenizer =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.tokenizer, "");
  recognizer_config.model_config.qwen3_asr.hotwords =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.hotwords, "");
  recognizer_config.model_config.qwen3_asr.max_total_len =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.max_total_len, 512);
  recognizer_config.model_config.qwen3_asr.max_new_tokens =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.max_new_tokens, 128);
  recognizer_config.model_config.qwen3_asr.temperature =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.temperature, 1e-6f);
  recognizer_config.model_config.qwen3_asr.top_p =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.top_p, 0.8f);
  recognizer_config.model_config.qwen3_asr.seed =
      EDGEVOX_ONNX_OR(config->model_config.qwen3_asr.seed, 42);

  recognizer_config.lm_config.model =
      EDGEVOX_ONNX_OR(config->lm_config.model, "");
  recognizer_config.lm_config.scale =
      EDGEVOX_ONNX_OR(config->lm_config.scale, 1.0);

  recognizer_config.decoding_method =
      EDGEVOX_ONNX_OR(config->decoding_method, "greedy_search");

  if (recognizer_config.decoding_method.empty()) {
    recognizer_config.decoding_method = "greedy_search";
  }

  recognizer_config.max_active_paths =
      EDGEVOX_ONNX_OR(config->max_active_paths, 4);

  recognizer_config.hotwords_file = EDGEVOX_ONNX_OR(config->hotwords_file, "");
  recognizer_config.hotwords_score =
      EDGEVOX_ONNX_OR(config->hotwords_score, 1.5);

  recognizer_config.blank_penalty = config->blank_penalty;

  recognizer_config.rule_fsts = EDGEVOX_ONNX_OR(config->rule_fsts, "");
  recognizer_config.rule_fars = EDGEVOX_ONNX_OR(config->rule_fars, "");

  recognizer_config.hr.lexicon = EDGEVOX_ONNX_OR(config->hr.lexicon, "");
  recognizer_config.hr.rule_fsts = EDGEVOX_ONNX_OR(config->hr.rule_fsts, "");

  if (config->model_config.debug) {
#if __OHOS__
    auto str_vec = edgevox_onnx::SplitString(recognizer_config.ToString(), 128);
    for (const auto &s : str_vec) {
      EDGEVOX_ONNX_LOGE("%{public}s\n", s.c_str());
      EDGEVOX_ONNX_LOGE("%s\n", s.c_str());
    }
#else
    EDGEVOX_ONNX_LOGE("%s", recognizer_config.ToString().c_str());
#endif
  }

  return recognizer_config;
}

const EdgevoxOnnxOfflineRecognizer *EdgevoxOnnxCreateOfflineRecognizer(
    const EdgevoxOnnxOfflineRecognizerConfig *config) {
  edgevox_onnx::OfflineRecognizerConfig recognizer_config =
      GetOfflineRecognizerConfig(config);

  if (!recognizer_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflineRecognizer *recognizer = new EdgevoxOnnxOfflineRecognizer;

  recognizer->impl =
      std::make_unique<edgevox_onnx::OfflineRecognizer>(recognizer_config);

  return recognizer;
}

void EdgevoxOnnxOfflineRecognizerSetConfig(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineRecognizerConfig *config) {
  if (!recognizer || !config) return;
  edgevox_onnx::OfflineRecognizerConfig recognizer_config =
      GetOfflineRecognizerConfig(config);
  recognizer->impl->SetConfig(recognizer_config);
}

void EdgevoxOnnxDestroyOfflineRecognizer(
    const EdgevoxOnnxOfflineRecognizer *recognizer) {
  if (!recognizer) return;
  delete recognizer;
}

const EdgevoxOnnxOfflineStream *EdgevoxOnnxCreateOfflineStream(
    const EdgevoxOnnxOfflineRecognizer *recognizer) {
  if (!recognizer) return nullptr;
  EdgevoxOnnxOfflineStream *stream =
      new EdgevoxOnnxOfflineStream(recognizer->impl->CreateStream());
  return stream;
}

const EdgevoxOnnxOfflineStream *EdgevoxOnnxCreateOfflineStreamWithHotwords(
    const EdgevoxOnnxOfflineRecognizer *recognizer, const char *hotwords) {
  if (!recognizer) return nullptr;
  EdgevoxOnnxOfflineStream *stream =
      new EdgevoxOnnxOfflineStream(recognizer->impl->CreateStream(hotwords));
  return stream;
}

void EdgevoxOnnxDestroyOfflineStream(const EdgevoxOnnxOfflineStream *stream) {
  if (!stream) return;
  delete stream;
}

void EdgevoxOnnxAcceptWaveformOffline(const EdgevoxOnnxOfflineStream *stream,
                                     int32_t sample_rate, const float *samples,
                                     int32_t n) {
  if (!stream) return;
  stream->impl->AcceptWaveform(sample_rate, samples, n);
}

void EdgevoxOnnxOfflineStreamSetOption(const EdgevoxOnnxOfflineStream *stream,
                                      const char *key, const char *value) {
  if (!stream || !key || !value) return;
  stream->impl->SetOption(key, value);
}

const char *EdgevoxOnnxOfflineStreamGetOption(
    const EdgevoxOnnxOfflineStream *stream, const char *key) {
  if (!stream || !key) return nullptr;
  return stream->impl->GetOption(key).c_str();
}

int32_t EdgevoxOnnxOfflineStreamHasOption(const EdgevoxOnnxOfflineStream *stream,
                                         const char *key) {
  if (!stream || !key) return 0;
  return stream->impl->HasOption(key);
}

void EdgevoxOnnxDecodeOfflineStream(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineStream *stream) {
  if (!recognizer || !stream) return;
  recognizer->impl->DecodeStream(stream->impl.get());
}

void EdgevoxOnnxDecodeMultipleOfflineStreams(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineStream **streams, int32_t n) {
  if (!recognizer || !streams) return;
  std::vector<edgevox_onnx::OfflineStream *> ss(n);
  for (int32_t i = 0; i != n; ++i) {
    ss[i] = streams[i]->impl.get();
  }
  recognizer->impl->DecodeStreams(ss.data(), n);
}

const EdgevoxOnnxOfflineRecognizerResult *EdgevoxOnnxGetOfflineStreamResult(
    const EdgevoxOnnxOfflineStream *stream) {
  if (!stream) return nullptr;
  const edgevox_onnx::OfflineRecognitionResult &result =
      stream->impl->GetResult();
  const auto &text = result.text;

  auto r = new EdgevoxOnnxOfflineRecognizerResult;
  memset(r, 0, sizeof(EdgevoxOnnxOfflineRecognizerResult));

  char *pText = new char[text.size() + 1];
  std::copy(text.begin(), text.end(), pText);
  pText[text.size()] = 0;
  r->text = pText;

  // lang
  const auto &lang = result.lang;
  char *c_lang = new char[lang.size() + 1];
  std::copy(lang.begin(), lang.end(), c_lang);
  c_lang[lang.size()] = '\0';
  r->lang = c_lang;

  // emotion
  const auto &emotion = result.emotion;
  char *c_emotion = new char[emotion.size() + 1];
  std::copy(emotion.begin(), emotion.end(), c_emotion);
  c_emotion[emotion.size()] = '\0';
  r->emotion = c_emotion;

  // event
  const auto &event = result.event;
  char *c_event = new char[event.size() + 1];
  std::copy(event.begin(), event.end(), c_event);
  c_event[event.size()] = '\0';
  r->event = c_event;

  // copy json
  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  r->json = pJson;

  // copy tokens
  auto count = result.tokens.size();
  if (count > 0) {
    size_t total_length = 0;
    for (const auto &token : result.tokens) {
      // +1 for the null character at the end of each token
      total_length += token.size() + 1;
    }

    r->count = count;
    // Each word ends with nullptr
    char *tokens = new char[total_length]{};
    char **tokens_temp = new char *[r->count];
    int32_t pos = 0;
    for (int32_t i = 0; i < r->count; ++i) {
      tokens_temp[i] = tokens + pos;
      memcpy(tokens + pos, result.tokens[i].c_str(), result.tokens[i].size());
      // +1 to move past the null character
      pos += result.tokens[i].size() + 1;
    }
    r->tokens_arr = tokens_temp;

    if (!result.timestamps.empty() && result.timestamps.size() == r->count) {
      r->timestamps = new float[r->count];
      std::copy(result.timestamps.begin(), result.timestamps.end(),
                r->timestamps);
    } else {
      r->timestamps = nullptr;
    }

    if (!result.durations.empty() && result.durations.size() == r->count) {
      r->durations = new float[r->count];
      std::copy(result.durations.begin(), result.durations.end(), r->durations);
    } else {
      r->durations = nullptr;
    }

    if (!result.ys_log_probs.empty() &&
        result.ys_log_probs.size() == r->count) {
      r->ys_log_probs = new float[r->count];
      std::copy(result.ys_log_probs.begin(), result.ys_log_probs.end(),
                r->ys_log_probs);
    } else {
      r->ys_log_probs = nullptr;
    }

    r->tokens = tokens;
  } else {
    r->count = 0;
    r->timestamps = nullptr;
    r->tokens = nullptr;
    r->tokens_arr = nullptr;
    r->ys_log_probs = nullptr;
  }

  // Copy segment-level timestamps (from Whisper with segment timestamps)
  auto segment_count = result.segment_texts.size();
  if (segment_count > 0 && result.segment_timestamps.size() == segment_count &&
      result.segment_durations.size() == segment_count) {
    r->segment_count = segment_count;

    // Copy segment timestamps
    float *timestamps = new float[segment_count];
    std::copy(result.segment_timestamps.begin(),
              result.segment_timestamps.end(), timestamps);
    r->segment_timestamps = timestamps;

    // Copy segment durations
    float *durations = new float[segment_count];
    std::copy(result.segment_durations.begin(), result.segment_durations.end(),
              durations);
    r->segment_durations = durations;

    // Copy segment texts (similar to tokens)
    size_t total_length = 0;
    for (const auto &seg_text : result.segment_texts) {
      total_length += seg_text.size() + 1;  // +1 for null terminator
    }

    char *segment_texts = new char[total_length]{};
    char **segment_texts_temp = new char *[segment_count];
    int32_t pos = 0;
    for (int32_t i = 0; i < static_cast<int32_t>(segment_count); ++i) {
      segment_texts_temp[i] = segment_texts + pos;
      memcpy(segment_texts + pos, result.segment_texts[i].c_str(),
             result.segment_texts[i].size());
      pos += result.segment_texts[i].size() + 1;
    }
    r->segment_texts = segment_texts;
    r->segment_texts_arr = segment_texts_temp;
  } else {
    r->segment_count = 0;
    r->segment_timestamps = nullptr;
    r->segment_durations = nullptr;
    r->segment_texts = nullptr;
    r->segment_texts_arr = nullptr;
  }

  return r;
}

void EdgevoxOnnxDestroyOfflineRecognizerResult(
    const EdgevoxOnnxOfflineRecognizerResult *r) {
  if (r) {
    delete[] r->text;
    delete[] r->timestamps;
    delete[] r->durations;
    delete[] r->ys_log_probs;
    delete[] r->tokens;
    delete[] r->tokens_arr;
    delete[] r->json;
    delete[] r->lang;
    delete[] r->emotion;
    delete[] r->event;
    delete[] r->segment_timestamps;
    delete[] r->segment_durations;
    delete[] r->segment_texts;
    delete[] r->segment_texts_arr;
    delete r;
  }
}

const char *EdgevoxOnnxGetOfflineStreamResultAsJson(
    const EdgevoxOnnxOfflineStream *stream) {
  const edgevox_onnx::OfflineRecognitionResult &result =
      stream->impl->GetResult();
  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  return pJson;
}

void EdgevoxOnnxDestroyOfflineStreamResultJson(const char *s) {
  if (!s) return;
  delete[] s;
}

// ============================================================
// For Keyword Spot
// ============================================================

struct EdgevoxOnnxKeywordSpotter {
  std::unique_ptr<edgevox_onnx::KeywordSpotter> impl;
};

static edgevox_onnx::KeywordSpotterConfig GetKeywordSpotterConfig(
    const EdgevoxOnnxKeywordSpotterConfig *config) {
  edgevox_onnx::KeywordSpotterConfig spotter_config;

  spotter_config.feat_config.sampling_rate =
      EDGEVOX_ONNX_OR(config->feat_config.sample_rate, 16000);
  spotter_config.feat_config.feature_dim =
      EDGEVOX_ONNX_OR(config->feat_config.feature_dim, 80);

  spotter_config.model_config.transducer.encoder =
      EDGEVOX_ONNX_OR(config->model_config.transducer.encoder, "");
  spotter_config.model_config.transducer.decoder =
      EDGEVOX_ONNX_OR(config->model_config.transducer.decoder, "");
  spotter_config.model_config.transducer.joiner =
      EDGEVOX_ONNX_OR(config->model_config.transducer.joiner, "");

  spotter_config.model_config.paraformer.encoder =
      EDGEVOX_ONNX_OR(config->model_config.paraformer.encoder, "");
  spotter_config.model_config.paraformer.decoder =
      EDGEVOX_ONNX_OR(config->model_config.paraformer.decoder, "");

  spotter_config.model_config.zipformer2_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.zipformer2_ctc.model, "");

  spotter_config.model_config.nemo_ctc.model =
      EDGEVOX_ONNX_OR(config->model_config.nemo_ctc.model, "");

  spotter_config.model_config.tokens =
      EDGEVOX_ONNX_OR(config->model_config.tokens, "");
  if (config->model_config.tokens_buf &&
      config->model_config.tokens_buf_size > 0) {
    spotter_config.model_config.tokens_buf = std::string(
        config->model_config.tokens_buf, config->model_config.tokens_buf_size);
  }

  spotter_config.model_config.num_threads =
      EDGEVOX_ONNX_OR(config->model_config.num_threads, 1);
  spotter_config.model_config.provider_config.provider =
      EDGEVOX_ONNX_OR(config->model_config.provider, "cpu");
  if (spotter_config.model_config.provider_config.provider.empty()) {
    spotter_config.model_config.provider_config.provider = "cpu";
  }

  spotter_config.model_config.model_type =
      EDGEVOX_ONNX_OR(config->model_config.model_type, "");
  spotter_config.model_config.debug = config->model_config.debug;

  spotter_config.max_active_paths = EDGEVOX_ONNX_OR(config->max_active_paths, 4);

  spotter_config.num_trailing_blanks =
      EDGEVOX_ONNX_OR(config->num_trailing_blanks, 1);

  spotter_config.keywords_score = EDGEVOX_ONNX_OR(config->keywords_score, 1.0);

  spotter_config.keywords_threshold =
      EDGEVOX_ONNX_OR(config->keywords_threshold, 0.25);

  spotter_config.keywords_file = EDGEVOX_ONNX_OR(config->keywords_file, "");
  if (config->keywords_buf && config->keywords_buf_size > 0) {
    spotter_config.keywords_buf =
        std::string(config->keywords_buf, config->keywords_buf_size);
  }

  if (spotter_config.model_config.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", spotter_config.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", spotter_config.ToString().c_str());
#endif
  }

  return spotter_config;
}

const EdgevoxOnnxKeywordSpotter *EdgevoxOnnxCreateKeywordSpotter(
    const EdgevoxOnnxKeywordSpotterConfig *config) {
  auto spotter_config = GetKeywordSpotterConfig(config);
  if (!spotter_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config!");
    return nullptr;
  }

  EdgevoxOnnxKeywordSpotter *spotter = new EdgevoxOnnxKeywordSpotter;

  spotter->impl = std::make_unique<edgevox_onnx::KeywordSpotter>(spotter_config);

  return spotter;
}

void EdgevoxOnnxDestroyKeywordSpotter(const EdgevoxOnnxKeywordSpotter *spotter) {
  if (!spotter) return;
  delete spotter;
}

const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateKeywordStream(
    const EdgevoxOnnxKeywordSpotter *spotter) {
  if (!spotter) return nullptr;
  EdgevoxOnnxOnlineStream *stream =
      new EdgevoxOnnxOnlineStream(spotter->impl->CreateStream());
  return stream;
}

const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateKeywordStreamWithKeywords(
    const EdgevoxOnnxKeywordSpotter *spotter, const char *keywords) {
  if (!spotter) return nullptr;
  EdgevoxOnnxOnlineStream *stream =
      new EdgevoxOnnxOnlineStream(spotter->impl->CreateStream(keywords));
  return stream;
}

int32_t EdgevoxOnnxIsKeywordStreamReady(const EdgevoxOnnxKeywordSpotter *spotter,
                                       const EdgevoxOnnxOnlineStream *stream) {
  if (!spotter || !stream) return 0;
  return spotter->impl->IsReady(stream->impl.get());
}

void EdgevoxOnnxDecodeKeywordStream(const EdgevoxOnnxKeywordSpotter *spotter,
                                   const EdgevoxOnnxOnlineStream *stream) {
  if (!spotter || !stream) return;
  spotter->impl->DecodeStream(stream->impl.get());
}

void EdgevoxOnnxResetKeywordStream(const EdgevoxOnnxKeywordSpotter *spotter,
                                  const EdgevoxOnnxOnlineStream *stream) {
  if (!spotter || !stream) return;
  spotter->impl->Reset(stream->impl.get());
}

void EdgevoxOnnxDecodeMultipleKeywordStreams(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream **streams, int32_t n) {
  if (!spotter || !streams || n <= 0) return;
  std::vector<edgevox_onnx::OnlineStream *> ss(n);
  for (int32_t i = 0; i != n; ++i) {
    ss[i] = streams[i]->impl.get();
  }
  spotter->impl->DecodeStreams(ss.data(), n);
}

const EdgevoxOnnxKeywordResult *EdgevoxOnnxGetKeywordResult(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream) {
  if (!spotter || !stream) return nullptr;
  const edgevox_onnx::KeywordResult &result =
      spotter->impl->GetResult(stream->impl.get());
  const auto &keyword = result.keyword;

  auto r = new EdgevoxOnnxKeywordResult;
  memset(r, 0, sizeof(EdgevoxOnnxKeywordResult));

  r->start_time = result.start_time;

  // copy keyword
  char *pKeyword = new char[keyword.size() + 1];
  std::copy(keyword.begin(), keyword.end(), pKeyword);
  pKeyword[keyword.size()] = 0;
  r->keyword = pKeyword;

  // copy json
  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  r->json = pJson;

  // copy tokens
  auto count = result.tokens.size();
  if (count > 0) {
    size_t total_length = 0;
    for (const auto &token : result.tokens) {
      // +1 for the null character at the end of each token
      total_length += token.size() + 1;
    }

    r->count = count;
    // Each word ends with nullptr
    char *pTokens = new char[total_length]{};
    char **tokens_temp = new char *[r->count];
    int32_t pos = 0;
    for (int32_t i = 0; i < r->count; ++i) {
      tokens_temp[i] = pTokens + pos;
      memcpy(pTokens + pos, result.tokens[i].c_str(), result.tokens[i].size());
      // +1 to move past the null character
      pos += result.tokens[i].size() + 1;
    }
    r->tokens = pTokens;
    r->tokens_arr = tokens_temp;

    if (!result.timestamps.empty()) {
      r->timestamps = new float[result.timestamps.size()];
      std::copy(result.timestamps.begin(), result.timestamps.end(),
                r->timestamps);
    } else {
      r->timestamps = nullptr;
    }

  } else {
    r->count = 0;
    r->timestamps = nullptr;
    r->tokens = nullptr;
    r->tokens_arr = nullptr;
  }

  return r;
}

void EdgevoxOnnxDestroyKeywordResult(const EdgevoxOnnxKeywordResult *r) {
  if (r) {
    delete[] r->keyword;
    delete[] r->json;
    delete[] r->tokens;
    delete[] r->tokens_arr;
    delete[] r->timestamps;
    delete r;
  }
}

const char *EdgevoxOnnxGetKeywordResultAsJson(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream) {
  if (!spotter || !stream) return nullptr;
  const edgevox_onnx::KeywordResult &result =
      spotter->impl->GetResult(stream->impl.get());

  std::string json = result.AsJsonString();
  char *pJson = new char[json.size() + 1];
  std::copy(json.begin(), json.end(), pJson);
  pJson[json.size()] = 0;
  return pJson;
}

void EdgevoxOnnxFreeKeywordResultJson(const char *s) {
  if (!s) return;
  delete[] s;
}

// ============================================================
// For VAD
// ============================================================
//
struct EdgevoxOnnxCircularBuffer {
  std::unique_ptr<edgevox_onnx::CircularBuffer> impl;
};

const EdgevoxOnnxCircularBuffer *EdgevoxOnnxCreateCircularBuffer(
    int32_t capacity) {
  EdgevoxOnnxCircularBuffer *buffer = new EdgevoxOnnxCircularBuffer;
  buffer->impl = std::make_unique<edgevox_onnx::CircularBuffer>(capacity);
  return buffer;
}

void EdgevoxOnnxDestroyCircularBuffer(const EdgevoxOnnxCircularBuffer *buffer) {
  if (!buffer) return;
  delete buffer;
}

void EdgevoxOnnxCircularBufferPush(const EdgevoxOnnxCircularBuffer *buffer,
                                  const float *p, int32_t n) {
  if (!buffer) return;
  buffer->impl->Push(p, n);
}

const float *EdgevoxOnnxCircularBufferGet(const EdgevoxOnnxCircularBuffer *buffer,
                                         int32_t start_index, int32_t n) {
  if (!buffer) return nullptr;
  std::vector<float> v = buffer->impl->Get(start_index, n);

  float *p = nullptr;
  if (!v.empty()) {
    p = new float[v.size()]();
    std::copy(v.begin(), v.end(), p);
  }

  return p;
}

void EdgevoxOnnxCircularBufferFree(const float *p) {
  if (!p) return;
  delete[] p;
}

void EdgevoxOnnxCircularBufferPop(const EdgevoxOnnxCircularBuffer *buffer,
                                 int32_t n) {
  if (!buffer) return;
  buffer->impl->Pop(n);
}

int32_t EdgevoxOnnxCircularBufferSize(const EdgevoxOnnxCircularBuffer *buffer) {
  if (!buffer) return 0;
  return buffer->impl->Size();
}

int32_t EdgevoxOnnxCircularBufferHead(const EdgevoxOnnxCircularBuffer *buffer) {
  if (!buffer) return 0;
  return buffer->impl->Head();
}

void EdgevoxOnnxCircularBufferReset(const EdgevoxOnnxCircularBuffer *buffer) {
  if (!buffer) return;
  buffer->impl->Reset();
}

struct EdgevoxOnnxVoiceActivityDetector {
  std::unique_ptr<edgevox_onnx::VoiceActivityDetector> impl;
};

static edgevox_onnx::VadModelConfig GetVadModelConfig(
    const EdgevoxOnnxVadModelConfig *config) {
  edgevox_onnx::VadModelConfig vad_config;

  vad_config.silero_vad.model = EDGEVOX_ONNX_OR(config->silero_vad.model, "");
  vad_config.silero_vad.threshold =
      EDGEVOX_ONNX_OR(config->silero_vad.threshold, 0.5);

  vad_config.silero_vad.min_silence_duration =
      EDGEVOX_ONNX_OR(config->silero_vad.min_silence_duration, 0.5);

  vad_config.silero_vad.min_speech_duration =
      EDGEVOX_ONNX_OR(config->silero_vad.min_speech_duration, 0.25);

  vad_config.silero_vad.window_size =
      EDGEVOX_ONNX_OR(config->silero_vad.window_size, 512);

  vad_config.silero_vad.max_speech_duration =
      EDGEVOX_ONNX_OR(config->silero_vad.max_speech_duration, 20);

  vad_config.ten_vad.model = EDGEVOX_ONNX_OR(config->ten_vad.model, "");
  vad_config.ten_vad.threshold = EDGEVOX_ONNX_OR(config->ten_vad.threshold, 0.5);

  vad_config.ten_vad.min_silence_duration =
      EDGEVOX_ONNX_OR(config->ten_vad.min_silence_duration, 0.5);

  vad_config.ten_vad.min_speech_duration =
      EDGEVOX_ONNX_OR(config->ten_vad.min_speech_duration, 0.25);

  vad_config.ten_vad.window_size =
      EDGEVOX_ONNX_OR(config->ten_vad.window_size, 256);

  vad_config.ten_vad.max_speech_duration =
      EDGEVOX_ONNX_OR(config->ten_vad.max_speech_duration, 20);

  vad_config.sample_rate = EDGEVOX_ONNX_OR(config->sample_rate, 16000);
  vad_config.num_threads = EDGEVOX_ONNX_OR(config->num_threads, 1);
  vad_config.provider = EDGEVOX_ONNX_OR(config->provider, "cpu");
  if (vad_config.provider.empty()) {
    vad_config.provider = "cpu";
  }

  vad_config.debug = config->debug;

  if (vad_config.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", vad_config.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", vad_config.ToString().c_str());
#endif
  }

  return vad_config;
}

const EdgevoxOnnxVoiceActivityDetector *EdgevoxOnnxCreateVoiceActivityDetector(
    const EdgevoxOnnxVadModelConfig *config, float buffer_size_in_seconds) {
  if (!config) {
    EDGEVOX_ONNX_LOGE("vad config is nullptr");
    return nullptr;
  }

  auto vad_config = GetVadModelConfig(config);

  if (!vad_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxVoiceActivityDetector *p = new EdgevoxOnnxVoiceActivityDetector;
  p->impl = std::make_unique<edgevox_onnx::VoiceActivityDetector>(
      vad_config, buffer_size_in_seconds);

  return p;
}

void EdgevoxOnnxDestroyVoiceActivityDetector(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) return;
  delete p;
}

void EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(
    const EdgevoxOnnxVoiceActivityDetector *p, const float *samples, int32_t n) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return;
  }

  if (!samples) {
    EDGEVOX_ONNX_LOGE("samples is nullptr");
    return;
  }

  p->impl->AcceptWaveform(samples, n);
}

int32_t EdgevoxOnnxVoiceActivityDetectorEmpty(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return 1;  // 1 means it is empty
  }

  return p->impl->Empty();
}

int32_t EdgevoxOnnxVoiceActivityDetectorDetected(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return 0;
  }

  return p->impl->IsSpeechDetected();
}

void EdgevoxOnnxVoiceActivityDetectorPop(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return;
  }

  p->impl->Pop();
}

void EdgevoxOnnxVoiceActivityDetectorClear(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return;
  }

  p->impl->Clear();
}

const EdgevoxOnnxSpeechSegment *EdgevoxOnnxVoiceActivityDetectorFront(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return nullptr;
  }

  if (EdgevoxOnnxVoiceActivityDetectorEmpty(p)) {
    return nullptr;
  }

  const edgevox_onnx::SpeechSegment &segment = p->impl->Front();

  EdgevoxOnnxSpeechSegment *ans = new EdgevoxOnnxSpeechSegment;
  ans->start = segment.start;
  ans->samples = new float[segment.samples.size()];
  std::copy(segment.samples.begin(), segment.samples.end(), ans->samples);
  ans->n = segment.samples.size();

  return ans;
}

void EdgevoxOnnxDestroySpeechSegment(const EdgevoxOnnxSpeechSegment *p) {
  if (p) {
    delete[] p->samples;
    delete p;
  }
}

void EdgevoxOnnxVoiceActivityDetectorReset(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return;
  }

  p->impl->Reset();
}

void EdgevoxOnnxVoiceActivityDetectorFlush(
    const EdgevoxOnnxVoiceActivityDetector *p) {
  if (!p) {
    EDGEVOX_ONNX_LOGE("vad is nullptr");
    return;
  }

  p->impl->Flush();
}

#if EDGEVOX_ONNX_ENABLE_TTS == 1
struct EdgevoxOnnxOfflineTts {
  std::unique_ptr<edgevox_onnx::OfflineTts> impl;
};

static edgevox_onnx::OfflineTtsConfig GetOfflineTtsConfig(
    const EdgevoxOnnxOfflineTtsConfig *config) {
  edgevox_onnx::OfflineTtsConfig tts_config;

  // vits
  tts_config.model.vits.model = EDGEVOX_ONNX_OR(config->model.vits.model, "");
  tts_config.model.vits.lexicon =
      EDGEVOX_ONNX_OR(config->model.vits.lexicon, "");
  tts_config.model.vits.tokens = EDGEVOX_ONNX_OR(config->model.vits.tokens, "");
  tts_config.model.vits.data_dir =
      EDGEVOX_ONNX_OR(config->model.vits.data_dir, "");
  tts_config.model.vits.noise_scale =
      EDGEVOX_ONNX_OR(config->model.vits.noise_scale, 0.667);
  tts_config.model.vits.noise_scale_w =
      EDGEVOX_ONNX_OR(config->model.vits.noise_scale_w, 0.8);
  tts_config.model.vits.length_scale =
      EDGEVOX_ONNX_OR(config->model.vits.length_scale, 1.0);
  tts_config.model.vits.openvoice_tone_encoder =
      EDGEVOX_ONNX_OR(config->model.vits.openvoice_tone_encoder, "");
  tts_config.model.vits.openvoice_tone_converter =
      EDGEVOX_ONNX_OR(config->model.vits.openvoice_tone_converter, "");

  // matcha
  tts_config.model.matcha.acoustic_model =
      EDGEVOX_ONNX_OR(config->model.matcha.acoustic_model, "");
  tts_config.model.matcha.vocoder =
      EDGEVOX_ONNX_OR(config->model.matcha.vocoder, "");
  tts_config.model.matcha.lexicon =
      EDGEVOX_ONNX_OR(config->model.matcha.lexicon, "");
  tts_config.model.matcha.tokens =
      EDGEVOX_ONNX_OR(config->model.matcha.tokens, "");
  tts_config.model.matcha.data_dir =
      EDGEVOX_ONNX_OR(config->model.matcha.data_dir, "");
  tts_config.model.matcha.noise_scale =
      EDGEVOX_ONNX_OR(config->model.matcha.noise_scale, 0.667);
  tts_config.model.matcha.length_scale =
      EDGEVOX_ONNX_OR(config->model.matcha.length_scale, 1.0);

  // kokoro
  tts_config.model.kokoro.model =
      EDGEVOX_ONNX_OR(config->model.kokoro.model, "");
  tts_config.model.kokoro.voices =
      EDGEVOX_ONNX_OR(config->model.kokoro.voices, "");
  tts_config.model.kokoro.tokens =
      EDGEVOX_ONNX_OR(config->model.kokoro.tokens, "");
  tts_config.model.kokoro.data_dir =
      EDGEVOX_ONNX_OR(config->model.kokoro.data_dir, "");
  tts_config.model.kokoro.length_scale =
      EDGEVOX_ONNX_OR(config->model.kokoro.length_scale, 1.0);
  tts_config.model.kokoro.lexicon =
      EDGEVOX_ONNX_OR(config->model.kokoro.lexicon, "");
  tts_config.model.kokoro.lang = EDGEVOX_ONNX_OR(config->model.kokoro.lang, "");

  // kitten
  tts_config.model.kitten.model =
      EDGEVOX_ONNX_OR(config->model.kitten.model, "");
  tts_config.model.kitten.voices =
      EDGEVOX_ONNX_OR(config->model.kitten.voices, "");
  tts_config.model.kitten.tokens =
      EDGEVOX_ONNX_OR(config->model.kitten.tokens, "");
  tts_config.model.kitten.data_dir =
      EDGEVOX_ONNX_OR(config->model.kitten.data_dir, "");
  tts_config.model.kitten.length_scale =
      EDGEVOX_ONNX_OR(config->model.kitten.length_scale, 1.0);

  // zipvoice
  tts_config.model.zipvoice.tokens =
      EDGEVOX_ONNX_OR(config->model.zipvoice.tokens, "");
  tts_config.model.zipvoice.encoder =
      EDGEVOX_ONNX_OR(config->model.zipvoice.encoder, "");
  tts_config.model.zipvoice.decoder =
      EDGEVOX_ONNX_OR(config->model.zipvoice.decoder, "");
  tts_config.model.zipvoice.vocoder =
      EDGEVOX_ONNX_OR(config->model.zipvoice.vocoder, "");
  tts_config.model.zipvoice.data_dir =
      EDGEVOX_ONNX_OR(config->model.zipvoice.data_dir, "");
  tts_config.model.zipvoice.lexicon =
      EDGEVOX_ONNX_OR(config->model.zipvoice.lexicon, "");
  // Required for non-English ZipVoice (e.g. Sinhala espeak_voice="si").
  // Without this copy, C++ keeps the default "en-us" and OOV phonemization
  // is wrong even when JS/WASM packs espeakVoice correctly.
  tts_config.model.zipvoice.espeak_voice =
      EDGEVOX_ONNX_OR(config->model.zipvoice.espeak_voice, "en-us");
  tts_config.model.zipvoice.feat_scale =
      EDGEVOX_ONNX_OR(config->model.zipvoice.feat_scale, 0.1f);
  tts_config.model.zipvoice.t_shift =
      EDGEVOX_ONNX_OR(config->model.zipvoice.t_shift, 0.5f);
  tts_config.model.zipvoice.target_rms =
      EDGEVOX_ONNX_OR(config->model.zipvoice.target_rms, 0.1f);
  tts_config.model.zipvoice.guidance_scale =
      EDGEVOX_ONNX_OR(config->model.zipvoice.guidance_scale, 1.0f);

  // pocket
  tts_config.model.pocket.lm_flow =
      EDGEVOX_ONNX_OR(config->model.pocket.lm_flow, "");
  tts_config.model.pocket.lm_main =
      EDGEVOX_ONNX_OR(config->model.pocket.lm_main, "");
  tts_config.model.pocket.encoder =
      EDGEVOX_ONNX_OR(config->model.pocket.encoder, "");
  tts_config.model.pocket.decoder =
      EDGEVOX_ONNX_OR(config->model.pocket.decoder, "");
  tts_config.model.pocket.text_conditioner =
      EDGEVOX_ONNX_OR(config->model.pocket.text_conditioner, "");
  tts_config.model.pocket.vocab_json =
      EDGEVOX_ONNX_OR(config->model.pocket.vocab_json, "");
  tts_config.model.pocket.token_scores_json =
      EDGEVOX_ONNX_OR(config->model.pocket.token_scores_json, "");
  if (config->model.pocket.voice_embedding_cache_capacity >= 0) {
    tts_config.model.pocket.voice_embedding_cache_capacity =
        config->model.pocket.voice_embedding_cache_capacity;
  } else {
    tts_config.model.pocket.voice_embedding_cache_capacity = 50;
  }

  // supertonic
  tts_config.model.supertonic.duration_predictor =
      EDGEVOX_ONNX_OR(config->model.supertonic.duration_predictor, "");
  tts_config.model.supertonic.text_encoder =
      EDGEVOX_ONNX_OR(config->model.supertonic.text_encoder, "");
  tts_config.model.supertonic.vector_estimator =
      EDGEVOX_ONNX_OR(config->model.supertonic.vector_estimator, "");
  tts_config.model.supertonic.vocoder =
      EDGEVOX_ONNX_OR(config->model.supertonic.vocoder, "");
  tts_config.model.supertonic.tts_json =
      EDGEVOX_ONNX_OR(config->model.supertonic.tts_json, "");
  tts_config.model.supertonic.unicode_indexer =
      EDGEVOX_ONNX_OR(config->model.supertonic.unicode_indexer, "");
  tts_config.model.supertonic.voice_style =
      EDGEVOX_ONNX_OR(config->model.supertonic.voice_style, "");

  // chatterbox
  tts_config.model.chatterbox.speech_encoder =
      EDGEVOX_ONNX_OR(config->model.chatterbox.speech_encoder, "");
  tts_config.model.chatterbox.embed_tokens =
      EDGEVOX_ONNX_OR(config->model.chatterbox.embed_tokens, "");
  tts_config.model.chatterbox.language_model =
      EDGEVOX_ONNX_OR(config->model.chatterbox.language_model, "");
  tts_config.model.chatterbox.conditional_decoder =
      EDGEVOX_ONNX_OR(config->model.chatterbox.conditional_decoder, "");
  tts_config.model.chatterbox.tokenizer =
      EDGEVOX_ONNX_OR(config->model.chatterbox.tokenizer, "");

  tts_config.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  tts_config.model.debug = config->model.debug;
  tts_config.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  if (tts_config.model.provider.empty()) {
    tts_config.model.provider = "cpu";
  }

  tts_config.rule_fsts = EDGEVOX_ONNX_OR(config->rule_fsts, "");
  tts_config.rule_fars = EDGEVOX_ONNX_OR(config->rule_fars, "");
  tts_config.max_num_sentences = EDGEVOX_ONNX_OR(config->max_num_sentences, 1);
  tts_config.silence_scale = EDGEVOX_ONNX_OR(config->silence_scale, 0.2);

  if (tts_config.model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", tts_config.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", tts_config.ToString().c_str());
#endif
  }

  return tts_config;
}

const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTts(
    const EdgevoxOnnxOfflineTtsConfig *config) {
  auto tts_config = GetOfflineTtsConfig(config);

  if (!tts_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflineTts *tts = new EdgevoxOnnxOfflineTts;

  tts->impl = std::make_unique<edgevox_onnx::OfflineTts>(tts_config);

  return tts;
}

void EdgevoxOnnxDestroyOfflineTts(const EdgevoxOnnxOfflineTts *tts) {
  if (!tts) return;
  delete tts;
}

int32_t EdgevoxOnnxOfflineTtsSampleRate(const EdgevoxOnnxOfflineTts *tts) {
  return tts->impl->SampleRate();
}

int32_t EdgevoxOnnxOfflineTtsNumSpeakers(const EdgevoxOnnxOfflineTts *tts) {
  return tts->impl->NumSpeakers();
}

static const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateInternal(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    std::function<int32_t(const float *, int32_t, float)> callback) {
  edgevox_onnx::GenerationConfig config;
  config.sid = sid;
  config.speed = speed;

  edgevox_onnx::GeneratedAudio audio =
      tts->impl->Generate(text, config, callback);

  if (audio.samples.empty()) {
    return nullptr;
  }

  EdgevoxOnnxGeneratedAudio *ans = new EdgevoxOnnxGeneratedAudio;

  float *samples = new float[audio.samples.size()];
  std::copy(audio.samples.begin(), audio.samples.end(), samples);

  ans->samples = samples;
  ans->n = audio.samples.size();
  ans->sample_rate = audio.sample_rate;

  return ans;
}

static const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateInternal(
    const EdgevoxOnnxOfflineTts *tts, const char *text,
    const EdgevoxOnnxGenerationConfig *config,
    std::function<int32_t(const float *, int32_t, float)> callback) {
  edgevox_onnx::GenerationConfig cfg;
  if (config->reference_audio) {
    if (config->reference_audio_len <= 0) {
      EDGEVOX_ONNX_LOGE("Invalid reference audio len: %d",
                       config->reference_audio_len);
      return nullptr;
    }

    cfg.reference_audio.assign(
        config->reference_audio,
        config->reference_audio + config->reference_audio_len);
  }

  cfg.silence_scale = EDGEVOX_ONNX_OR(config->silence_scale, 0.2);
  cfg.speed = EDGEVOX_ONNX_OR(config->speed, 1.0);
  cfg.sid = config->sid;

  cfg.reference_sample_rate = config->reference_sample_rate;

  cfg.reference_text = EDGEVOX_ONNX_OR(config->reference_text, "");
  cfg.num_steps = EDGEVOX_ONNX_OR(config->num_steps, 5);

  if (config->extra && !std::string(config->extra).empty()) {
    try {
      auto json = nlohmann::json::parse(config->extra);
      for (auto &[k, v] : json.items()) {
        std::string val = v.is_string() ? v.get<std::string>() : v.dump();
        cfg.extra.insert_or_assign(std::string(k), std::move(val));
      }
    } catch (const nlohmann::json::parse_error &e) {
      EDGEVOX_ONNX_LOGE("Failed to parse extra JSON: '%s'", e.what());
      EDGEVOX_ONNX_LOGE("Ignore the extra opt");
    }
  }

  edgevox_onnx::GeneratedAudio audio = tts->impl->Generate(text, cfg, callback);

  if (audio.samples.empty()) {
    return nullptr;
  }

  EdgevoxOnnxGeneratedAudio *ans = new EdgevoxOnnxGeneratedAudio;

  float *samples = new float[audio.samples.size()];
  std::copy(audio.samples.begin(), audio.samples.end(), samples);

  ans->samples = samples;
  ans->n = audio.samples.size();
  ans->sample_rate = audio.sample_rate;

  return ans;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerate(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
    float speed) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed, nullptr);
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallback(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioCallback callback) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (callback) {
    auto wrapper = [callback](const float *samples, int32_t n,
                              float /*progress*/) {
      return callback(samples, n);
    };

    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed,
                                                std::move(wrapper));
  } else {
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed, nullptr);
  }
}

const EdgevoxOnnxGeneratedAudio *
EdgevoxOnnxOfflineTtsGenerateWithProgressCallback(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioProgressCallback callback) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (callback) {
    auto wrapper = [callback](const float *samples, int32_t n, float progress) {
      return callback(samples, n, progress);
    };
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed,
                                                std::move(wrapper));
  } else {
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed, nullptr);
  }
}

const EdgevoxOnnxGeneratedAudio *
EdgevoxOnnxOfflineTtsGenerateWithProgressCallbackWithArg(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback, void *arg) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (callback) {
    auto wrapper = [callback, arg](const float *samples, int32_t n,
                                   float progress) {
      return callback(samples, n, progress, arg);
    };
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed,
                                                std::move(wrapper));
  } else {
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed, nullptr);
  }
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallbackWithArg(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioCallbackWithArg callback, void *arg) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (callback) {
    auto wrapper = [callback, arg](const float *samples, int32_t n,
                                   float /*progress*/) {
      return callback(samples, n, arg);
    };

    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed,
                                                std::move(wrapper));
  } else {
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, sid, speed, nullptr);
  }
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithZipvoice(
    const EdgevoxOnnxOfflineTts *tts, const char *text, const char *prompt_text,
    const float *prompt_samples, int32_t n_prompt, int32_t prompt_sr,
    float speed, int32_t num_steps) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (!prompt_text) {
    EDGEVOX_ONNX_LOGE("prompt_text is nullptr");
    return nullptr;
  }

  if (!prompt_samples) {
    EDGEVOX_ONNX_LOGE("prompt_samples is nullptr");
    return nullptr;
  }

  std::string text_s = text;
  std::string ptext_s = prompt_text;

  std::vector<float> prompt_vec;
  if (n_prompt > 0) {
    prompt_vec.assign(prompt_samples,
                      prompt_samples + static_cast<size_t>(n_prompt));
  }

  edgevox_onnx::GenerationConfig config;
  config.speed = speed;
  config.reference_audio = std::move(prompt_vec);
  config.reference_sample_rate = prompt_sr;
  config.reference_text = ptext_s;
  config.num_steps = num_steps;

  auto out = tts->impl->Generate(text_s, config, /*callback=*/nullptr);

  if (out.samples.empty()) {
    return nullptr;
  }

  auto *ans = new EdgevoxOnnxGeneratedAudio;
  ans->sample_rate = static_cast<int32_t>(out.sample_rate);
  ans->n = static_cast<int32_t>(out.samples.size());

  float *buf = new float[out.samples.size()];
  std::copy(out.samples.begin(), out.samples.end(), buf);
  ans->samples = buf;

  return ans;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithConfig(
    const EdgevoxOnnxOfflineTts *tts, const char *text,
    const EdgevoxOnnxGenerationConfig *config,
    EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback, void *arg) {
  if (!tts) {
    EDGEVOX_ONNX_LOGE("tts is nullptr");
    return nullptr;
  }

  if (!text) {
    EDGEVOX_ONNX_LOGE("text is nullptr");
    return nullptr;
  }

  if (!config) {
    EDGEVOX_ONNX_LOGE("config is nullptr");
    return nullptr;
  }

  if (callback) {
    auto wrapper = [callback, arg](const float *samples, int32_t n,
                                   float progress) {
      return callback(samples, n, progress, arg);
    };

    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, config,
                                                std::move(wrapper));
  } else {
    return EdgevoxOnnxOfflineTtsGenerateInternal(tts, text, config, nullptr);
  }
}

void EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(
    const EdgevoxOnnxGeneratedAudio *p) {
  if (p) {
    delete[] p->samples;
    delete p;
  }
}
#else
const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTts(
    const EdgevoxOnnxOfflineTtsConfig *config) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

void EdgevoxOnnxDestroyOfflineTts(const EdgevoxOnnxOfflineTts *tts) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
}

int32_t EdgevoxOnnxOfflineTtsSampleRate(const EdgevoxOnnxOfflineTts *tts) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return 0;
}

int32_t EdgevoxOnnxOfflineTtsNumSpeakers(const EdgevoxOnnxOfflineTts *tts) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return 0;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerate(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
    float speed) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallback(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioCallback callback) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *
EdgevoxOnnxOfflineTtsGenerateWithProgressCallback(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioProgressCallback callback) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *
EdgevoxOnnxOfflineTtsGenerateWithProgressCallbackWithArg(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback, void *arg) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallbackWithArg(
    const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid, float speed,
    EdgevoxOnnxGeneratedAudioCallbackWithArg callback, void *arg) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithZipvoice(
    const EdgevoxOnnxOfflineTts *tts, const char *text, const char *prompt_text,
    const float *prompt_samples, int32_t n_prompt, int32_t prompt_sr,
    float speed, int32_t num_steps) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithConfig(
    const EdgevoxOnnxOfflineTts *tts, const char *text,
    const EdgevoxOnnxGenerationConfig *config,
    EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback, void *arg) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

void EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(
    const EdgevoxOnnxGeneratedAudio *p) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
}
#endif  // EDGEVOX_ONNX_ENABLE_TTS == 1

int32_t EdgevoxOnnxWriteWave(const float *samples, int32_t n,
                            int32_t sample_rate, const char *filename) {
  return edgevox_onnx::WriteWave(filename, sample_rate, samples, n);
}

int64_t EdgevoxOnnxWaveFileSize(int32_t n_samples) {
  return edgevox_onnx::WaveFileSize(n_samples);
}

void EdgevoxOnnxWriteWaveToBuffer(const float *samples, int32_t n,
                                 int32_t sample_rate, char *buffer) {
  edgevox_onnx::WriteWave(buffer, sample_rate, samples, n);
}

int32_t EdgevoxOnnxWriteWaveMultiChannel(const float *const *samples, int32_t n,
                                        int32_t sample_rate,
                                        int32_t num_channels,
                                        const char *filename) {
  return edgevox_onnx::WriteWaveMultiChannel(filename, sample_rate, samples,
                                            num_channels, n);
}

const EdgevoxOnnxWave *EdgevoxOnnxReadWave(const char *filename) {
  int32_t sample_rate = -1;
  bool is_ok = false;
  std::vector<float> samples =
      edgevox_onnx::ReadWave(filename, &sample_rate, &is_ok);
  if (!is_ok) {
    return nullptr;
  }

  float *c_samples = new float[samples.size()];
  std::copy(samples.begin(), samples.end(), c_samples);

  EdgevoxOnnxWave *wave = new EdgevoxOnnxWave;
  wave->samples = c_samples;
  wave->sample_rate = sample_rate;
  wave->num_samples = samples.size();
  return wave;
}

const EdgevoxOnnxWave *EdgevoxOnnxReadWaveFromBinaryData(const char *data,
                                                       int32_t n) {
  if (!data || n <= 0) {
    return nullptr;
  }

  int32_t sample_rate = -1;
  bool is_ok = false;

  std::istringstream is(std::string(data, n));

  std::vector<float> samples = edgevox_onnx::ReadWave(is, &sample_rate, &is_ok);
  if (!is_ok) {
    return nullptr;
  }

  float *c_samples = new float[samples.size()];
  std::copy(samples.begin(), samples.end(), c_samples);

  EdgevoxOnnxWave *wave = new EdgevoxOnnxWave;
  wave->samples = c_samples;
  wave->sample_rate = sample_rate;
  wave->num_samples = samples.size();
  return wave;
}

void EdgevoxOnnxFreeWave(const EdgevoxOnnxWave *wave) {
  if (wave) {
    delete[] wave->samples;
    delete wave;
  }
}

struct EdgevoxOnnxMultiChannelWaveInternal : EdgevoxOnnxMultiChannelWave {
  std::vector<float> flat_samples;
  std::vector<const float *> channel_ptrs;
};

const EdgevoxOnnxMultiChannelWave *EdgevoxOnnxReadWaveMultiChannel(
    const char *filename) {
  int32_t sample_rate = -1;
  bool is_ok = false;

  auto samples_2d =
      edgevox_onnx::ReadWaveMultiChannel(filename, &sample_rate, &is_ok);

  if (!is_ok || samples_2d.empty()) {
    return nullptr;
  }

  int32_t num_channels = static_cast<int32_t>(samples_2d.size());
  int32_t num_samples = static_cast<int32_t>(samples_2d[0].size());

  auto *w = new EdgevoxOnnxMultiChannelWaveInternal();

  w->flat_samples.reserve(num_channels * num_samples);
  for (const auto &channel : samples_2d) {
    w->flat_samples.insert(w->flat_samples.end(), channel.begin(),
                           channel.end());
  }

  w->channel_ptrs.resize(num_channels);
  for (int32_t c = 0; c != num_channels; ++c) {
    w->channel_ptrs[c] = w->flat_samples.data() + (c * num_samples);
  }

  w->samples = w->channel_ptrs.data();
  w->num_channels = num_channels;
  w->num_samples = num_samples;
  w->sample_rate = sample_rate;

  return w;
}

void EdgevoxOnnxFreeMultiChannelWave(const EdgevoxOnnxMultiChannelWave *wave) {
  if (wave) {
    delete static_cast<const EdgevoxOnnxMultiChannelWaveInternal *>(wave);
  }
}

struct EdgevoxOnnxSpokenLanguageIdentification {
  std::unique_ptr<edgevox_onnx::SpokenLanguageIdentification> impl;
};

const EdgevoxOnnxSpokenLanguageIdentification *
EdgevoxOnnxCreateSpokenLanguageIdentification(
    const EdgevoxOnnxSpokenLanguageIdentificationConfig *config) {
  edgevox_onnx::SpokenLanguageIdentificationConfig slid_config;
  slid_config.whisper.encoder = EDGEVOX_ONNX_OR(config->whisper.encoder, "");
  slid_config.whisper.decoder = EDGEVOX_ONNX_OR(config->whisper.decoder, "");
  slid_config.whisper.tail_paddings =
      EDGEVOX_ONNX_OR(config->whisper.tail_paddings, -1);
  slid_config.num_threads = EDGEVOX_ONNX_OR(config->num_threads, 1);
  slid_config.debug = config->debug;
  slid_config.provider = EDGEVOX_ONNX_OR(config->provider, "cpu");
  if (slid_config.provider.empty()) {
    slid_config.provider = "cpu";
  }

  if (slid_config.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", slid_config.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", slid_config.ToString().c_str());
#endif
  }

  if (!slid_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxSpokenLanguageIdentification *slid =
      new EdgevoxOnnxSpokenLanguageIdentification;
  slid->impl =
      std::make_unique<edgevox_onnx::SpokenLanguageIdentification>(slid_config);

  return slid;
}

void EdgevoxOnnxDestroySpokenLanguageIdentification(
    const EdgevoxOnnxSpokenLanguageIdentification *slid) {
  if (!slid) return;
  delete slid;
}

EdgevoxOnnxOfflineStream *
EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(
    const EdgevoxOnnxSpokenLanguageIdentification *slid) {
  EdgevoxOnnxOfflineStream *stream =
      new EdgevoxOnnxOfflineStream(slid->impl->CreateStream());
  return stream;
}

const EdgevoxOnnxSpokenLanguageIdentificationResult *
EdgevoxOnnxSpokenLanguageIdentificationCompute(
    const EdgevoxOnnxSpokenLanguageIdentification *slid,
    const EdgevoxOnnxOfflineStream *s) {
  std::string lang = slid->impl->Compute(s->impl.get());
  char *c_lang = new char[lang.size() + 1];
  std::copy(lang.begin(), lang.end(), c_lang);
  c_lang[lang.size()] = '\0';
  EdgevoxOnnxSpokenLanguageIdentificationResult *r =
      new EdgevoxOnnxSpokenLanguageIdentificationResult;
  r->lang = c_lang;
  return r;
}

void EdgevoxOnnxDestroySpokenLanguageIdentificationResult(
    const EdgevoxOnnxSpokenLanguageIdentificationResult *r) {
  if (r) {
    delete[] r->lang;
    delete r;
  }
}

struct EdgevoxOnnxSpeakerEmbeddingExtractor {
  std::unique_ptr<edgevox_onnx::SpeakerEmbeddingExtractor> impl;
};

static edgevox_onnx::SpeakerEmbeddingExtractorConfig
GetSpeakerEmbeddingExtractorConfig(
    const EdgevoxOnnxSpeakerEmbeddingExtractorConfig *config) {
  edgevox_onnx::SpeakerEmbeddingExtractorConfig c;
  c.model = EDGEVOX_ONNX_OR(config->model, "");

  c.num_threads = EDGEVOX_ONNX_OR(config->num_threads, 1);
  c.debug = config->debug;
  c.provider = EDGEVOX_ONNX_OR(config->provider, "cpu");
  if (c.provider.empty()) {
    c.provider = "cpu";
  }

  if (config->debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxSpeakerEmbeddingExtractor *
EdgevoxOnnxCreateSpeakerEmbeddingExtractor(
    const EdgevoxOnnxSpeakerEmbeddingExtractorConfig *config) {
  auto c = GetSpeakerEmbeddingExtractorConfig(config);

  if (!c.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config!");
    return nullptr;
  }

  auto p = new EdgevoxOnnxSpeakerEmbeddingExtractor;

  p->impl = std::make_unique<edgevox_onnx::SpeakerEmbeddingExtractor>(c);

  return p;
}

void EdgevoxOnnxDestroySpeakerEmbeddingExtractor(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p) {
  if (!p) return;
  delete p;
}

int32_t EdgevoxOnnxSpeakerEmbeddingExtractorDim(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p) {
  return p->impl->Dim();
}

const EdgevoxOnnxOnlineStream *EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p) {
  EdgevoxOnnxOnlineStream *stream =
      new EdgevoxOnnxOnlineStream(p->impl->CreateStream());
  return stream;
}

int32_t EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p,
    const EdgevoxOnnxOnlineStream *s) {
  return p->impl->IsReady(s->impl.get());
}

const float *EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p,
    const EdgevoxOnnxOnlineStream *s) {
  std::vector<float> v = p->impl->Compute(s->impl.get());
  float *ans = new float[v.size()];
  std::copy(v.begin(), v.end(), ans);
  return ans;
}

void EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(const float *v) {
  if (!v) return;
  delete[] v;
}

struct EdgevoxOnnxSpeakerEmbeddingManager {
  std::unique_ptr<edgevox_onnx::SpeakerEmbeddingManager> impl;
};

const EdgevoxOnnxSpeakerEmbeddingManager *
EdgevoxOnnxCreateSpeakerEmbeddingManager(int32_t dim) {
  auto p = new EdgevoxOnnxSpeakerEmbeddingManager;
  p->impl = std::make_unique<edgevox_onnx::SpeakerEmbeddingManager>(dim);
  return p;
}

void EdgevoxOnnxDestroySpeakerEmbeddingManager(
    const EdgevoxOnnxSpeakerEmbeddingManager *p) {
  if (!p) return;
  delete p;
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerAdd(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float *v) {
  return p->impl->Add(name, v);
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerAddList(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float **v) {
  int32_t n = 0;
  auto q = v;
  while (q && q[0]) {
    ++n;
    ++q;
  }

  if (n == 0) {
    EDGEVOX_ONNX_LOGE("Empty embedding!");
    return 0;
  }

  std::vector<std::vector<float>> vec(n);
  int32_t dim = p->impl->Dim();

  for (int32_t i = 0; i != n; ++i) {
    vec[i] = std::vector<float>(v[i], v[i] + dim);
  }

  return p->impl->Add(name, vec);
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float *v, int32_t n) {
  std::vector<std::vector<float>> vec(n);

  int32_t dim = p->impl->Dim();

  for (int32_t i = 0; i != n; ++i, v += dim) {
    vec[i] = std::vector<float>(v, v + dim);
  }

  return p->impl->Add(name, vec);
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerRemove(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name) {
  return p->impl->Remove(name);
}

const char *EdgevoxOnnxSpeakerEmbeddingManagerSearch(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const float *v,
    float threshold) {
  auto r = p->impl->Search(v, threshold);
  if (r.empty()) {
    return nullptr;
  }

  char *name = new char[r.size() + 1];
  std::copy(r.begin(), r.end(), name);
  name[r.size()] = '\0';

  return name;
}

void EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch(const char *name) {
  if (!name) return;
  delete[] name;
}

const EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult *
EdgevoxOnnxSpeakerEmbeddingManagerGetBestMatches(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const float *v, float threshold,
    int32_t n) {
  auto matches = p->impl->GetBestMatches(v, threshold, n);

  if (matches.empty()) {
    return nullptr;
  }

  auto resultMatches =
      new EdgevoxOnnxSpeakerEmbeddingManagerSpeakerMatch[matches.size()];
  for (int i = 0; i < matches.size(); ++i) {
    resultMatches[i].score = matches[i].score;

    char *name = new char[matches[i].name.size() + 1];
    std::copy(matches[i].name.begin(), matches[i].name.end(), name);
    name[matches[i].name.size()] = '\0';

    resultMatches[i].name = name;
  }

  auto *result = new EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult();
  result->count = matches.size();
  result->matches = resultMatches;

  return result;
}

void EdgevoxOnnxSpeakerEmbeddingManagerFreeBestMatches(
    const EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult *r) {
  if (r == nullptr) {
    return;
  }

  for (int32_t i = 0; i < r->count; ++i) {
    delete[] r->matches[i].name;
  }
  delete[] r->matches;
  delete r;
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerVerify(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float *v, float threshold) {
  return p->impl->Verify(name, v, threshold);
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerContains(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name) {
  return p->impl->Contains(name);
}

int32_t EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers(
    const EdgevoxOnnxSpeakerEmbeddingManager *p) {
  return p->impl->NumSpeakers();
}

const char *const *EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers(
    const EdgevoxOnnxSpeakerEmbeddingManager *manager) {
  std::vector<std::string> all_speakers = manager->impl->GetAllSpeakers();
  int32_t num_speakers = all_speakers.size();
  char **p = new char *[num_speakers + 1];
  p[num_speakers] = nullptr;

  int32_t i = 0;
  for (const auto &name : all_speakers) {
    p[i] = new char[name.size() + 1];
    std::copy(name.begin(), name.end(), p[i]);
    p[i][name.size()] = '\0';

    i += 1;
  }
  return p;
}

void EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers(
    const char *const *names) {
  auto p = names;

  while (p && p[0]) {
    delete[] p[0];
    ++p;
  }

  delete[] names;
}

struct EdgevoxOnnxAudioTagging {
  std::unique_ptr<edgevox_onnx::AudioTagging> impl;
};

const EdgevoxOnnxAudioTagging *EdgevoxOnnxCreateAudioTagging(
    const EdgevoxOnnxAudioTaggingConfig *config) {
  edgevox_onnx::AudioTaggingConfig ac;
  ac.model.zipformer.model = EDGEVOX_ONNX_OR(config->model.zipformer.model, "");
  ac.model.ced = EDGEVOX_ONNX_OR(config->model.ced, "");
  ac.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  ac.model.debug = config->model.debug;
  ac.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  if (ac.model.provider.empty()) {
    ac.model.provider = "cpu";
  }

  ac.labels = EDGEVOX_ONNX_OR(config->labels, "");
  ac.top_k = EDGEVOX_ONNX_OR(config->top_k, 5);

  if (ac.model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", ac.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", ac.ToString().c_str());
#endif
  }

  if (!ac.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxAudioTagging *tagger = new EdgevoxOnnxAudioTagging;
  tagger->impl = std::make_unique<edgevox_onnx::AudioTagging>(ac);

  return tagger;
}

void EdgevoxOnnxDestroyAudioTagging(const EdgevoxOnnxAudioTagging *tagger) {
  if (!tagger) return;
  delete tagger;
}

const EdgevoxOnnxOfflineStream *EdgevoxOnnxAudioTaggingCreateOfflineStream(
    const EdgevoxOnnxAudioTagging *tagger) {
  const EdgevoxOnnxOfflineStream *stream =
      new EdgevoxOnnxOfflineStream(tagger->impl->CreateStream());
  return stream;
}

const EdgevoxOnnxAudioEvent *const *EdgevoxOnnxAudioTaggingCompute(
    const EdgevoxOnnxAudioTagging *tagger, const EdgevoxOnnxOfflineStream *s,
    int32_t top_k) {
  std::vector<edgevox_onnx::AudioEvent> events =
      tagger->impl->Compute(s->impl.get(), top_k);

  int32_t n = static_cast<int32_t>(events.size());
  EdgevoxOnnxAudioEvent **ans = new EdgevoxOnnxAudioEvent *[n + 1];
  ans[n] = nullptr;

  int32_t i = 0;
  for (const auto &e : events) {
    EdgevoxOnnxAudioEvent *p = new EdgevoxOnnxAudioEvent;

    char *name = new char[e.name.size() + 1];
    std::copy(e.name.begin(), e.name.end(), name);
    name[e.name.size()] = 0;

    p->name = name;

    p->index = e.index;
    p->prob = e.prob;

    ans[i] = p;
    i += 1;
  }

  return ans;
}

void EdgevoxOnnxAudioTaggingFreeResults(
    const EdgevoxOnnxAudioEvent *const *events) {
  auto p = events;

  while (p && *p) {
    auto e = *p;

    delete[] e->name;
    delete e;

    ++p;
  }

  delete[] events;
}

struct EdgevoxOnnxOfflinePunctuation {
  std::unique_ptr<edgevox_onnx::OfflinePunctuation> impl;
};

static edgevox_onnx::OfflinePunctuationConfig GetOfflinePunctuationConfig(
    const EdgevoxOnnxOfflinePunctuationConfig *config) {
  edgevox_onnx::OfflinePunctuationConfig c;
  c.model.ct_transformer = EDGEVOX_ONNX_OR(config->model.ct_transformer, "");
  c.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  c.model.debug = config->model.debug;
  c.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  if (c.model.provider.empty()) {
    c.model.provider = "cpu";
  }

  if (config->model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxOfflinePunctuation *EdgevoxOnnxCreateOfflinePunctuation(
    const EdgevoxOnnxOfflinePunctuationConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto c = GetOfflinePunctuationConfig(config);

  if (!c.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflinePunctuation *punct = new EdgevoxOnnxOfflinePunctuation;
  punct->impl = std::make_unique<edgevox_onnx::OfflinePunctuation>(c);

  return punct;
}

void EdgevoxOnnxDestroyOfflinePunctuation(
    const EdgevoxOnnxOfflinePunctuation *punct) {
  if (!punct) return;
  delete punct;
}

const char *SherpaOfflinePunctuationAddPunct(
    const EdgevoxOnnxOfflinePunctuation *punct, const char *text) {
  if (!punct || !text) return nullptr;
  std::string text_with_punct = punct->impl->AddPunctuation(text);

  char *ans = new char[text_with_punct.size() + 1];
  std::copy(text_with_punct.begin(), text_with_punct.end(), ans);
  ans[text_with_punct.size()] = 0;

  return ans;
}

void SherpaOfflinePunctuationFreeText(const char *text) {
  if (!text) return;
  delete[] text;
}

struct EdgevoxOnnxOnlinePunctuation {
  std::unique_ptr<edgevox_onnx::OnlinePunctuation> impl;
};

static edgevox_onnx::OnlinePunctuationConfig GetOnlinePunctuationConfig(
    const EdgevoxOnnxOnlinePunctuationConfig *config) {
  edgevox_onnx::OnlinePunctuationConfig punctuation_config;
  punctuation_config.model.cnn_bilstm =
      EDGEVOX_ONNX_OR(config->model.cnn_bilstm, "");
  punctuation_config.model.bpe_vocab =
      EDGEVOX_ONNX_OR(config->model.bpe_vocab, "");
  punctuation_config.model.num_threads =
      EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  punctuation_config.model.debug = config->model.debug;
  punctuation_config.model.provider =
      EDGEVOX_ONNX_OR(config->model.provider, "cpu");

  if (config->model.debug) {
#if __OHOS__
    auto str_vec = edgevox_onnx::SplitString(punctuation_config.ToString(), 128);
    for (const auto &s : str_vec) {
      EDGEVOX_ONNX_LOGE("%{public}s\n", s.c_str());
      EDGEVOX_ONNX_LOGE("%s\n", s.c_str());
    }
#else
    EDGEVOX_ONNX_LOGE("%s", punctuation_config.ToString().c_str());
#endif
  }

  return punctuation_config;
}

const EdgevoxOnnxOnlinePunctuation *EdgevoxOnnxCreateOnlinePunctuation(
    const EdgevoxOnnxOnlinePunctuationConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto punctuation_config = GetOnlinePunctuationConfig(config);
  if (!punctuation_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  auto *p = new EdgevoxOnnxOnlinePunctuation;
  p->impl =
      std::make_unique<edgevox_onnx::OnlinePunctuation>(punctuation_config);
  return p;
}

void EdgevoxOnnxDestroyOnlinePunctuation(const EdgevoxOnnxOnlinePunctuation *p) {
  if (!p) return;
  delete p;
}

const char *EdgevoxOnnxOnlinePunctuationAddPunct(
    const EdgevoxOnnxOnlinePunctuation *punctuation, const char *text) {
  if (!punctuation || !text) return nullptr;

  try {
    std::string s = punctuation->impl->AddPunctuationWithCase(text);
    char *p = new char[s.size() + 1];
    std::copy(s.begin(), s.end(), p);
    p[s.size()] = '\0';
    return p;
  } catch (const std::exception &e) {
    EDGEVOX_ONNX_LOGE("Failed to add punctuation: %s", e.what());
    return nullptr;
  }
}

void EdgevoxOnnxOnlinePunctuationFreeText(const char *text) {
  if (!text) return;
  delete[] text;
}

struct EdgevoxOnnxLinearResampler {
  std::unique_ptr<edgevox_onnx::LinearResample> impl;
};

const EdgevoxOnnxLinearResampler *EdgevoxOnnxCreateLinearResampler(
    int32_t samp_rate_in_hz, int32_t samp_rate_out_hz, float filter_cutoff_hz,
    int32_t num_zeros) {
  if (samp_rate_in_hz <= 0 || samp_rate_out_hz <= 0) {
    EDGEVOX_ONNX_LOGE("Sample rates must be > 0. Given in=%d out=%d",
                     samp_rate_in_hz, samp_rate_out_hz);
    return nullptr;
  }
  if (num_zeros < 0) {
    EDGEVOX_ONNX_LOGE("num_zeros must be >= 0. Given %d", num_zeros);
    return nullptr;
  }
  if (filter_cutoff_hz == 0) {
    float min_freq = std::min(samp_rate_in_hz, samp_rate_out_hz);
    filter_cutoff_hz = 0.99f * 0.5f * min_freq;
  }

  if (num_zeros == 0) {
    num_zeros = 6;
  }

  EdgevoxOnnxLinearResampler *p = new EdgevoxOnnxLinearResampler;
  p->impl = std::make_unique<edgevox_onnx::LinearResample>(
      samp_rate_in_hz, samp_rate_out_hz, filter_cutoff_hz, num_zeros);

  return p;
}

void EdgevoxOnnxDestroyLinearResampler(const EdgevoxOnnxLinearResampler *p) {
  if (!p) return;
  delete p;
}

const EdgevoxOnnxResampleOut *EdgevoxOnnxLinearResamplerResample(
    const EdgevoxOnnxLinearResampler *p, const float *input, int32_t input_dim,
    int32_t flush) {
  if (!p) return nullptr;
  std::vector<float> o;
  p->impl->Resample(input, input_dim, flush, &o);

  float *s = new float[o.size()];
  std::copy(o.begin(), o.end(), s);

  EdgevoxOnnxResampleOut *ans = new EdgevoxOnnxResampleOut;
  ans->samples = s;
  ans->n = static_cast<int32_t>(o.size());

  return ans;
}

void EdgevoxOnnxLinearResamplerResampleFree(const EdgevoxOnnxResampleOut *p) {
  if (!p) return;
  delete[] p->samples;
  delete p;
}

int32_t EdgevoxOnnxLinearResamplerResampleGetInputSampleRate(
    const EdgevoxOnnxLinearResampler *p) {
  if (!p) return 0;
  return p->impl->GetInputSamplingRate();
}

int32_t EdgevoxOnnxLinearResamplerResampleGetOutputSampleRate(
    const EdgevoxOnnxLinearResampler *p) {
  if (!p) return 0;
  return p->impl->GetOutputSamplingRate();
}

void EdgevoxOnnxLinearResamplerReset(const EdgevoxOnnxLinearResampler *p) {
  if (!p) return;
  p->impl->Reset();
}

int32_t EdgevoxOnnxFileExists(const char *filename) {
  return edgevox_onnx::FileExists(filename);
}

struct EdgevoxOnnxOfflineSpeechDenoiser {
  std::unique_ptr<edgevox_onnx::OfflineSpeechDenoiser> impl;
};

static const EdgevoxOnnxDenoisedAudio *CreateDenoisedAudio(
    const edgevox_onnx::DenoisedAudio &audio) {
  auto ans = new EdgevoxOnnxDenoisedAudio;

  float *denoised_samples = nullptr;
  if (!audio.samples.empty()) {
    denoised_samples = new float[audio.samples.size()];
    std::copy(audio.samples.begin(), audio.samples.end(), denoised_samples);
  }

  ans->samples = denoised_samples;
  ans->n = audio.samples.size();
  ans->sample_rate = audio.sample_rate;

  return ans;
}

static edgevox_onnx::OfflineSpeechDenoiserConfig GetOfflineSpeechDenoiserConfig(
    const EdgevoxOnnxOfflineSpeechDenoiserConfig *config) {
  edgevox_onnx::OfflineSpeechDenoiserConfig c;
  c.model.gtcrn.model = EDGEVOX_ONNX_OR(config->model.gtcrn.model, "");
  c.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  c.model.debug = config->model.debug;
  c.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  c.model.dpdfnet.model = EDGEVOX_ONNX_OR(config->model.dpdfnet.model, "");

  if (c.model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxOfflineSpeechDenoiser *EdgevoxOnnxCreateOfflineSpeechDenoiser(
    const EdgevoxOnnxOfflineSpeechDenoiserConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto sd_config = GetOfflineSpeechDenoiserConfig(config);

  if (!sd_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflineSpeechDenoiser *sd = new EdgevoxOnnxOfflineSpeechDenoiser;

  sd->impl = std::make_unique<edgevox_onnx::OfflineSpeechDenoiser>(sd_config);

  return sd;
}

void EdgevoxOnnxDestroyOfflineSpeechDenoiser(
    const EdgevoxOnnxOfflineSpeechDenoiser *sd) {
  if (!sd) return;
  delete sd;
}

int32_t EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(
    const EdgevoxOnnxOfflineSpeechDenoiser *sd) {
  if (sd == nullptr) {
    return 0;
  }

  return sd->impl->GetSampleRate();
}

const EdgevoxOnnxDenoisedAudio *EdgevoxOnnxOfflineSpeechDenoiserRun(
    const EdgevoxOnnxOfflineSpeechDenoiser *sd, const float *samples, int32_t n,
    int32_t sample_rate) {
  if (sd == nullptr) {
    return nullptr;
  }

  if (samples == nullptr && n > 0) {
    return nullptr;
  }

  auto audio = sd->impl->Run(samples, n, sample_rate);
  return CreateDenoisedAudio(audio);
}

void EdgevoxOnnxDestroyDenoisedAudio(const EdgevoxOnnxDenoisedAudio *p) {
  if (!p) return;
  delete[] p->samples;
  delete p;
}

// =========================================================================
// Source separation
// =========================================================================

struct EdgevoxOnnxOfflineSourceSeparation {
  std::unique_ptr<edgevox_onnx::OfflineSourceSeparation> impl;
};

static edgevox_onnx::OfflineSourceSeparationConfig
GetOfflineSourceSeparationConfig(
    const EdgevoxOnnxOfflineSourceSeparationConfig *config) {
  edgevox_onnx::OfflineSourceSeparationConfig c;
  c.model.spleeter.vocals = EDGEVOX_ONNX_OR(config->model.spleeter.vocals, "");
  c.model.spleeter.accompaniment =
      EDGEVOX_ONNX_OR(config->model.spleeter.accompaniment, "");
  c.model.uvr.model = EDGEVOX_ONNX_OR(config->model.uvr.model, "");
  c.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  c.model.debug = config->model.debug;
  c.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");

  if (c.model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxOfflineSourceSeparation *
EdgevoxOnnxCreateOfflineSourceSeparation(
    const EdgevoxOnnxOfflineSourceSeparationConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto ss_config = GetOfflineSourceSeparationConfig(config);

  if (!ss_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in source separation config");
    return nullptr;
  }

  auto *ss = new EdgevoxOnnxOfflineSourceSeparation;
  ss->impl = std::make_unique<edgevox_onnx::OfflineSourceSeparation>(ss_config);

  return ss;
}

void EdgevoxOnnxDestroyOfflineSourceSeparation(
    const EdgevoxOnnxOfflineSourceSeparation *ss) {
  if (!ss) return;
  delete ss;
}

int32_t EdgevoxOnnxOfflineSourceSeparationGetOutputSampleRate(
    const EdgevoxOnnxOfflineSourceSeparation *ss) {
  return ss ? ss->impl->GetOutputSampleRate() : 0;
}

int32_t EdgevoxOnnxOfflineSourceSeparationGetNumberOfStems(
    const EdgevoxOnnxOfflineSourceSeparation *ss) {
  return ss ? ss->impl->GetNumberOfStems() : 0;
}

const EdgevoxOnnxSourceSeparationOutput *
EdgevoxOnnxOfflineSourceSeparationProcess(
    const EdgevoxOnnxOfflineSourceSeparation *ss, const float *const *samples,
    int32_t num_channels, int32_t num_samples, int32_t sample_rate) {
  if (ss == nullptr) {
    return nullptr;
  }

  if (samples == nullptr) {
    return nullptr;
  }

  edgevox_onnx::OfflineSourceSeparationInput input;
  input.sample_rate = sample_rate;
  input.samples.data.resize(num_channels);
  for (int32_t i = 0; i < num_channels; ++i) {
    input.samples.data[i].assign(samples[i], samples[i] + num_samples);
  }

  auto output = ss->impl->Process(input);

  auto *ans = new EdgevoxOnnxSourceSeparationOutput;
  ans->sample_rate = output.sample_rate;
  ans->num_stems = static_cast<int32_t>(output.stems.size());

  auto *stems = new EdgevoxOnnxSourceSeparationStem[ans->num_stems];
  for (int32_t s = 0; s < ans->num_stems; ++s) {
    auto &stem = output.stems[s];
    int32_t nc = static_cast<int32_t>(stem.data.size());
    stems[s].num_channels = nc;
    stems[s].n = nc > 0 ? static_cast<int32_t>(stem.data[0].size()) : 0;

    auto **channel_data = new float *[nc];
    for (int32_t c = 0; c < nc; ++c) {
      channel_data[c] = new float[stem.data[c].size()];
      std::copy(stem.data[c].begin(), stem.data[c].end(), channel_data[c]);
    }
    stems[s].samples = channel_data;
  }
  ans->stems = stems;

  return ans;
}

void EdgevoxOnnxDestroySourceSeparationOutput(
    const EdgevoxOnnxSourceSeparationOutput *p) {
  if (!p) return;
  for (int32_t s = 0; s < p->num_stems; ++s) {
    for (int32_t c = 0; c < p->stems[s].num_channels; ++c) {
      delete[] p->stems[s].samples[c];
    }
    delete[] p->stems[s].samples;
  }
  delete[] p->stems;
  delete p;
}

struct EdgevoxOnnxOnlineSpeechDenoiser {
  std::unique_ptr<edgevox_onnx::OnlineSpeechDenoiser> impl;
};

static edgevox_onnx::OnlineSpeechDenoiserConfig GetOnlineSpeechDenoiserConfig(
    const EdgevoxOnnxOnlineSpeechDenoiserConfig *config) {
  edgevox_onnx::OnlineSpeechDenoiserConfig c;
  c.model.gtcrn.model = EDGEVOX_ONNX_OR(config->model.gtcrn.model, "");
  c.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  c.model.debug = config->model.debug;
  c.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  c.model.dpdfnet.model = EDGEVOX_ONNX_OR(config->model.dpdfnet.model, "");

  if (c.model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxOnlineSpeechDenoiser *EdgevoxOnnxCreateOnlineSpeechDenoiser(
    const EdgevoxOnnxOnlineSpeechDenoiserConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto sd_config = GetOnlineSpeechDenoiserConfig(config);

  if (!sd_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  auto *sd = new EdgevoxOnnxOnlineSpeechDenoiser;
  sd->impl = std::make_unique<edgevox_onnx::OnlineSpeechDenoiser>(sd_config);
  return sd;
}

void EdgevoxOnnxDestroyOnlineSpeechDenoiser(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd) {
  if (!sd) return;
  delete sd;
}

int32_t EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd) {
  if (sd == nullptr) {
    return 0;
  }

  return sd->impl->GetSampleRate();
}

int32_t EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd) {
  if (sd == nullptr) {
    return 0;
  }

  return sd->impl->GetFrameShiftInSamples();
}

const EdgevoxOnnxDenoisedAudio *EdgevoxOnnxOnlineSpeechDenoiserRun(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd, const float *samples, int32_t n,
    int32_t sample_rate) {
  if (sd == nullptr) {
    return nullptr;
  }

  if (samples == nullptr && n > 0) {
    return nullptr;
  }

  auto audio = sd->impl->Run(samples, n, sample_rate);

  if (audio.samples.empty()) {
    return nullptr;
  }

  return CreateDenoisedAudio(audio);
}

const EdgevoxOnnxDenoisedAudio *EdgevoxOnnxOnlineSpeechDenoiserFlush(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd) {
  if (sd == nullptr) {
    return nullptr;
  }

  auto audio = sd->impl->Flush();

  if (audio.samples.empty()) {
    return nullptr;
  }

  return CreateDenoisedAudio(audio);
}

void EdgevoxOnnxOnlineSpeechDenoiserReset(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd) {
  if (sd == nullptr) {
    return;
  }

  sd->impl->Reset();
}

#if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1

struct EdgevoxOnnxOfflineSpeakerDiarization {
  std::unique_ptr<edgevox_onnx::OfflineSpeakerDiarization> impl;
};

struct EdgevoxOnnxOfflineSpeakerDiarizationResult {
  edgevox_onnx::OfflineSpeakerDiarizationResult impl;
};

static edgevox_onnx::OfflineSpeakerDiarizationConfig
GetOfflineSpeakerDiarizationConfig(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config) {
  edgevox_onnx::OfflineSpeakerDiarizationConfig sd_config;

  sd_config.segmentation.pyannote.model =
      EDGEVOX_ONNX_OR(config->segmentation.pyannote.model, "");
  sd_config.segmentation.num_threads =
      EDGEVOX_ONNX_OR(config->segmentation.num_threads, 1);
  sd_config.segmentation.debug = config->segmentation.debug;
  sd_config.segmentation.provider =
      EDGEVOX_ONNX_OR(config->segmentation.provider, "cpu");
  if (sd_config.segmentation.provider.empty()) {
    sd_config.segmentation.provider = "cpu";
  }

  sd_config.embedding.model = EDGEVOX_ONNX_OR(config->embedding.model, "");
  sd_config.embedding.num_threads =
      EDGEVOX_ONNX_OR(config->embedding.num_threads, 1);
  sd_config.embedding.debug = config->embedding.debug;
  sd_config.embedding.provider =
      EDGEVOX_ONNX_OR(config->embedding.provider, "cpu");
  if (sd_config.embedding.provider.empty()) {
    sd_config.embedding.provider = "cpu";
  }

  sd_config.clustering.num_clusters =
      EDGEVOX_ONNX_OR(config->clustering.num_clusters, -1);

  sd_config.clustering.threshold =
      EDGEVOX_ONNX_OR(config->clustering.threshold, 0.5);

  sd_config.min_duration_on = EDGEVOX_ONNX_OR(config->min_duration_on, 0.3);

  sd_config.min_duration_off = EDGEVOX_ONNX_OR(config->min_duration_off, 0.5);

  if (sd_config.segmentation.debug || sd_config.embedding.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", sd_config.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", sd_config.ToString().c_str());
#endif
  }

  return sd_config;
}

const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config) {
  auto sd_config = GetOfflineSpeakerDiarizationConfig(config);

  if (!sd_config.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflineSpeakerDiarization *sd =
      new EdgevoxOnnxOfflineSpeakerDiarization;

  sd->impl =
      std::make_unique<edgevox_onnx::OfflineSpeakerDiarization>(sd_config);

  return sd;
}

void EdgevoxOnnxDestroyOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd) {
  if (!sd) return;
  delete sd;
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd) {
  return sd->impl->SampleRate();
}

void EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd,
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config) {
  edgevox_onnx::OfflineSpeakerDiarizationConfig sd_config;

  sd_config.clustering.num_clusters =
      EDGEVOX_ONNX_OR(config->clustering.num_clusters, -1);

  sd_config.clustering.threshold =
      EDGEVOX_ONNX_OR(config->clustering.threshold, 0.5);

  sd->impl->SetConfig(sd_config);
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  return r->impl.NumSpeakers();
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  return r->impl.NumSegments();
}

const EdgevoxOnnxOfflineSpeakerDiarizationSegment *
EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  if (r->impl.NumSegments() == 0) {
    return nullptr;
  }

  auto segments = r->impl.SortByStartTime();

  int32_t n = segments.size();
  EdgevoxOnnxOfflineSpeakerDiarizationSegment *ans =
      new EdgevoxOnnxOfflineSpeakerDiarizationSegment[n];

  for (int32_t i = 0; i != n; ++i) {
    const auto &s = segments[i];

    ans[i].start = s.Start();
    ans[i].end = s.End();
    ans[i].speaker = s.Speaker();
  }

  return ans;
}

void EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(
    const EdgevoxOnnxOfflineSpeakerDiarizationSegment *s) {
  if (!s) return;
  delete[] s;
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcess(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n) {
  auto ans = new EdgevoxOnnxOfflineSpeakerDiarizationResult;
  ans->impl = sd->impl->Process(samples, n);

  return ans;
}

void EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  if (!r) return;
  delete r;
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n, EdgevoxOnnxOfflineSpeakerDiarizationProgressCallback callback,
    void *arg) {
  auto ans = new EdgevoxOnnxOfflineSpeakerDiarizationResult;
  ans->impl = sd->impl->Process(samples, n, callback, arg);

  return ans;
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n,
    EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg callback) {
  auto wrapper = [callback](int32_t num_processed_chunks,
                            int32_t num_total_chunks, void *) {
    return callback(num_processed_chunks, num_total_chunks);
  };

  auto ans = new EdgevoxOnnxOfflineSpeakerDiarizationResult;
  ans->impl = sd->impl->Process(samples, n, wrapper);

  return ans;
}
#else

const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

void EdgevoxOnnxDestroyOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return 0;
}

void EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd,
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return 0;
}

int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return 0;
}

const EdgevoxOnnxOfflineSpeakerDiarizationSegment *
EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

void EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(
    const EdgevoxOnnxOfflineSpeakerDiarizationSegment *s) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcess(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n, EdgevoxOnnxOfflineSpeakerDiarizationProgressCallback callback,
    void *arg) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n,
    EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg callback) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

void EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
}

#endif

#ifdef __OHOS__

const EdgevoxOnnxOfflineSpeechDenoiser *
EdgevoxOnnxCreateOfflineSpeechDenoiserOHOS(
    const EdgevoxOnnxOfflineSpeechDenoiserConfig *config,
    NativeResourceManager *mgr) {
  if (config == nullptr) {
    return nullptr;
  }

  if (!mgr) {
    return EdgevoxOnnxCreateOfflineSpeechDenoiser(config);
  }

  auto sd_config = GetOfflineSpeechDenoiserConfig(config);

  EdgevoxOnnxOfflineSpeechDenoiser *sd = new EdgevoxOnnxOfflineSpeechDenoiser;

  sd->impl =
      std::make_unique<edgevox_onnx::OfflineSpeechDenoiser>(mgr, sd_config);

  return sd;
}

const EdgevoxOnnxOnlineSpeechDenoiser *EdgevoxOnnxCreateOnlineSpeechDenoiserOHOS(
    const EdgevoxOnnxOnlineSpeechDenoiserConfig *config,
    NativeResourceManager *mgr) {
  if (config == nullptr) {
    return nullptr;
  }

  if (mgr == nullptr) {
    return EdgevoxOnnxCreateOnlineSpeechDenoiser(config);
  }

  auto sd_config = GetOnlineSpeechDenoiserConfig(config);

  auto *sd = new EdgevoxOnnxOnlineSpeechDenoiser;
  sd->impl =
      std::make_unique<edgevox_onnx::OnlineSpeechDenoiser>(mgr, sd_config);

  return sd;
}

const EdgevoxOnnxOnlineRecognizer *EdgevoxOnnxCreateOnlineRecognizerOHOS(
    const EdgevoxOnnxOnlineRecognizerConfig *config,
    NativeResourceManager *mgr) {
  if (!mgr) {
    return EdgevoxOnnxCreateOnlineRecognizer(config);
  }

  edgevox_onnx::OnlineRecognizerConfig recognizer_config =
      GetOnlineRecognizerConfig(config);

  EdgevoxOnnxOnlineRecognizer *recognizer = new EdgevoxOnnxOnlineRecognizer;

  recognizer->impl =
      std::make_unique<edgevox_onnx::OnlineRecognizer>(mgr, recognizer_config);

  return recognizer;
}

const EdgevoxOnnxOnlinePunctuation *EdgevoxOnnxCreateOnlinePunctuationOHOS(
    const EdgevoxOnnxOnlinePunctuationConfig *config,
    NativeResourceManager *mgr) {
  if (config == nullptr) {
    return nullptr;
  }

  if (mgr == nullptr) {
    return EdgevoxOnnxCreateOnlinePunctuation(config);
  }

  auto punctuation_config = GetOnlinePunctuationConfig(config);
  auto *p = new EdgevoxOnnxOnlinePunctuation;
  p->impl =
      std::make_unique<edgevox_onnx::OnlinePunctuation>(mgr, punctuation_config);
  return p;
}

const EdgevoxOnnxOfflineRecognizer *EdgevoxOnnxCreateOfflineRecognizerOHOS(
    const EdgevoxOnnxOfflineRecognizerConfig *config,
    NativeResourceManager *mgr) {
  if (mgr == nullptr) {
    return EdgevoxOnnxCreateOfflineRecognizer(config);
  }

  edgevox_onnx::OfflineRecognizerConfig recognizer_config =
      GetOfflineRecognizerConfig(config);

  EdgevoxOnnxOfflineRecognizer *recognizer = new EdgevoxOnnxOfflineRecognizer;

  recognizer->impl =
      std::make_unique<edgevox_onnx::OfflineRecognizer>(mgr, recognizer_config);

  return recognizer;
}

const EdgevoxOnnxVoiceActivityDetector *
EdgevoxOnnxCreateVoiceActivityDetectorOHOS(
    const EdgevoxOnnxVadModelConfig *config, float buffer_size_in_seconds,
    NativeResourceManager *mgr) {
  if (mgr == nullptr) {
    return EdgevoxOnnxCreateVoiceActivityDetector(config,
                                                 buffer_size_in_seconds);
  }

  auto vad_config = GetVadModelConfig(config);

  EdgevoxOnnxVoiceActivityDetector *p = new EdgevoxOnnxVoiceActivityDetector;
  p->impl = std::make_unique<edgevox_onnx::VoiceActivityDetector>(
      mgr, vad_config, buffer_size_in_seconds);

  return p;
}

const EdgevoxOnnxSpeakerEmbeddingExtractor *
EdgevoxOnnxCreateSpeakerEmbeddingExtractorOHOS(
    const EdgevoxOnnxSpeakerEmbeddingExtractorConfig *config,
    NativeResourceManager *mgr) {
  if (!mgr) {
    return EdgevoxOnnxCreateSpeakerEmbeddingExtractor(config);
  }

  auto c = GetSpeakerEmbeddingExtractorConfig(config);

  auto p = new EdgevoxOnnxSpeakerEmbeddingExtractor;

  p->impl = std::make_unique<edgevox_onnx::SpeakerEmbeddingExtractor>(mgr, c);

  return p;
}

const EdgevoxOnnxKeywordSpotter *EdgevoxOnnxCreateKeywordSpotterOHOS(
    const EdgevoxOnnxKeywordSpotterConfig *config, NativeResourceManager *mgr) {
  if (!mgr) {
    return EdgevoxOnnxCreateKeywordSpotter(config);
  }

  auto spotter_config = GetKeywordSpotterConfig(config);

  EdgevoxOnnxKeywordSpotter *spotter = new EdgevoxOnnxKeywordSpotter;

  spotter->impl =
      std::make_unique<edgevox_onnx::KeywordSpotter>(mgr, spotter_config);

  return spotter;
}

#if EDGEVOX_ONNX_ENABLE_TTS == 1
const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTtsOHOS(
    const EdgevoxOnnxOfflineTtsConfig *config, NativeResourceManager *mgr) {
  if (!mgr) {
    return EdgevoxOnnxCreateOfflineTts(config);
  }

  auto tts_config = GetOfflineTtsConfig(config);

  EdgevoxOnnxOfflineTts *tts = new EdgevoxOnnxOfflineTts;

  tts->impl = std::make_unique<edgevox_onnx::OfflineTts>(mgr, tts_config);

  return tts;
}
#else
const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTtsOHOS(
    const EdgevoxOnnxOfflineTtsConfig *config, NativeResourceManager *mgr) {
  EDGEVOX_ONNX_LOGE("TTS is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}
#endif  // #if EDGEVOX_ONNX_ENABLE_TTS == 1

const EdgevoxOnnxOfflinePunctuation *EdgevoxOnnxCreateOfflinePunctuationOHOS(
    const EdgevoxOnnxOfflinePunctuationConfig *config,
    NativeResourceManager *mgr) {
  if (config == nullptr) {
    return nullptr;
  }

  if (!mgr) {
    return EdgevoxOnnxCreateOfflinePunctuation(config);
  }

  auto c = GetOfflinePunctuationConfig(config);
  if (c.model.ct_transformer.empty()) {
    EDGEVOX_ONNX_LOGE(
        "Please specify a punctuation model! Return a null pointer");
    return nullptr;
  }

  auto *punct = new EdgevoxOnnxOfflinePunctuation;
  punct->impl = std::make_unique<edgevox_onnx::OfflinePunctuation>(mgr, c);

  return punct;
}

#if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1
const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarizationOHOS(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config,
    NativeResourceManager *mgr) {
  if (!mgr) {
    return EdgevoxOnnxCreateOfflineSpeakerDiarization(config);
  }

  auto sd_config = GetOfflineSpeakerDiarizationConfig(config);

  EdgevoxOnnxOfflineSpeakerDiarization *sd =
      new EdgevoxOnnxOfflineSpeakerDiarization;

  sd->impl =
      std::make_unique<edgevox_onnx::OfflineSpeakerDiarization>(mgr, sd_config);

  return sd;
}
#else

const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarizationOHOS(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config,
    NativeResourceManager *mgr) {
  EDGEVOX_ONNX_LOGE(
      "Speaker diarization is not enabled. Please rebuild edgevox-onnx");
  return nullptr;
}

#endif  // #if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1

const EdgevoxOnnxOfflineSourceSeparation *
EdgevoxOnnxCreateOfflineSourceSeparationOHOS(
    const EdgevoxOnnxOfflineSourceSeparationConfig *config,
    NativeResourceManager *mgr) {
  if (config == nullptr) {
    return nullptr;
  }

  if (!mgr) {
    return EdgevoxOnnxCreateOfflineSourceSeparation(config);
  }

  auto ss_config = GetOfflineSourceSeparationConfig(config);

  auto *ss = new EdgevoxOnnxOfflineSourceSeparation;
  ss->impl =
      std::make_unique<edgevox_onnx::OfflineSourceSeparation>(mgr, ss_config);

  return ss;
}

#endif  // #ifdef __OHOS__

struct EdgevoxOnnxOfflineDiacritization {
  std::unique_ptr<edgevox_onnx::OfflineDiacritization> impl;
};

static edgevox_onnx::OfflineDiacritizationConfig GetOfflineDiacritizationConfig(
    const EdgevoxOnnxOfflineDiacritizationConfig *config) {
  edgevox_onnx::OfflineDiacritizationConfig c;
  c.model.catt_encoder = EDGEVOX_ONNX_OR(config->model.catt_encoder, "");
  c.model.catt_decoder = EDGEVOX_ONNX_OR(config->model.catt_decoder, "");
  c.model.num_threads = EDGEVOX_ONNX_OR(config->model.num_threads, 1);
  c.model.debug = config->model.debug;
  c.model.provider = EDGEVOX_ONNX_OR(config->model.provider, "cpu");
  if (c.model.provider.empty()) {
    c.model.provider = "cpu";
  }

  if (config->model.debug) {
#if __OHOS__
    EDGEVOX_ONNX_LOGE("%{public}s\n", c.ToString().c_str());
#else
    EDGEVOX_ONNX_LOGE("%s\n", c.ToString().c_str());
#endif
  }

  return c;
}

const EdgevoxOnnxOfflineDiacritization *EdgevoxOnnxCreateOfflineDiacritization(
    const EdgevoxOnnxOfflineDiacritizationConfig *config) {
  if (config == nullptr) {
    return nullptr;
  }

  auto cfg = GetOfflineDiacritizationConfig(config);

  if (!cfg.Validate()) {
    EDGEVOX_ONNX_LOGE("Errors in config");
    return nullptr;
  }

  EdgevoxOnnxOfflineDiacritization *diacrt = new EdgevoxOnnxOfflineDiacritization;
  diacrt->impl = std::make_unique<edgevox_onnx::OfflineDiacritization>(cfg);

  return diacrt;
}

void EdgevoxOnnxDestroyOfflineDiacritization(
    const EdgevoxOnnxOfflineDiacritization *diacrt) {
  if (!diacrt) return;
  delete diacrt;
}

const char *SherpaOfflineDiacritizationAddDiacritics(
    const EdgevoxOnnxOfflineDiacritization *diacrt, const char *text) {
  if (!diacrt || !text) return nullptr;
  std::string text_with_diacritics = diacrt->impl->AddDiacritics(text);

  char *ans = new char[text_with_diacritics.size() + 1];
  std::copy(text_with_diacritics.begin(), text_with_diacritics.end(), ans);
  ans[text_with_diacritics.size()] = 0;

  return ans;
}

void SherpaOfflineDiacritizationFreeText(const char *text) {
  if (!text) return;
  delete[] text;
}
