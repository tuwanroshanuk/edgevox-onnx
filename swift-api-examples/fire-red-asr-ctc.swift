func run() {
  let model =
    "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx"
  let tokens =
    "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt"

  let fireRedAsrCtc = edgevoxOnnxOfflineFireRedAsrCtcModelConfig(
    model: model
  )

  let modelConfig = edgevoxOnnxOfflineModelConfig(
    tokens: tokens,
    debug: 1,
    fireRedAsrCtc: fireRedAsrCtc
  )

  let featConfig = edgevoxOnnxFeatureConfig()
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  let filePath = "./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/test_wavs/1.wav"
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
