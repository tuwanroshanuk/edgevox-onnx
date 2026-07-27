package com.k2fsa.sherpa.onnx

fun main() {
  testPocketTts()
}

fun testPocketTts() {
  // see https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models
  val config = OfflineTtsConfig(
    model=OfflineTtsModelConfig(
      pocket=OfflineTtsPocketModelConfig(
        lmFlow="./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx",
        lmMain="./edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx",
        encoder="./edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx",
        decoder="./edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx",
        textConditioner="./edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx",
        vocabJson="./edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json",
        tokenScoresJson="./edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json",
      ),
      numThreads=2,
      debug=true,
    ),
  )
  val tts = OfflineTts(config=config)

  val referenceAudioFilename = "./edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav"
  val wave = WaveReader.readWave(
      filename = referenceAudioFilename,
  )

  val genConfig = GenerationConfig(
    referenceAudio = wave.samples,
    referenceSampleRate = wave.sampleRate,
    numSteps = 5,
    extra = mapOf(
        "temperature" to "0.7",
        "chunk_size" to "15",
    )
  )

  val text = "Today as always, men fall into two groups: slaves and free men. Whoever does not have two-thirds of his day for himself, is a slave, whatever he may be, a statesman, a businessman, an official, or a scholar."

  val audio = tts.generateWithConfigAndCallback(text=text, config=genConfig, callback=::callback)
  audio.save(filename="out-bria.wav")
  tts.release()
  println("Saved to out-bria.wav")
}

fun callback(samples: FloatArray): Int {
  println("callback got called with ${samples.size} samples")

  // 1 means to continue
  // 0 means to stop
  return 1
}
