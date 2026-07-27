// edgevox-onnx/csrc/microphone.h
//
// Copyright (c)  2022-2023  Xiaomi Corporation

#ifndef EDGEVOX_ONNX_CSRC_MICROPHONE_H_
#define EDGEVOX_ONNX_CSRC_MICROPHONE_H_
#include <cstdint>

#include "portaudio.h"  // NOLINT
namespace edgevox_onnx {

class Microphone {
 public:
  Microphone();
  ~Microphone();

  int32_t GetDeviceCount() const;
  int32_t GetDefaultInputDevice() const;
  void PrintDevices(int32_t sel) const;

  bool OpenDevice(int32_t index, int32_t sample_rate, int32_t channel,
                  PaStreamCallback cb, void *userdata);

  void CloseDevice();

 private:
  PaStream *stream = nullptr;
};

}  // namespace edgevox_onnx

#endif  // EDGEVOX_ONNX_CSRC_MICROPHONE_H_
