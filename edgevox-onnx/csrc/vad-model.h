// edgevox-onnx/csrc/vad-model.h
//
// Copyright (c)  2023  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_VAD_MODEL_H_
#define EDGEVOX_ONNX_CSRC_VAD_MODEL_H_

#include <memory>

#include "edgevox-onnx/csrc/vad-model-config.h"

namespace edgevox_onnx {

class VadModel {
 public:
  virtual ~VadModel() = default;

  static std::unique_ptr<VadModel> Create(const VadModelConfig &config);

  template <typename Manager>
  static std::unique_ptr<VadModel> Create(Manager *mgr,
                                          const VadModelConfig &config);

  // reset the internal model states
  virtual void Reset() = 0;

  /**
   * @param samples Pointer to a 1-d array containing audio samples.
   *                Each sample should be normalized to the range [-1, 1].
   * @param n Number of samples. Should be equal to WindowSize()
   *
   * @return Return true if speech is detected. Return false otherwise.
   */
  virtual bool IsSpeech(const float *samples, int32_t n) = 0;

  virtual float Compute(const float *samples, int32_t n) = 0;

  virtual int32_t WindowSize() const = 0;

  virtual int32_t WindowShift() const = 0;

  virtual int32_t MinSilenceDurationSamples() const = 0;
  virtual int32_t MinSpeechDurationSamples() const = 0;
  virtual void SetMinSilenceDuration(float s) = 0;
  virtual void SetThreshold(float threshold) = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_VAD_MODEL_H_
