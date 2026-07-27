#include "edgevox-onnx/csrc/offline-recognizer.h"
#include "edgevox-onnx/csrc/parse-options.h"

int main(int32_t argc, char *argv[]) {
  edgevox_onnx::ParseOptions po("help info");
  edgevox_onnx::OfflineRecognizerConfig config;
  config.Register(&po);
  po.PrintUsage();
  return 0;
}
