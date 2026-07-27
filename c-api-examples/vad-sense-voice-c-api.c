// c-api-examples/vad-sense-voice-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

//
// This file demonstrates how to use VAD + SenseVoice with edgevox-onnx's C API.
// clang-format off
//
// To use silero-vad:
//  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/silero_vad.onnx
//
// To use ten-vad:
//  wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/ten-vad.onnx
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/lei-jun-test.wav
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
// tar xvf edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
// rm edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename = "./lei-jun-test.wav";
  if (!EdgevoxOnnxFileExists(wav_filename)) {
    fprintf(stderr, "Please download %s\n", wav_filename);
    return -1;
  }

  const char *vad_filename;
  int32_t use_silero_vad = 0;
  int32_t use_ten_vad = 0;

  if (EdgevoxOnnxFileExists("./silero_vad.onnx")) {
    printf("Use silero-vad\n");
    vad_filename = "./silero_vad.onnx";
    use_silero_vad = 1;
  } else if (EdgevoxOnnxFileExists("./ten-vad.onnx")) {
    printf("Use ten-vad\n");
    vad_filename = "./ten-vad.onnx";
    use_ten_vad = 1;
  } else {
    fprintf(stderr, "Please provide either silero_vad.onnx or ten-vad.onnx\n");
    return -1;
  }

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

  if (wave->sample_rate != 16000) {
    fprintf(stderr, "Expect the sample rate to be 16000. Given: %d\n",
            wave->sample_rate);
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  // Recognizer config
  EdgevoxOnnxOfflineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config.debug = 0;
  recognizer_config.model_config.num_threads = 1;
  recognizer_config.model_config.provider = provider;
  recognizer_config.model_config.tokens = tokens_filename;
  recognizer_config.model_config.sense_voice.model = model_filename;
  recognizer_config.model_config.sense_voice.language = language;
  recognizer_config.model_config.sense_voice.use_itn = use_inverse_text_normalization;

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      EdgevoxOnnxCreateOfflineRecognizer(&recognizer_config);

  if (recognizer == NULL) {
    fprintf(stderr, "Please check your recognizer config!\n");
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  EdgevoxOnnxVadModelConfig vadConfig;
  memset(&vadConfig, 0, sizeof(vadConfig));

  if (use_silero_vad) {
    vadConfig.silero_vad.model = vad_filename;
    vadConfig.silero_vad.threshold = 0.25;
    vadConfig.silero_vad.min_silence_duration = 0.5;
    vadConfig.silero_vad.min_speech_duration = 0.5;
    vadConfig.silero_vad.max_speech_duration = 10;
    vadConfig.silero_vad.window_size = 512;
  } else if (use_ten_vad) {
    vadConfig.ten_vad.model = vad_filename;
    vadConfig.ten_vad.threshold = 0.25;
    vadConfig.ten_vad.min_silence_duration = 0.5;
    vadConfig.ten_vad.min_speech_duration = 0.5;
    vadConfig.ten_vad.max_speech_duration = 10;
    vadConfig.ten_vad.window_size = 256;
  }

  vadConfig.sample_rate = 16000;
  vadConfig.num_threads = 1;
  vadConfig.debug = 1;

  const EdgevoxOnnxVoiceActivityDetector *vad =
      EdgevoxOnnxCreateVoiceActivityDetector(&vadConfig, 30);

  if (vad == NULL) {
    fprintf(stderr, "Please check your recognizer config!\n");
    EdgevoxOnnxFreeWave(wave);
    EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
    return -1;
  }

  int32_t window_size = use_silero_vad ? vadConfig.silero_vad.window_size
                                       : vadConfig.ten_vad.window_size;
  int32_t i = 0;
  int is_eof = 0;

  while (!is_eof) {
    if (i + window_size < wave->num_samples) {
      EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(vad, wave->samples + i,
                                                    window_size);
    } else {
      EdgevoxOnnxVoiceActivityDetectorFlush(vad);
      is_eof = 1;
    }

    while (!EdgevoxOnnxVoiceActivityDetectorEmpty(vad)) {
      const EdgevoxOnnxSpeechSegment *segment =
          EdgevoxOnnxVoiceActivityDetectorFront(vad);

      const EdgevoxOnnxOfflineStream *stream =
          EdgevoxOnnxCreateOfflineStream(recognizer);

      EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate,
                                      segment->samples, segment->n);

      EdgevoxOnnxDecodeOfflineStream(recognizer, stream);

      const EdgevoxOnnxOfflineRecognizerResult *result =
          EdgevoxOnnxGetOfflineStreamResult(stream);

      float start = segment->start / 16000.0f;
      float duration = segment->n / 16000.0f;
      float stop = start + duration;

      fprintf(stderr, "%.3f -- %.3f: %s\n", start, stop, result->text);

      EdgevoxOnnxDestroyOfflineRecognizerResult(result);
      EdgevoxOnnxDestroyOfflineStream(stream);

      EdgevoxOnnxDestroySpeechSegment(segment);
      EdgevoxOnnxVoiceActivityDetectorPop(vad);
    }
    i += window_size;
  }

  EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
  EdgevoxOnnxDestroyVoiceActivityDetector(vad);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
