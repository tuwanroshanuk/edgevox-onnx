func run() {
  let encoder =
    "./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/encoder_model.ort"
  let decoder =
    "./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/decoder_model_merged.ort"
  let tokens =
    "./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/tokens.txt"

  let moonshine = edgevoxOnnxOfflineMoonshineModelConfig(
    encoder: encoder,
    mergedDecoder: decoder
  )

  let modelConfig = edgevoxOnnxOfflineModelConfig(
    tokens: tokens,
    debug: 1,
    moonshine: moonshine
  )

  let featConfig = edgevoxOnnxFeatureConfig()
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  let filePath = "./edgevox-onnx-moonshine-tiny-en-quantized-2026-02-27/test_wavs/0.wav"
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
