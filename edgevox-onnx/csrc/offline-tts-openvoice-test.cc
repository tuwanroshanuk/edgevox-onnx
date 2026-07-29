// edgevox-onnx/csrc/offline-tts-openvoice-test.cc
//
// Copyright (c)  2026  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-tts-openvoice.h"

#include <cmath>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

namespace edgevox_onnx {

TEST(OpenVoice, LinearSpectrogramMatchesNumpyReference) {
  constexpr float kPi = 3.14159265358979323846f;
  std::vector<float> audio(2048);
  for (int32_t i = 0; i != static_cast<int32_t>(audio.size()); ++i) {
    audio[i] = std::sin(2 * kPi * 440 * i / 22050);
  }

  int64_t frames = 0;
  auto spec = ComputeOpenVoiceLinearSpectrogramForTest(audio, &frames);
  ASSERT_EQ(frames, 8);
  ASSERT_EQ(spec.size(), 513 * frames);
  EXPECT_NEAR(spec[0 * frames + 0], 13.6193562f, 2e-4f);
  EXPECT_NEAR(spec[1 * frames + 0], 13.6609402f, 2e-4f);
  EXPECT_NEAR(spec[20 * frames + 0], 144.166519f, 2e-3f);
  EXPECT_NEAR(spec[20 * frames + 1], 224.009033f, 2e-3f);
  EXPECT_NEAR(spec[21 * frames + 4], 207.109482f, 2e-3f);
  EXPECT_NEAR(spec[100 * frames + 3], 0.00100709f, 2e-5f);
  EXPECT_NEAR(spec[512 * frames + 7], 0.0308645f, 2e-4f);
}

}  // namespace edgevox_onnx
