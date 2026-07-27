// c-api-examples/streaming-nemotron-c-api.c
//
// Copyright (c)  2026  Xiaomi Corporation

//
// This file demonstrates how to use streaming Nemotron with edgevox-onnx's C
// API.
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25.tar.bz2
// tar xvf edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25.tar.bz2
// rm edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25/"
      "test_wavs/0.wav";
  const char *encoder_filename =
      "edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25/"
      "encoder.int8.onnx";
  const char *decoder_filename =
      "edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25/"
      "decoder.int8.onnx";
  const char *joiner_filename =
      "edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25/"
      "joiner.int8.onnx";
  const char *tokens_filename =
      "edgevox-onnx-nemotron-speech-streaming-en-0.6b-560ms-int8-2026-04-25/"
      "tokens.txt";
  const char *provider = "cpu";

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    return -1;
  }

  // Recognizer config
  EdgevoxOnnxOnlineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config.debug = 1;
  recognizer_config.model_config.num_threads = 1;
  recognizer_config.model_config.provider = provider;
  recognizer_config.model_config.tokens = tokens_filename;
  recognizer_config.model_config.transducer.encoder = encoder_filename;
  recognizer_config.model_config.transducer.decoder = decoder_filename;
  recognizer_config.model_config.transducer.joiner = joiner_filename;
  recognizer_config.enable_endpoint = 1;

  const EdgevoxOnnxOnlineRecognizer *recognizer =
      EdgevoxOnnxCreateOnlineRecognizer(&recognizer_config);

  if (recognizer == NULL) {
    fprintf(stderr, "Please check your config!\n");
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  const EdgevoxOnnxOnlineStream *stream =
      EdgevoxOnnxCreateOnlineStream(recognizer);
  // Multilingual Nemotron models use the generic stream option "language".
  // For example: EdgevoxOnnxOnlineStreamSetOption(stream, "language", "ja");
  // Empty/unset means auto. English-only Nemotron ignores this option.
  EdgevoxOnnxOnlineStreamSetOption(stream, "language", "en");

  const EdgevoxOnnxDisplay *display = EdgevoxOnnxCreateDisplay(50);
  int32_t segment_id = 0;

// simulate streaming. You can choose an arbitrary N
#define N 3200

  fprintf(stderr, "sample rate: %d, num samples: %d, duration: %.2f s\n",
          wave->sample_rate, wave->num_samples,
          (float)wave->num_samples / wave->sample_rate);

  int32_t k = 0;
  while (k < wave->num_samples) {
    int32_t start = k;
    int32_t end =
        (start + N > wave->num_samples) ? wave->num_samples : (start + N);
    k += N;

    EdgevoxOnnxOnlineStreamAcceptWaveform(stream, wave->sample_rate,
                                         wave->samples + start, end - start);
    while (EdgevoxOnnxIsOnlineStreamReady(recognizer, stream)) {
      EdgevoxOnnxDecodeOnlineStream(recognizer, stream);
    }

    const EdgevoxOnnxOnlineRecognizerResult *r =
        EdgevoxOnnxGetOnlineStreamResult(recognizer, stream);

    if (strlen(r->text)) {
      EdgevoxOnnxPrint(display, segment_id, r->text);
    }

    if (EdgevoxOnnxOnlineStreamIsEndpoint(recognizer, stream)) {
      if (strlen(r->text)) {
        ++segment_id;
      }
      EdgevoxOnnxOnlineStreamReset(recognizer, stream);
    }

    EdgevoxOnnxDestroyOnlineRecognizerResult(r);
  }

  // add some tail padding
  float tail_paddings[4800] = {0};  // 0.3 seconds at 16 kHz sample rate
  EdgevoxOnnxOnlineStreamAcceptWaveform(stream, wave->sample_rate, tail_paddings,
                                       4800);

  EdgevoxOnnxFreeWave(wave);

  EdgevoxOnnxOnlineStreamInputFinished(stream);
  while (EdgevoxOnnxIsOnlineStreamReady(recognizer, stream)) {
    EdgevoxOnnxDecodeOnlineStream(recognizer, stream);
  }

  const EdgevoxOnnxOnlineRecognizerResult *r =
      EdgevoxOnnxGetOnlineStreamResult(recognizer, stream);

  if (strlen(r->text)) {
    EdgevoxOnnxPrint(display, segment_id, r->text);
  }

  EdgevoxOnnxDestroyOnlineRecognizerResult(r);

  EdgevoxOnnxDestroyDisplay(display);
  EdgevoxOnnxDestroyOnlineStream(stream);
  EdgevoxOnnxDestroyOnlineRecognizer(recognizer);
  fprintf(stderr, "\n");

  return 0;
}
