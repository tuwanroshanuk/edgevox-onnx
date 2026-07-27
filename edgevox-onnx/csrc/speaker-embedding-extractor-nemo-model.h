// edgevox-onnx/csrc/speaker-embedding-extractor-nemo-model.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_H_
#define EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_H_

#include <memory>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/speaker-embedding-extractor-nemo-model-meta-data.h"
#include "edgevox-onnx/csrc/speaker-embedding-extractor.h"

namespace edgevox_onnx {

class SpeakerEmbeddingExtractorNeMoModel {
 public:
  explicit SpeakerEmbeddingExtractorNeMoModel(
      const SpeakerEmbeddingExtractorConfig &config);

  template <typename Manager>
  SpeakerEmbeddingExtractorNeMoModel(
      Manager *mgr, const SpeakerEmbeddingExtractorConfig &config);

  ~SpeakerEmbeddingExtractorNeMoModel();

  const SpeakerEmbeddingExtractorNeMoModelMetaData &GetMetaData() const;

  /**
   * @param x A float32 tensor of shape (N, C, T)
   * @param x_len A int64 tensor of shape (N,)
   * @return A float32 tensor of shape (N, C)
   */
  Ort::Value Compute(Ort::Value x, Ort::Value x_len) const;

  OrtAllocator *Allocator() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_NEMO_MODEL_H_
