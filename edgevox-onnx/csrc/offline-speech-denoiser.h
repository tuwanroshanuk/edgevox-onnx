// edgevox-onnx/csrc/offline-speech-denoiser.h
//
// Copyright (c)  2025  Xiaomi Corporation
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-speech-denoiser-model-config.h"
#include "edgevox-onnx/csrc/parse-options.h"

namespace edgevox_onnx {

struct DenoisedAudio {
  std::vector<float> samples;
  int32_t sample_rate;
};

struct OfflineSpeechDenoiserConfig {
  OfflineSpeechDenoiserModelConfig model;

  void Register(ParseOptions *po);
  bool Validate() const;

  std::string ToString() const;
};

class OfflineSpeechDenoiserImpl;

class OfflineSpeechDenoiser {
 public:
  explicit OfflineSpeechDenoiser(const OfflineSpeechDenoiserConfig &config);
  ~OfflineSpeechDenoiser();

  template <typename Manager>
  OfflineSpeechDenoiser(Manager *mgr,
                        const OfflineSpeechDenoiserConfig &config);

  /*
   * @param samples 1-D array of audio samples. Each sample is in the
   *                range [-1, 1].
   * @param n Number of samples
   * @param sample_rate Sample rate of the input samples
   *
   */
  DenoisedAudio Run(const float *samples, int32_t n, int32_t sample_rate) const;

  /*
   * Return the sample rate of the denoised audio
   */
  int32_t GetSampleRate() const;

 private:
  std::unique_ptr<OfflineSpeechDenoiserImpl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SPEECH_DENOISER_H_
