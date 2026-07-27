// edgevox-onnx/csrc/online-speech-denoiser-impl.h
//
// Copyright (c)  2026  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_ONLINE_SPEECH_DENOISER_IMPL_H_
#define EDGEVOX_ONNX_CSRC_ONLINE_SPEECH_DENOISER_IMPL_H_

#include <memory>

#include "edgevox-onnx/csrc/online-speech-denoiser.h"

namespace edgevox_onnx {

class OnlineSpeechDenoiserImpl {
 public:
  virtual ~OnlineSpeechDenoiserImpl() = default;

  static std::unique_ptr<OnlineSpeechDenoiserImpl> Create(
      const OnlineSpeechDenoiserConfig &config);

  template <typename Manager>
  static std::unique_ptr<OnlineSpeechDenoiserImpl> Create(
      Manager *mgr, const OnlineSpeechDenoiserConfig &config);

  virtual DenoisedAudio Run(const float *samples, int32_t n,
                            int32_t sample_rate) = 0;
  virtual DenoisedAudio Flush() = 0;
  virtual void Reset() = 0;
  virtual int32_t GetSampleRate() const = 0;
  virtual int32_t GetFrameShiftInSamples() const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_ONLINE_SPEECH_DENOISER_IMPL_H_
