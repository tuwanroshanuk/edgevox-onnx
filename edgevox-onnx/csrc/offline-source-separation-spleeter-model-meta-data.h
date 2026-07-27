// edgevox-onnx/csrc/offline-source-separation-spleeter-model-meta-data.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_META_DATA_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_META_DATA_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace edgevox_onnx {

// See also
// https://github.com/k2-fsa/sherpa-onnx/blob/master/scripts/spleeter/separate_onnx.py
struct OfflineSourceSeparationSpleeterModelMetaData {
  int32_t sample_rate = 44100;
  int32_t num_stems = 2;

  int32_t n_fft = 4096;
  int32_t hop_length = 1024;
  int32_t window_length = 4096;
  bool center = false;
  std::string window_type = "hann";
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SOURCE_SEPARATION_SPLEETER_MODEL_META_DATA_H_
