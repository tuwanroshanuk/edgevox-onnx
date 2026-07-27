// edgevox-onnx/csrc/offline-speaker-segmentation-pyannote-model.h
//
// Copyright (c)  2024  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_SEGMENTATION_PYANNOTE_MODEL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_SEGMENTATION_PYANNOTE_MODEL_H_

#include <memory>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/offline-speaker-segmentation-model-config.h"
#include "edgevox-onnx/csrc/offline-speaker-segmentation-pyannote-model-meta-data.h"

namespace edgevox_onnx {

class OfflineSpeakerSegmentationPyannoteModel {
 public:
  explicit OfflineSpeakerSegmentationPyannoteModel(
      const OfflineSpeakerSegmentationModelConfig &config);

  template <typename Manager>
  OfflineSpeakerSegmentationPyannoteModel(
      Manager *mgr, const OfflineSpeakerSegmentationModelConfig &config);

  ~OfflineSpeakerSegmentationPyannoteModel();

  const OfflineSpeakerSegmentationPyannoteModelMetaData &GetModelMetaData()
      const;

  /**
   * @param x A 3-D float tensor of shape (batch_size, 1, num_samples)
   * @return Return a float tensor of
   *         shape (batch_size, num_frames, num_speakers). Note that
   *         num_speakers here uses powerset encoding.
   */
  Ort::Value Forward(Ort::Value x) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_SEGMENTATION_PYANNOTE_MODEL_H_
