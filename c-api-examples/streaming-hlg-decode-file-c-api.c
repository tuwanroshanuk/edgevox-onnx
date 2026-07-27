// c-api-examples/streaming-hlg-decode-file-c-api.c
//
// Copyright (c)  2024  Xiaomi Corporation
/*
We use the following model as an example

// clang-format off

Download the model from
https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2

tar xvf sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
rm sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2

build/bin/streaming-hlg-decode-file-c-api

(The above model is from https://github.com/k2-fsa/icefall/pull/1557)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  // clang-format off
  //
  // Please download the model from
  // https://github.com/k2-fsa/sherpa-onnx/releases/download/asr-models/sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
  const char *model = "./sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18/ctc-epoch-30-avg-3-chunk-16-left-128.int8.onnx";
  const char *tokens = "./sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18/tokens.txt";
  const char *graph = "./sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18/HLG.fst";
  const char *wav_filename = "./sherpa-onnx-streaming-zipformer-ctc-small-2024-03-18/test_wavs/8k.wav";
  // clang-format on

  EdgevoxOnnxOnlineRecognizerConfig config;

  memset(&config, 0, sizeof(config));
  config.feat_config.sample_rate = 16000;
  config.feat_config.feature_dim = 80;
  config.model_config.zipformer2_ctc.model = model;
  config.model_config.tokens = tokens;
  config.model_config.num_threads = 1;
  config.model_config.provider = "cpu";
  config.model_config.debug = 0;
  config.ctc_fst_decoder_config.graph = graph;
  const EdgevoxOnnxOnlineRecognizer *recognizer =
      EdgevoxOnnxCreateOnlineRecognizer(&config);
  if (!recognizer) {
    fprintf(stderr, "Failed to create recognizer");
    exit(-1);
  }

  const EdgevoxOnnxOnlineStream *stream =
      EdgevoxOnnxCreateOnlineStream(recognizer);

  const EdgevoxOnnxDisplay *display = EdgevoxOnnxCreateDisplay(50);
  int32_t segment_id = 0;

  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read %s\n", wav_filename);
    exit(-1);
  }

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
