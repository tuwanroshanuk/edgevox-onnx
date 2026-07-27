func run() {
  let encoderAdaptor =
    "./edgevox-onnx-funasr-nano-int8-2025-12-30/encoder_adaptor.int8.onnx"
  let llm =
    "./edgevox-onnx-funasr-nano-int8-2025-12-30/llm.int8.onnx"
  let embedding =
    "./edgevox-onnx-funasr-nano-int8-2025-12-30/embedding.int8.onnx"
  let tokenizer =
    "./edgevox-onnx-funasr-nano-int8-2025-12-30/Qwen3-0.6B"

  let funasrNano = edgevoxOnnxOfflineFunASRNanoModelConfig(
    encoderAdaptor: encoderAdaptor,
    llm: llm,
    embedding: embedding,
    tokenizer: tokenizer
  )

  let modelConfig = edgevoxOnnxOfflineModelConfig(
    tokens: "",
    debug: 1,
    funasrNano: funasrNano
  )

  let featConfig = edgevoxOnnxFeatureConfig()
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  let filePath = "./edgevox-onnx-funasr-nano-int8-2025-12-30/test_wavs/lyrics.wav"
  let audio = EdgevoxOnnxWaveWrapper.readWave(filename: filePath)

  let result = recognizer.decode(samples: audio.samples, sampleRate: audio.sampleRate)
  print("decode done")

  print("\nresult is:\n\(result.text)")
  if !result.timestamps.isEmpty {
    print("\ntimestamps is:\n\(result.timestamps)")
  }
}

@main
struct App {
  static func main() {
    run()
  }
}

