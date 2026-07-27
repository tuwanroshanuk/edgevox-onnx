// edgevox-onnx/csrc/offline-tts-supertonic-model-config.cc
//
// Copyright (c)  2026 zengyw

#include "edgevox-onnx/csrc/offline-tts-supertonic-model-config.h"

#include <sstream>
#include <string>

#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

void OfflineTtsSupertonicModelConfig::Register(ParseOptions *po) {
  po->Register("supertonic-duration-predictor", &duration_predictor,
               "Path to duration_predictor.onnx for Supertonic TTS");
  po->Register("supertonic-text-encoder", &text_encoder,
               "Path to text_encoder.onnx for Supertonic TTS");
  po->Register("supertonic-vector-estimator", &vector_estimator,
               "Path to vector_estimator.onnx for Supertonic TTS");
  po->Register("supertonic-vocoder", &vocoder,
               "Path to vocoder.onnx for Supertonic TTS");
  po->Register("supertonic-tts-json", &tts_json,
               "Path to tts.json for Supertonic TTS");
  po->Register("supertonic-unicode-indexer", &unicode_indexer,
               "Path to unicode_indexer.bin for Supertonic TTS");
  po->Register("supertonic-voice-style", &voice_style,
               "Path to Supertonic voice.bin (use sid 0..NumSpeakers()-1 to "
               "select)");
}

bool OfflineTtsSupertonicModelConfig::Validate() const {
  if (duration_predictor.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-duration-predictor");
    return false;
  }
  if (!FileExists(duration_predictor)) {
    EDGEVOX_ONNX_LOGE("--supertonic-duration-predictor '%s' does not exist",
                     duration_predictor.c_str());
    return false;
  }

  if (text_encoder.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-text-encoder");
    return false;
  }
  if (!FileExists(text_encoder)) {
    EDGEVOX_ONNX_LOGE("--supertonic-text-encoder '%s' does not exist",
                     text_encoder.c_str());
    return false;
  }

  if (vector_estimator.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-vector-estimator");
    return false;
  }
  if (!FileExists(vector_estimator)) {
    EDGEVOX_ONNX_LOGE("--supertonic-vector-estimator '%s' does not exist",
                     vector_estimator.c_str());
    return false;
  }

  if (vocoder.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-vocoder");
    return false;
  }
  if (!FileExists(vocoder)) {
    EDGEVOX_ONNX_LOGE("--supertonic-vocoder '%s' does not exist",
                     vocoder.c_str());
    return false;
  }

  if (tts_json.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-tts-json");
    return false;
  }
  if (!FileExists(tts_json)) {
    EDGEVOX_ONNX_LOGE("--supertonic-tts-json '%s' does not exist",
                     tts_json.c_str());
    return false;
  }

  if (unicode_indexer.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-unicode-indexer");
    return false;
  }
  if (!FileExists(unicode_indexer)) {
    EDGEVOX_ONNX_LOGE("--supertonic-unicode-indexer '%s' does not exist",
                     unicode_indexer.c_str());
    return false;
  }

  if (voice_style.empty()) {
    EDGEVOX_ONNX_LOGE("Please provide --supertonic-voice-style");
    return false;
  }
  if (!FileExists(voice_style)) {
    EDGEVOX_ONNX_LOGE("--supertonic-voice-style '%s' does not exist",
                     voice_style.c_str());
    return false;
  }
  return true;
}

std::string OfflineTtsSupertonicModelConfig::ToString() const {
  std::ostringstream os;
  os << "OfflineTtsSupertonicModelConfig(";
  os << "duration_predictor=\"" << duration_predictor << "\", ";
  os << "text_encoder=\"" << text_encoder << "\", ";
  os << "vector_estimator=\"" << vector_estimator << "\", ";
  os << "vocoder=\"" << vocoder << "\", ";
  os << "tts_json=\"" << tts_json << "\", ";
  os << "unicode_indexer=\"" << unicode_indexer << "\", ";
  os << "voice_style=\"" << voice_style << "\")";
  return os.str();
}

}  // namespace edgevox_onnx
