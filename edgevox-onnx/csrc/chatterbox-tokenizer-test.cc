#include "gtest/gtest.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "edgevox-onnx/csrc/funasr-nano-tokenizer.h"

namespace edgevox_onnx {

TEST(ChatterboxTokenizer, MatchesOfficialGpt2Tokenizer) {
  const char *dir = std::getenv("CHATTERBOX_MODEL_DIR");
  if (!dir) GTEST_SKIP() << "CHATTERBOX_MODEL_DIR is not set";
  FunASRNanoTokenizer tokenizer(dir);
  const std::vector<int64_t> expected = {
      15496, 422, 609, 1436, 3524, 22278, 13, 220, 50274,
      15282, 45973, 318, 2491, 15726, 13};
  EXPECT_EQ(tokenizer.Encode(
                "Hello from Chatterbox Turbo. [chuckle] Voice cloning is "
                "running locally."),
            expected);
}

}  // namespace edgevox_onnx
