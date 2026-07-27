// edgevox-onnx/csrc/offline-tts-zipvoice-model-meta-data.h
//
// Copyright (c)  2025  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_MODEL_META_DATA_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_MODEL_META_DATA_H_

#include <cstdint>
#include <string>

namespace edgevox_onnx {

// If you are not sure what each field means, please
// have a look of the Python file in the model directory that
// you have downloaded.
struct OfflineTtsZipvoiceModelMetaData {
  int32_t version = 1;
  int32_t feat_dim = 100;
  int32_t sample_rate = 24000;
  int32_t n_fft = 1024;
  int32_t hop_length = 256;
  int32_t window_length = 1024;
  int32_t num_mels = 100;
  int32_t use_espeak = 1;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_TTS_ZIPVOICE_MODEL_META_DATA_H_
