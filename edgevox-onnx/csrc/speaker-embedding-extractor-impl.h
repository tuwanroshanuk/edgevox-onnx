// edgevox-onnx/csrc/speaker-embedding-extractor-impl.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_IMPL_H_
#define EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/speaker-embedding-extractor.h"

namespace edgevox_onnx {

class SpeakerEmbeddingExtractorImpl {
 public:
  virtual ~SpeakerEmbeddingExtractorImpl() = default;

  static std::unique_ptr<SpeakerEmbeddingExtractorImpl> Create(
      const SpeakerEmbeddingExtractorConfig &config);

  template <typename Manager>
  static std::unique_ptr<SpeakerEmbeddingExtractorImpl> Create(
      Manager *mgr, const SpeakerEmbeddingExtractorConfig &config);

  virtual int32_t Dim() const = 0;

  virtual std::unique_ptr<OnlineStream> CreateStream() const = 0;

  virtual bool IsReady(OnlineStream *s) const = 0;

  virtual std::vector<float> Compute(OnlineStream *s) const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_SPEAKER_EMBEDDING_EXTRACTOR_IMPL_H_
