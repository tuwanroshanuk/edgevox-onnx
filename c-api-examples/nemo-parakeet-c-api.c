// c-api-examples/nemo-parakeet-c-api.c
// Example using the C API and edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8 model
// Prints recognized text, per-token timestamps, and durations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edgevox-onnx/c-api/c-api.h"

int32_t main() {
  const char *wav_filename =
      "./edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8/test_wavs/en.wav";
  const char *encoder_filename =
      "edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8/encoder.int8.onnx";
  const char *decoder_filename =
      "edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8/decoder.int8.onnx";
  const char *joiner_filename =
      "edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8/joiner.int8.onnx";
  const char *tokens_filename =
      "edgevox-onnx-nemo-parakeet-tdt-0.6b-v3-int8/tokens.txt";
  const char *provider = "cpu";

  if (!EdgevoxOnnxFileExists(wav_filename)) {
    fprintf(stderr, "File not found: %s\n", wav_filename);
    return -1;
  }
  const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(wav_filename);
  if (wave == NULL) {
    fprintf(stderr, "Failed to read or parse %s (not a valid mono 16-bit WAVE file)\n", wav_filename);
    return -1;
  }

  EdgevoxOnnxOfflineModelConfig offline_model_config;
  memset(&offline_model_config, 0, sizeof(offline_model_config));
  offline_model_config.debug = 0;
  offline_model_config.num_threads = 1;
  offline_model_config.provider = provider;
  offline_model_config.tokens = tokens_filename;
  offline_model_config.transducer.encoder = encoder_filename;
  offline_model_config.transducer.decoder = decoder_filename;
  offline_model_config.transducer.joiner = joiner_filename;

  EdgevoxOnnxOfflineRecognizerConfig recognizer_config;
  memset(&recognizer_config, 0, sizeof(recognizer_config));
  recognizer_config.decoding_method = "greedy_search";
  recognizer_config.model_config = offline_model_config;

  const EdgevoxOnnxOfflineRecognizer *recognizer =
      EdgevoxOnnxCreateOfflineRecognizer(&recognizer_config);
  if (recognizer == NULL) {
    fprintf(stderr, "Please check your config!\n");
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  const EdgevoxOnnxOfflineStream *stream =
      EdgevoxOnnxCreateOfflineStream(recognizer);
  if (stream == NULL) {
    fprintf(stderr, "Failed to create offline stream.\n");
    EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
    EdgevoxOnnxFreeWave(wave);
    return -1;
  }

  EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
                                  wave->num_samples);
  EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
  const EdgevoxOnnxOfflineRecognizerResult *result =
      EdgevoxOnnxGetOfflineStreamResult(stream);

  printf("Recognized text: %s\n", result->text);

  if (result->tokens_arr && result->timestamps && result->durations) {
    printf("Token\tTimestamp\tDuration\n");
    for (int32_t i = 0; i < result->count; ++i) {
      printf("%s\t%.2f\t%.2f\n", result->tokens_arr[i], result->timestamps[i], result->durations[i]);
    }
  } else {
    printf("Timestamps or durations not available.\n");
  }

  EdgevoxOnnxDestroyOfflineRecognizerResult(result);
  EdgevoxOnnxDestroyOfflineStream(stream);
  EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
  EdgevoxOnnxFreeWave(wave);

  return 0;
}
