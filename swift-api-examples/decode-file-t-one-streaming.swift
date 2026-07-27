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
  let filePath = "./edgevox-onnx-streaming-t-one-russian-2025-09-08/0.wav"
  let model =
    "./edgevox-onnx-streaming-t-one-russian-2025-09-08/model.onnx"
  let tokens = "./edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt"

  let toneCtcConfig = edgevoxOnnxOnlineToneCtcModelConfig(
    model: model)

  let modelConfig = edgevoxOnnxOnlineModelConfig(
    tokens: tokens,
    toneCtc: toneCtcConfig
  )

  let featConfig = edgevoxOnnxFeatureConfig(
    sampleRate: 8000,
    featureDim: 80
  )
  var config = edgevoxOnnxOnlineRecognizerConfig(
    featConfig: featConfig,  // not used
    modelConfig: modelConfig
  )

  let recognizer = EdgevoxOnnxRecognizer(config: &config)

  let fileURL: NSURL = NSURL(fileURLWithPath: filePath)
  let audioFile = try! AVAudioFile(forReading: fileURL as URL)

  let audioFormat = audioFile.processingFormat
  assert(audioFormat.sampleRate == 8000)
  assert(audioFormat.channelCount == 1)
  assert(audioFormat.commonFormat == AVAudioCommonFormat.pcmFormatFloat32)

  let audioFrameCount = UInt32(audioFile.length)
  let audioFileBuffer = AVAudioPCMBuffer(pcmFormat: audioFormat, frameCapacity: audioFrameCount)

  try! audioFile.read(into: audioFileBuffer!)
  let array: [Float]! = audioFileBuffer?.array()

  let leftPadding = [Float](repeating: 0.0, count: 2400)
  recognizer.acceptWaveform(samples: leftPadding, sampleRate: Int(audioFormat.sampleRate))

  recognizer.acceptWaveform(samples: array, sampleRate: Int(audioFormat.sampleRate))

  let tailPadding = [Float](repeating: 0.0, count: 4800)
  recognizer.acceptWaveform(samples: tailPadding, sampleRate: Int(audioFormat.sampleRate))

  recognizer.inputFinished()
  while recognizer.isReady() {
    recognizer.decode()
  }

  let result = recognizer.getResult()
  print("\nresult is:\n\(result.text)")
  print("\nresult is:\n\(result.timestamps)")
}

@main
struct App {
  static func main() {
    run()
  }
}
