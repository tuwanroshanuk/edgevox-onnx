// edgevox-onnx/csrc/offline-speaker-diarization-impl.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_DIARIZATION_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_DIARIZATION_IMPL_H_

#include <functional>
#include <memory>

#include "edgevox-onnx/csrc/offline-speaker-diarization.h"
namespace edgevox_onnx {

class OfflineSpeakerDiarizationImpl {
 public:
  static std::unique_ptr<OfflineSpeakerDiarizationImpl> Create(
      const OfflineSpeakerDiarizationConfig &config);

  template <typename Manager>
  static std::unique_ptr<OfflineSpeakerDiarizationImpl> Create(
      Manager *mgr, const OfflineSpeakerDiarizationConfig &config);

  virtual ~OfflineSpeakerDiarizationImpl() = default;

  virtual int32_t SampleRate() const = 0;

  // Note: Only config.clustering is used. All other fields in config are
  // ignored
  virtual void SetConfig(const OfflineSpeakerDiarizationConfig &config) = 0;

  virtual OfflineSpeakerDiarizationResult Process(
      const float *audio, int32_t n,
      OfflineSpeakerDiarizationProgressCallback callback = nullptr,
      void *callback_arg = nullptr) const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_SPEAKER_DIARIZATION_IMPL_H_
