// edgevox-onnx/python/csrc/version.cc
//
// Copyright (c)  2025  Xiaomi Corporation

#include "edgevox-onnx/python/csrc/version.h"

#include <string>

#include "edgevox-onnx/csrc/version.h"

namespace edgevox_onnx {

void PybindVersion(py::module *m) {
  m->attr("version") = std::string(GetVersionStr());
  m->attr("git_sha1") = std::string(GetGitSha1());
  m->attr("git_date") = std::string(GetGitDate());

  m->attr("__doc_version") = "str: The version of edgevox-onnx.";
  m->attr("__doc_git_sha1") =
      "str: The git commit SHA1 used to build edgevox-onnx.";
  m->attr("__doc_git_date") =
      "str: The git commit date used to build edgevox-onnx.";
}

}  // namespace edgevox_onnx
