// cxx-api-examples/spoken-language-identification-cxx-api.cc
//
// Copyright (c)  2026  Xiaomi Corporation

//
// This file demonstrates how to use spoken language identification
// with edgevox-onnx's C++ API.
//
// clang-format off
//
// wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
// tar xvf edgevox-onnx-whisper-tiny.tar.bz2
// rm edgevox-onnx-whisper-tiny.tar.bz2
//
// clang-format on

#include <cstdio>
#include <iostream>
#include <string>

#include "edgevox-onnx/c-api/cxx-api.h"

int32_t main() {
  using namespace edgevox_onnx::cxx;  // NOLINT

  SpokenLanguageIdentificationConfig config;
  config.whisper.encoder = "./edgevox-onnx-whisper-tiny/tiny-encoder.int8.onnx";
  config.whisper.decoder = "./edgevox-onnx-whisper-tiny/tiny-decoder.int8.onnx";
  config.num_threads = 1;

  SpokenLanguageIdentification slid =
      SpokenLanguageIdentification::Create(config);
  if (!slid.Get()) {
    std::cerr << "Failed to create spoken language identifier\n";
    return -1;
  }

  std::string wav_filename = "./edgevox-onnx-whisper-tiny/test_wavs/0.wav";
  Wave wave = ReadWave(wav_filename);
  if (wave.samples.empty()) {
    std::cerr << "Failed to read: '" << wav_filename << "'\n";
    return -1;
  }

  OfflineStream stream = slid.CreateStream();
  stream.AcceptWaveform(wave.sample_rate, wave.samples.data(),
                        wave.samples.size());

  SpokenLanguageIdentificationResult result = slid.Compute(&stream);

  std::cout << "Detected language: " << result.lang << "\n";

  return 0;
}
