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

TEST(WfloatEmotion, RemovesGenericPiperFramingAndBlanks) {
  std::vector<int64_t> tokens = {1, 14, 0, 74, 0, 3, 0, 23, 0, 2};
  NormalizeWfloatPiperTokens(&tokens);
  EXPECT_EQ(tokens, (std::vector<int64_t>{14, 74, 3, 23}));
}

TEST(WfloatEmotion, JoinsSentencesBeforeAddingOneControlSuffix) {
  auto tokens = MergeWfloatPiperTokenGroups(
      {{1, 14, 0, 10, 0, 2}, {1, 32, 0, 18, 0, 2}});
  EXPECT_EQ(tokens, (std::vector<int64_t>{14, 10, 3, 32, 18}));
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
  std::vector<std::vector<int64_t>> groups;
  for (auto &item : result) {
    groups.push_back(std::move(item.tokens));
  }
  auto merged = MergeWfloatPiperTokenGroups(std::move(groups));
  merged.push_back(160);
  merged.push_back(182);
  std::ostringstream os;
  for (auto id : merged) {
    os << id << " ";
  }
  std::cout << "WFLOAT_NATIVE_IDS=" << os.str() << "\n";
}

}  // namespace edgevox_onnx
