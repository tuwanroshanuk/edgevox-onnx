import Foundation

class PocketTtsProgressHandler {
  func progress(samples: [Float], progress: Float) {
    print(String(format: "Received %d samples, Progress: %.2f%%", samples.count, progress * 100))
  }
}

func runPocketTtsDemo() {
  let pocket = edgevoxOnnxOfflineTtsPocketModelConfig(
    lmFlow: "./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx",
    lmMain: "./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx",
    encoder: "./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx",
    decoder: "./edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx",
    textConditioner: "./edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx",
    vocabJson: "./edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json",
    tokenScoresJson: "./edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json"
  )

  let modelConfig = edgevoxOnnxOfflineTtsModelConfig(numThreads: 2, pocket: pocket)
  var ttsConfig = edgevoxOnnxOfflineTtsConfig(model: modelConfig)
  ttsConfig.model.debug = 1

  let tts = EdgevoxOnnxOfflineTtsWrapper(config: &ttsConfig)

  let referenceAudioFile = "./edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav"
  let referenceWave = EdgevoxOnnxWaveWrapper.readWave(filename: referenceAudioFile)

  var genConfig = EdgevoxOnnxGenerationConfigSwift()
  genConfig.speed = 1.0
  genConfig.referenceAudio = referenceWave.samples
  genConfig.referenceSampleRate = referenceWave.sampleRate
  genConfig.extra = ["max_reference_audio_len": 15.0]

  let text = """
    Today as always, men fall into two groups: slaves and free men. Whoever \
    does not have two-thirds of his day for himself, is a slave, whatever \
    he may be: a statesman, a businessman, an official, or a scholar. \
    Friends fell out often because life was changing so fast. \
    The easiest thing in the world was to lose touch with someone.
    """

  func generateAndSave(
    outputFile: String, callback: TtsProgressCallbackWithArg? = nil,
    arg: UnsafeMutableRawPointer? = nil
  ) {
    let audio = tts.generateWithConfig(
      text: text,
      config: genConfig,
      callback: callback,
      arg: arg
    )

    if audio.save(filename: outputFile) == 1 {
      print("Saved to: \(outputFile)")
    } else {
      print("Failed to save to \(outputFile)")
    }
  }

  // -------------------------
  // Option 1: with callback
  // -------------------------
  let useCallback = true
  if useCallback {
    let progressHandler = PocketTtsProgressHandler()
    let arg = Unmanaged.passUnretained(progressHandler).toOpaque()

    let callback: TtsProgressCallbackWithArg = { samples, n, progress, arg in
      let handler = Unmanaged<PocketTtsProgressHandler>.fromOpaque(arg!).takeUnretainedValue()

      let buffer: [Float] =
        samples != nil ? Array(UnsafeBufferPointer(start: samples, count: Int(n))) : []
      handler.progress(samples: buffer, progress: progress)
      return 1  // continue generating
    }

    generateAndSave(outputFile: "generated-pocket-callback.wav", callback: callback, arg: arg)
  } else {
    // -------------------------
    // Option 2: direct generation
    // -------------------------
    generateAndSave(outputFile: "generated-pocket-direct.wav")
  }
}

// -------------------------
// Run demo
// -------------------------
@main
struct App {
  static func main() {
    runPocketTtsDemo()
  }
}
