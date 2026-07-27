func run() {
  let model =
    "./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/model.int8.onnx"
  let tokens =
    "./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/tokens.txt"

  let medasr = edgevoxOnnxOfflineMedAsrCtcModelConfig(
    model: model
  )

  let modelConfig = edgevoxOnnxOfflineModelConfig(
    tokens: tokens,
    debug: 1,
    medasr: medasr
  )

  let featConfig = edgevoxOnnxFeatureConfig()
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  let filePath = "./edgevox-onnx-medasr-ctc-en-int8-2025-12-25/test_wavs/0.wav"
  let audio = EdgevoxOnnxWaveWrapper.readWave(filename: filePath)

  let result = recognizer.decode(samples: audio.samples, sampleRate: audio.sampleRate)
  print("decode done")

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
