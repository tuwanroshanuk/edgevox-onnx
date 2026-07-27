// edgevox-onnx/csrc/macros.h
//
// Copyright      2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_MACROS_H_
#define EDGEVOX_ONNX_CSRC_MACROS_H_
#include <stdio.h>
#include <stdlib.h>

#include <utility>
#if EDGEVOX_ONNX_ENABLE_WASM
#include <emscripten.h>
#endif
#if __OHOS__
#include "hilog/log.h"

#undef LOG_DOMAIN
#undef LOG_TAG

// https://gitee.com/openharmony/docs/blob/145a084f0b742e4325915e32f8184817927d1251/en/contribute/OpenHarmony-Log-guide.md#hilog-api-usage-specifications
#define LOG_DOMAIN 0x6666
#define LOG_TAG "edgevox_onnx"
#endif

#if __ANDROID_API__ >= 8
#include "android/log.h"
#define EDGEVOX_ONNX_LOGE(...)                                                  \
  do {                                                                         \
    fprintf(stderr, "%s:%s:%d ", __FILE__, __func__,                           \
            static_cast<int32_t>(__LINE__));                                   \
    fprintf(stderr, ##__VA_ARGS__);                                            \
    fprintf(stderr, "\n");                                                     \
    __android_log_print(ANDROID_LOG_WARN, "edgevox-onnx", "%s:%s:%d", __FILE__, \
                        __func__, static_cast<int32_t>(__LINE__));             \
    __android_log_print(ANDROID_LOG_WARN, "edgevox-onnx", ##__VA_ARGS__);       \
  } while (0)
#elif defined(__OHOS__)
#define EDGEVOX_ONNX_LOGE(...) OH_LOG_INFO(LOG_APP, ##__VA_ARGS__)
#elif EDGEVOX_ONNX_ENABLE_WASM
#define EDGEVOX_ONNX_LOGE(...)                                          \
  do {                                                                 \
    char _buf[4096];                                                   \
    snprintf(_buf, sizeof(_buf), ##__VA_ARGS__);                       \
    emscripten_log(EM_LOG_CONSOLE | EM_LOG_NO_PATHS | EM_LOG_ERROR,   \
                   "%s:%s:%d %s",                                      \
                   __FILE__, __func__, static_cast<int>(__LINE__),     \
                   _buf);                                              \
  } while (0)
#else
#define EDGEVOX_ONNX_LOGE(...)                        \
  do {                                               \
    fprintf(stderr, "%s:%s:%d ", __FILE__, __func__, \
            static_cast<int>(__LINE__));             \
    fprintf(stderr, ##__VA_ARGS__);                  \
    fprintf(stderr, "\n");                           \
  } while (0)
#endif

#define EDGEVOX_ONNX_EXIT(code) \
  do {                         \
    fflush(stdout);            \
    fflush(stderr);            \
    _Exit(code);               \
  } while (0)

// Read an integer
#define EDGEVOX_ONNX_READ_META_DATA(dst, src_key)                           \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      EDGEVOX_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
                                                                           \
    dst = atoi(value.c_str());                                             \
    if (dst < 0) {                                                         \
      EDGEVOX_ONNX_LOGE("Invalid value %d for '%s'", dst, src_key);         \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

#define EDGEVOX_ONNX_READ_META_DATA_WITH_DEFAULT(dst, src_key, default_value) \
  do {                                                                       \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);   \
    if (value.empty()) {                                                     \
      dst = default_value;                                                   \
    } else {                                                                 \
      dst = atoi(value.c_str());                                             \
      if (dst < 0) {                                                         \
        EDGEVOX_ONNX_LOGE("Invalid value %d for '%s'", dst, src_key);         \
        EDGEVOX_ONNX_EXIT(-1);                                                \
      }                                                                      \
    }                                                                        \
  } while (0)

// read a vector of integers
#define EDGEVOX_ONNX_READ_META_DATA_VEC(dst, src_key)                           \
  do {                                                                         \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);     \
    if (value.empty()) {                                                       \
      EDGEVOX_ONNX_LOGE("'%s' does not exist in the metadata", src_key);        \
      EDGEVOX_ONNX_EXIT(-1);                                                    \
    }                                                                          \
                                                                               \
    bool ret = SplitStringToIntegers(value.c_str(), ",", true, &dst);          \
    if (!ret) {                                                                \
      EDGEVOX_ONNX_LOGE("Invalid value '%s' for '%s'", value.c_str(), src_key); \
      EDGEVOX_ONNX_EXIT(-1);                                                    \
    }                                                                          \
  } while (0)

// read a vector of floats
#define EDGEVOX_ONNX_READ_META_DATA_VEC_FLOAT(dst, src_key)                     \
  do {                                                                         \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);     \
    if (value.empty()) {                                                       \
      EDGEVOX_ONNX_LOGE("%s does not exist in the metadata", src_key);          \
      EDGEVOX_ONNX_EXIT(-1);                                                    \
    }                                                                          \
                                                                               \
    bool ret = SplitStringToFloats(value.c_str(), ",", true, &dst);            \
    if (!ret) {                                                                \
      EDGEVOX_ONNX_LOGE("Invalid value '%s' for '%s'", value.c_str(), src_key); \
      EDGEVOX_ONNX_EXIT(-1);                                                    \
    }                                                                          \
  } while (0)

// read a vector of strings
#define EDGEVOX_ONNX_READ_META_DATA_VEC_STRING(dst, src_key)                \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      EDGEVOX_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
    SplitStringToVector(value.c_str(), ",", false, &dst);                  \
                                                                           \
    if (dst.empty()) {                                                     \
      EDGEVOX_ONNX_LOGE("Invalid value '%s' for '%s'. Empty vector!",       \
                       value.c_str(), src_key);                            \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

// read a vector of strings separated by sep
#define EDGEVOX_ONNX_READ_META_DATA_VEC_STRING_SEP(dst, src_key, sep)       \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      EDGEVOX_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
    SplitStringToVector(value.c_str(), sep, false, &dst);                  \
                                                                           \
    if (dst.empty()) {                                                     \
      EDGEVOX_ONNX_LOGE("Invalid value '%s' for '%s'. Empty vector!",       \
                       value.c_str(), src_key);                            \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

// Read a string
#define EDGEVOX_ONNX_READ_META_DATA_STR(dst, src_key)                       \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      EDGEVOX_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
                                                                           \
    dst = std::move(value);                                                \
    if (dst.empty()) {                                                     \
      EDGEVOX_ONNX_LOGE("Invalid value for '%s'\n", src_key);               \
      EDGEVOX_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

#define EDGEVOX_ONNX_READ_META_DATA_STR_ALLOW_EMPTY(dst, src_key)           \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
                                                                           \
    dst = std::move(value);                                                \
  } while (0)

#define EDGEVOX_ONNX_READ_META_DATA_STR_WITH_DEFAULT(dst, src_key,          \
                                                    default_value)         \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      dst = default_value;                                                 \
    } else {                                                               \
      dst = std::move(value);                                              \
      if (dst.empty()) {                                                   \
        EDGEVOX_ONNX_LOGE("Invalid value for '%s'\n", src_key);             \
        EDGEVOX_ONNX_EXIT(-1);                                              \
      }                                                                    \
    }                                                                      \
  } while (0)

#endif  // EDGEVOX_ONNX_CSRC_MACROS_H_
