// edgevox-onnx/python/csrc/edgevox-onnx.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/edgevox-onnx.h"

#include "edgevox-onnx/python/csrc/alsa.h"
#include "edgevox-onnx/python/csrc/audio-tagging.h"
#include "edgevox-onnx/python/csrc/circular-buffer.h"
#include "edgevox-onnx/python/csrc/display.h"
#include "edgevox-onnx/python/csrc/endpoint.h"
#include "edgevox-onnx/python/csrc/features.h"
#include "edgevox-onnx/python/csrc/homophone-replacer.h"
#include "edgevox-onnx/python/csrc/keyword-spotter.h"
#include "edgevox-onnx/python/csrc/offline-ctc-fst-decoder-config.h"
#include "edgevox-onnx/python/csrc/offline-diacritization.h"
#include "edgevox-onnx/python/csrc/offline-lm-config.h"
#include "edgevox-onnx/python/csrc/offline-model-config.h"
#include "edgevox-onnx/python/csrc/offline-punctuation.h"
#include "edgevox-onnx/python/csrc/offline-recognizer.h"
#include "edgevox-onnx/python/csrc/offline-source-separation.h"
#include "edgevox-onnx/python/csrc/offline-speech-denoiser.h"
#include "edgevox-onnx/python/csrc/offline-stream.h"
#include "edgevox-onnx/python/csrc/online-ctc-fst-decoder-config.h"
#include "edgevox-onnx/python/csrc/online-lm-config.h"
#include "edgevox-onnx/python/csrc/online-model-config.h"
#include "edgevox-onnx/python/csrc/online-punctuation.h"
#include "edgevox-onnx/python/csrc/online-recognizer.h"
#include "edgevox-onnx/python/csrc/online-speech-denoiser.h"
#include "edgevox-onnx/python/csrc/online-stream.h"
#include "edgevox-onnx/python/csrc/speaker-embedding-extractor.h"
#include "edgevox-onnx/python/csrc/speaker-embedding-manager.h"
#include "edgevox-onnx/python/csrc/spoken-language-identification.h"
#include "edgevox-onnx/python/csrc/vad-model-config.h"
#include "edgevox-onnx/python/csrc/vad-model.h"
#include "edgevox-onnx/python/csrc/version.h"
#include "edgevox-onnx/python/csrc/voice-activity-detector.h"
#include "edgevox-onnx/python/csrc/wave-writer.h"

#if EDGEVOX_ONNX_ENABLE_TTS == 1
#include "edgevox-onnx/python/csrc/offline-tts.h"
#include "edgevox-onnx/python/csrc/sentence-piece-tokenizer.h"
#endif

#if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1
#include "edgevox-onnx/python/csrc/fast-clustering.h"
#include "edgevox-onnx/python/csrc/offline-speaker-diarization-result.h"
#include "edgevox-onnx/python/csrc/offline-speaker-diarization.h"
#endif

namespace edgevox_onnx {

PYBIND11_MODULE(_edgevox_onnx, m) {
  m.doc() = "pybind11 binding of edgevox-onnx";

  PybindWaveWriter(&m);
  PybindAudioTagging(&m);
  PybindOfflineDiacritization(&m);
  PybindOfflinePunctuation(&m);
  PybindOnlinePunctuation(&m);
  PybindHomophoneReplacer(&m);

  PybindFeatures(&m);
  PybindOnlineCtcFstDecoderConfig(&m);
  PybindOnlineModelConfig(&m);
  PybindOnlineLMConfig(&m);
  PybindOnlineStream(&m);
  PybindEndpoint(&m);
  PybindOnlineRecognizer(&m);
  PybindKeywordSpotter(&m);
  PybindDisplay(&m);

  PybindOfflineStream(&m);
  PybindOfflineLMConfig(&m);
  PybindOfflineModelConfig(&m);
  PybindOfflineCtcFstDecoderConfig(&m);
  PybindOfflineRecognizer(&m);

  PybindVadModelConfig(&m);
  PybindVadModel(&m);
  PybindCircularBuffer(&m);
  PybindVoiceActivityDetector(&m);

#if EDGEVOX_ONNX_ENABLE_TTS == 1
  PybindOfflineTts(&m);
  PybindSentencePieceTokenizer(&m);
#else
  /* Define "empty" TTS symbols */
  m.attr("OfflineTtsKittenModelConfig") = py::none();
  m.attr("OfflineTtsPocketModelConfig") = py::none();
  m.attr("OfflineTtsKokoroModelConfig") = py::none();
  m.attr("OfflineTtsMatchaModelConfig") = py::none();
  m.attr("OfflineTtsModelConfig") = py::none();
  m.attr("OfflineTtsVitsModelConfig") = py::none();
  m.attr("OfflineTtsZipvoiceModelConfig") = py::none();
  m.attr("GeneratedAudio") = py::none();
  m.attr("OfflineTtsConfig") = py::none();
  m.attr("OfflineTts") = py::none();
  m.attr("SentencePieceTokenizer") = py::none();
#endif

  PybindSpeakerEmbeddingExtractor(&m);
  PybindSpeakerEmbeddingManager(&m);
  PybindSpokenLanguageIdentification(&m);

#if EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION == 1
  PybindFastClustering(&m);
  PybindOfflineSpeakerDiarizationResult(&m);
  PybindOfflineSpeakerDiarization(&m);
#else
  /* Define "empty" diarization symbols */
  m.attr("FastClusteringConfig") = py::none();
  m.attr("FastClustering") = py::none();
  m.attr("OfflineSpeakerDiarizationSegment") = py::none();
  m.attr("OfflineSpeakerDiarizationResult") = py::none();
  m.attr("OfflineSpeakerSegmentationPyannoteModelConfig") = py::none();
  m.attr("OfflineSpeakerSegmentationModelConfig") = py::none();
  m.attr("OfflineSpeakerDiarizationConfig") = py::none();
  m.attr("OfflineSpeakerDiarization") = py::none();
#endif

  PybindAlsa(&m);
  PybindOfflineSpeechDenoiser(&m);
  PybindOnlineSpeechDenoiser(&m);
  PybindOfflineSourceSeparation(&m);
  PybindVersion(&m);
}

}  // namespace edgevox_onnx
