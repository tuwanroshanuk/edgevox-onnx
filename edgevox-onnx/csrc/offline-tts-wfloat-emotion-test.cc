// edgevox-onnx/csrc/offline-tts-wfloat-emotion-test.cc
//
// Copyright (c)  2026  Xiaomi Corporation

#include "edgevox-onnx/csrc/offline-tts-wfloat-emotion.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "edgevox-onnx/csrc/offline-tts-vits-model-meta-data.h"
#include "edgevox-onnx/csrc/piper-phonemize-lexicon.h"
#include "gtest/gtest.h"

namespace edgevox_onnx {

TEST(WfloatEmotion, MapsAllEmotions) {
  const char *emotions[] = {"neutral", "joy",     "sadness",   "anger",
                            "fear",    "surprise", "dismissive", "confusion"};
  for (int32_t i = 0; i != 8; ++i) {
    WfloatEmotionControl control;
    ASSERT_TRUE(GetWfloatEmotionControl(emotions[i], 0.5, &control));
    EXPECT_EQ(control.emotion_token, 159 + i);
    EXPECT_EQ(control.intensity_token, 178);
  }
}

TEST(WfloatEmotion, NormalizesNameAndClampsIntensity) {
  WfloatEmotionControl control;
  ASSERT_TRUE(GetWfloatEmotionControl("  JOY ", -1, &control));
  EXPECT_EQ(control.emotion_token, 160);
  EXPECT_EQ(control.intensity_token, 173);

  ASSERT_TRUE(GetWfloatEmotionControl("joy", 1, &control));
  EXPECT_EQ(control.intensity_token, 182);

  ASSERT_TRUE(GetWfloatEmotionControl(
      "joy", std::numeric_limits<float>::quiet_NaN(), &control));
  EXPECT_EQ(control.intensity_token, 173);
}

TEST(WfloatEmotion, RejectsUnknownEmotion) {
  WfloatEmotionControl control;
  EXPECT_FALSE(GetWfloatEmotionControl("excited", 0.5, &control));
}

TEST(WfloatEmotion, MapsOfficialModelControlValues) {
  WfloatEmotionControl control;
  ASSERT_TRUE(GetWfloatEmotionControl("anger", 0.8, &control));
  EXPECT_EQ(control.emotion_token, 162);
  EXPECT_EQ(control.intensity_token, 181);
}

TEST(WfloatEmotion, ConvertsGenericPiperSequenceToRawWfloatPhonemes) {
  std::vector<int64_t> tokens = {1, 14, 0, 74, 0, 3, 0, 23, 0, 2};
  AppendWfloatEmotionControl(&tokens, {162, 182});
  EXPECT_EQ(tokens, (std::vector<int64_t>{14, 74, 3, 23, 162, 182}));
}

TEST(WfloatEmotion, KeepsEverySentenceIndependent) {
  std::vector<int64_t> tokens = {1, 14, 0, 10, 0};
  AppendWfloatEmotionControl(&tokens, {160, 178});
  EXPECT_EQ(tokens, (std::vector<int64_t>{14, 10, 160, 178}));
}

TEST(WfloatEmotion, OptionalNativeFrontendParityProbe) {
  const char *tokens_path = std::getenv("WFLOAT_TOKENS");
  const char *data_dir = std::getenv("WFLOAT_ESPEAK_DATA");
  if (!tokens_path || !data_dir) {
    GTEST_SKIP() << "Set WFLOAT_TOKENS and WFLOAT_ESPEAK_DATA to run";
  }

  OfflineTtsVitsModelMetaData metadata;
  metadata.is_piper = true;
  PiperPhonemizeLexicon frontend(tokens_path, data_dir, metadata);
  auto result = frontend.ConvertTextToTokenIds(
      "I cannot believe it. This changes everything!", "en-us");
  for (auto &item : result) {
    AppendWfloatEmotionControl(&item.tokens, {160, 182});
    std::ostringstream os;
    for (auto id : item.tokens) {
      os << id << " ";
    }
    std::cout << "WFLOAT_NATIVE_IDS=" << os.str() << "\n";
  }
}

}  // namespace edgevox_onnx
