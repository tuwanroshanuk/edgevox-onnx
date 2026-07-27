// c-api-examples/streaming-zipformer-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation

//
// This file demonstrates how to use streaming Zipformer with edgevox-onnx's C
// API.
// clang-format off
// 
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-en-20M-2023-02-17.tar.bz2
// tar xvf edgevox-onnx-streaming-zipformer-en-20M-2023-02-17.tar.bz2
// rm edgevox-onnx-streaming-zipformer-en-20M-2023-02-17.tar.bz2
//
// clang-format on

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "edgevox-onnx-streaming-zipformer-en-20M-2023-02-17/test_wavs/0.wav";
  const char *encoder_filename =
      "edgevox-onnx-streaming-zipformer-en-20M-2023-02-17/"
      "encoder-epoch-99-avg-1.onnx";
  const char *decoder_filename =
      "edgevox-onnx-streaming-zipformer-en-20M-2023-02-17/"
      "decoder-epoch-99-avg-1.onnx";
  const char *joiner_filename =
      "edgevox-onnx-streaming-zipformer-en-20M-2023-02-17/"
      "joiner-epoch-99-avg-1.onnx";
  const char *tokens_filename =
      "edgevox-onnx-streaming-zipformer-en-20M-2023-02-17/tokens.txt";
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
