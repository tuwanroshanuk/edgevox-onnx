// edgevox-onnx/csrc/base64-decode.h
//
// Copyright (c)  2022-2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_BASE64_DECODE_H_
#define EDGEVOX_ONNX_CSRC_BASE64_DECODE_H_

#include <string>

namespace edgevox_onnx {

/** @param s A base64 encoded string.
 *  @return Return the decoded string.
 */
std::string Base64Decode(const std::string &s);

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_BASE64_DECODE_H_
