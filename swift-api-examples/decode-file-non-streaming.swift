import AVFoundation

extension AudioBuffer {
  func array() -> [Float] {
    return Array(UnsafeBufferPointer(self))
  }
}

extension AVAudioPCMBuffer {
  func array() -> [Float] {
    return self.audioBufferList.pointee.mBuffers.array()
  }
}

func run() {
  var recognizer: EdgevoxOnnxOfflineRecognizer
  var modelConfig: EdgevoxOnnxOfflineModelConfig
  var modelType = "whisper"
  // modelType = "paraformer"
  // modelType = "sense_voice"
  // modelType = "moonshine"

  if modelType == "whisper" {
    let encoder = "./edgevox-onnx-whisper-tiny.en/tiny.en-encoder.int8.onnx"
    let decoder = "./edgevox-onnx-whisper-tiny.en/tiny.en-decoder.int8.onnx"
    let tokens = "./edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt"

    let whisperConfig = edgevoxOnnxOfflineWhisperModelConfig(
      encoder: encoder,
      decoder: decoder
    )

    modelConfig = edgevoxOnnxOfflineModelConfig(
      tokens: tokens,
      whisper: whisperConfig,
      debug: 0,
      modelType: "whisper"
    )
  } else if modelType == "paraformer" {
    let model = "./edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx"
    let tokens = "./edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt"
    let paraformerConfig = edgevoxOnnxOfflineParaformerModelConfig(
      model: model
    )

    modelConfig = edgevoxOnnxOfflineModelConfig(
      tokens: tokens,
      paraformer: paraformerConfig,
      debug: 0,
      modelType: "paraformer"
    )
  } else if modelType == "sense_voice" {
    let model = "./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/model.int8.onnx"
    let tokens = "./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/tokens.txt"
    let senseVoiceConfig = edgevoxOnnxOfflineSenseVoiceModelConfig(
      model: model,
      useInverseTextNormalization: true
    )

    modelConfig = edgevoxOnnxOfflineModelConfig(
      tokens: tokens,
      debug: 0,
      senseVoice: senseVoiceConfig
    )
  } else if modelType == "moonshine" {
    let preprocessor = "./edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx"
    let encoder = "./edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx"
    let uncachedDecoder = "./edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx"
    let cachedDecoder = "./edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx"
    let tokens = "./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt"
    let moonshine = edgevoxOnnxOfflineMoonshineModelConfig(
      preprocessor: preprocessor,
      encoder: encoder,
      uncachedDecoder: uncachedDecoder,
      cachedDecoder: cachedDecoder
    )

    modelConfig = edgevoxOnnxOfflineModelConfig(
      tokens: tokens,
      debug: 0,
      moonshine: moonshine
    )
  } else {
    print("Please specify a supported modelType \(modelType)")
    return
  }

  let featConfig = edgevoxOnnxFeatureConfig(
    sampleRate: 16000,
    featureDim: 80
  )
  var config = edgevoxOnnxOfflineRecognizerConfig(
    featConfig: featConfig,
    modelConfig: modelConfig
  )

  recognizer = EdgevoxOnnxOfflineRecognizer(config: &config)

  var filePath = "./edgevox-onnx-whisper-tiny.en/test_wavs/0.wav"
  if modelType == "sense_voice" {
    filePath = "./edgevox-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17/test_wavs/zh.wav"
  } else if modelType == "moonshine" {
    filePath = "./edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav"
  }
  let fileURL: NSURL = NSURL(fileURLWithPath: filePath)
  let audioFile = try! AVAudioFile(forReading: fileURL as URL)

  let audioFormat = audioFile.processingFormat
  assert(audioFormat.channelCount == 1)
  assert(audioFormat.commonFormat == AVAudioCommonFormat.pcmFormatFloat32)

  let audioFrameCount = UInt32(audioFile.length)
  let audioFileBuffer = AVAudioPCMBuffer(pcmFormat: audioFormat, frameCapacity: audioFrameCount)

  try! audioFile.read(into: audioFileBuffer!)
  let array: [Float]! = audioFileBuffer?.array()
  let result = recognizer.decode(samples: array, sampleRate: Int(audioFormat.sampleRate))
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
