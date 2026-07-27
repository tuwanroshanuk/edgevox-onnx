// edgevox-onnx/csrc/fst-utils.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include "edgevox-onnx/csrc/fst-utils.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "fst/extensions/far/far.h"
#include "kaldifst/csrc/kaldi-fst-io.h"
#include "edgevox-onnx/csrc/file-utils.h"
#include "edgevox-onnx/csrc/macros.h"

namespace edgevox_onnx {

// This function is copied from kaldi.
//
// @param filename Path to a StdVectorFst or StdConstFst graph
// @return The caller should free the returned pointer using `delete` to
//         avoid memory leak.
fst::Fst<fst::StdArc> *ReadGraph(const std::string &filename) {
  // read decoding network FST
  auto is = OpenInputFile(filename, std::ios::binary);
  if (!is.good()) {
    EDGEVOX_ONNX_LOGE("Could not open decoding-graph FST %s", filename.c_str());
  }

  fst::FstHeader hdr;
  if (!hdr.Read(is, "<unknown>")) {
    EDGEVOX_ONNX_LOGE("Reading FST: error reading FST header.");
  }

  if (hdr.ArcType() != fst::StdArc::Type()) {
    EDGEVOX_ONNX_LOGE("FST with arc type %s not supported",
                     hdr.ArcType().c_str());
  }
  fst::FstReadOptions ropts("<unspecified>", &hdr);

  fst::Fst<fst::StdArc> *decode_fst = nullptr;

  if (hdr.FstType() == "vector") {
    decode_fst = fst::VectorFst<fst::StdArc>::Read(is, ropts);
  } else if (hdr.FstType() == "const") {
    decode_fst = fst::ConstFst<fst::StdArc>::Read(is, ropts);
  } else {
    EDGEVOX_ONNX_LOGE("Reading FST: unsupported FST type: %s",
                     hdr.FstType().c_str());
  }

  if (decode_fst == nullptr) {  // fst code will warn.
    EDGEVOX_ONNX_LOGE("Error reading FST (after reading header).");
    return nullptr;
  } else {
    return decode_fst;
  }
}

std::vector<std::unique_ptr<fst::StdConstFst>> ReadFstsFromFar(
    const std::vector<char> &buffer) {
  std::vector<std::unique_ptr<fst::StdConstFst>> ans;

  auto stream = std::make_unique<std::istringstream>(
      std::string(buffer.data(), buffer.size()), std::ios::binary);

  std::unique_ptr<fst::FarReader<fst::StdArc>> reader(
      fst::FarReader<fst::StdArc>::Open(std::move(stream)));
  if (!reader) {
    EDGEVOX_ONNX_LOGE("Failed to open FAR data");
    EDGEVOX_ONNX_EXIT(-1);
  }

  for (; !reader->Done(); reader->Next()) {
    ans.emplace_back(fst::CastOrConvertToConstFst(reader->GetFst()->Copy()));
  }

  return ans;
}

}  // namespace edgevox_onnx
