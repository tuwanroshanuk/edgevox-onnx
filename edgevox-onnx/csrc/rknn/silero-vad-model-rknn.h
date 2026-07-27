// edgevox-onnx/csrc/rknn/silero-vad-model-rknn.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_RKNN_SILERO_VAD_MODEL_RKNN_H_
#define EDGEVOX_ONNX_CSRC_RKNN_SILERO_VAD_MODEL_RKNN_H_

#include <memory>

#include "rknn_api.h"  // NOLINT
#include "edgevox-onnx/csrc/online-model-config.h"
#include "edgevox-onnx/csrc/vad-model.h"

namespace edgevox_onnx {

class SileroVadModelRknn : public VadModel {
 public:
  explicit SileroVadModelRknn(const VadModelConfig &config);

  template <typename Manager>
  SileroVadModelRknn(Manager *mgr, const VadModelConfig &config);

  ~SileroVadModelRknn() override;

  // reset the internal model states
  void Reset() override;

  /**
   * @param samples Pointer to a 1-d array containing audio samples.
   *                Each sample should be normalized to the range [-1, 1].
   * @param n Number of samples.
   *
   * @return Return true if speech is detected. Return false otherwise.
   */
  bool IsSpeech(const float *samples, int32_t n) override;
  float Compute(const float *samples, int32_t n) override;

  // For silero vad V4, it is WindowShift().
  int32_t WindowSize() const override;

  // 512
  int32_t WindowShift() const override;

  int32_t MinSilenceDurationSamples() const override;
  int32_t MinSpeechDurationSamples() const override;

  void SetMinSilenceDuration(float s) override;
  void SetThreshold(float threshold) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_RKNN_SILERO_VAD_MODEL_RKNN_H_
