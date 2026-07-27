// edgevox-onnx/csrc/speaker-embedding-extractor-nemo-model-meta-data.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_META_DATA_H_
#define EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_META_DATA_H_

#include <cstdint>
#include <string>

namespace edgevox_onnx {

struct SpeakerEmbeddingExtractorNeMoModelMetaData {
  int32_t output_dim = 0;
  int32_t feat_dim = 80;
  int32_t sample_rate = 0;
  int32_t window_size_ms = 25;
  int32_t window_stride_ms = 25;

  // Chinese, English, etc.
  std::string language;

  // for 3d-speaker, it is global-mean
  std::string feature_normalize_type;
  std::string window_type = "hann";
};

}  // namespace edgevox_onnx
#endif  // EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_META_DATA_H_
