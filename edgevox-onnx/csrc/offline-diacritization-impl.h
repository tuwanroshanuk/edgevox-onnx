// edgevox-onnx/csrc/offline-diacritization-impl.h
//
// Copyright (c)  2026  Matias Lin
#ifndef EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_IMPL_H_
#define EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/offline-diacritization.h"

namespace edgevox_onnx {

class OfflineDiacritizationImpl {
 public:
  virtual ~OfflineDiacritizationImpl() = default;

  static std::unique_ptr<OfflineDiacritizationImpl> Create(
      const OfflineDiacritizationConfig &config);

  template <typename Manager>
  static std::unique_ptr<OfflineDiacritizationImpl> Create(
      Manager *mgr, const OfflineDiacritizationConfig &config);

  virtual std::string AddDiacritics(const std::string &text) const = 0;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_OFFLINE_DIACRITIZATION_IMPL_H_
