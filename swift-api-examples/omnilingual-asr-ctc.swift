func run() {
  let model =
    "./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/model.int8.onnx"
  let tokens =
    "./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/tokens.txt"

  let omnilingual = edgevoxOnnxOfflineOmnilingualAsrCtcModelConfig(
    model: model
  )

  let modelConfig = edgevoxOnnxOfflineModelConfig(
    tokens: tokens,
    debug: 0,
    omnilingual: omnilingual
  )

  let featConfig = edgevoxOnnxFeatureConfig()
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  let filePath = "./edgevox-onnx-omnilingual-asr-1600-languages-300M-ctc-int8-2025-11-12/test_wavs/en.wav"
  let audio = EdgevoxOnnxWaveWrapper.readWave(filename: filePath)

  let result = recognizer.decode(samples: audio.samples, sampleRate: audio.sampleRate)

  print("\nresult is:\n\(result.text)")
  if result.timestamps.count != 0 {
    print("\ntimestamps is:\n\(result.timestamps)")
  }
}

@main
struct App {
  static func main() {
    run()
  }
}
