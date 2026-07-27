// c-api-examples/sense-voice-with-hr-c-api.c
//
// Copyright (c)  2024-2025  Xiaomi Corporation

//
// This file demonstrates how to use SenseVoice with edgevox-onnx's C API
// with homophone replacer.
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
// tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
// rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/dict.tar.bz2
// tar xf dict.tar.bz2
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/replace.fst
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/test-hr.wav
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/hr-files/lexicon.txt
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename = "./test-hr.wav";
  const char *model_filename =
      "./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/model.int8.onnx";
  const char *tokens_filename =
      "./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/tokens.txt";
  const char *language = "auto";
  const char *provider = "cpu";
  int32_t use_inverse_text_normalization = 1;

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  // Recognizer config
  EdgevoxOnnxOfflineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config.debug = 1;
  recognizer_config.model_config.num_threads = 1;
  recognizer_config.model_config.provider = provider;
  recognizer_config.model_config.tokens = tokens_filename;
  recognizer_config.model_config.sense_voice.model = model_filename;
  recognizer_config.model_config.sense_voice.language = language;
  recognizer_config.model_config.sense_voice.use_itn = use_inverse_text_normalization;
  recognizer_config.hr.dict_dir = "./dict";
  recognizer_config.hr.lexicon = "./lexicon.txt";

  // Please see
  // https://colab.research.google.com/drive/1jEaS3s8FbRJIcVQJv2EQx19EM_mnuARi?usp=sharing
  // for how to generate your own replace.fst
  recognizer_config.hr.rule_fsts = "./replace.fst";

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      EdgevoxOnnxCreateOfflineRecognizer(&recognizer_config);

  if (recognizer == NULL) {
    fprintf(stderr, "Please check your config!\n");
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  const EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxCreateOfflineStream(recognizer);

  EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
                                  wave->num_samples);
  EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
  const EdgevoxOnnxOfflineRecognizerResult *result =
      EdgevoxOnnxGetOfflineStreamResult(stream);

  fprintf(stderr, "Decoded text: %s\n", result->text);

  EdgevoxOnnxDestroyOfflineRecognizerResult(result);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
