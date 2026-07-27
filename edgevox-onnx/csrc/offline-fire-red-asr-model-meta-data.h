// edgevox-onnx/csrc/offline-fire-red-asr-model-meta-data.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_MODEL_META_DATA_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_MODEL_META_DATA_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace edgevox_onnx {

struct OfflineFireRedAsrModelMetaData {
  int32_t sos_id;
  int32_t eos_id;
  int32_t max_len;

  int32_t num_decoder_layers;
  int32_t num_head;
  int32_t head_dim;

  std::vector<float> mean;
  std::vector<float> inv_stddev;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_FIRE_RED_ASR_MODEL_META_DATA_H_
