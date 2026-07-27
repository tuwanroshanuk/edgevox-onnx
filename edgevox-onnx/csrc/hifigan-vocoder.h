// edgevox-onnx/csrc/hifigan-vocoder.h
//
// Copyright (c)  2024  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_HIFIGAN_VOCODER_H_
#define EDGEVOX_ONNX_CSRC_HIFIGAN_VOCODER_H_

#include <memory>
#include <string>
#include <vector>

#include "onnxruntime_cxx_api.h"  // NOLINT
#include "edgevox-onnx/csrc/vocoder.h"

namespace edgevox_onnx {

class HifiganVocoder : public Vocoder {
 public:
  ~HifiganVocoder() override;

  HifiganVocoder(int32_t num_threads, const std::string &provider,
                 const std::string &model);

  template <typename Manager>
  HifiganVocoder(Manager *mgr, int32_t num_threads, const std::string &provider,
                 const std::string &model);

  /** @param mel A float32 tensor of shape (batch_size, feat_dim, num_frames).
   *  @return Return a float32 tensor of shape (batch_size, num_samples).
   */
  std::vector<float> Run(Ort::Value mel) const override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_HIFIGAN_VOCODER_H_
