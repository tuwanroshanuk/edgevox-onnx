// edgevox-onnx/csrc/offline-dolphin-model-meta-data.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_META_DATA_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_META_DATA_H_

#include <string>
#include <vector>

namespace edgevox_onnx {

struct OfflineDolphinModelMetaData {
  int32_t vocab_size;
  int32_t subsampling_factor = 4;
  std::vector<float> mean;
  std::vector<float> inv_stddev;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_DOLPHIN_MODEL_META_DATA_H_
