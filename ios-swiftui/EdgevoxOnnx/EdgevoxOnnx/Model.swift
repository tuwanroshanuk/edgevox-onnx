import Foundation

func getResource(_ forResource: String, _ ofType: String) -> String {
  let path = Bundle.main.path(forResource: forResource, ofType: ofType)
  precondition(
    path != nil,
    "\(forResource).\(ofType) does not exist!\n" + "Remember to change \n"
      + "  Build Phases -> Copy Bundle Resources\n" + "to add it!"
  )
  return path!
}
/// Please refer to
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/index.html
/// to download pre-trained models

/// edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20 (Bilingual, Chinese + English)
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/zipformer-transducer-models.html
func getBilingualStreamZhEnZipformer20230220() -> EdgevoxOnnxOnlineModelConfig {
  let encoder = getResource("encoder-epoch-99-avg-1", "onnx")
  let decoder = getResource("decoder-epoch-99-avg-1", "onnx")
  let joiner = getResource("joiner-epoch-99-avg-1", "onnx")
  let tokens = getResource("tokens", "txt")

  return edgevoxOnnxOnlineModelConfig(
    tokens: tokens,
    transducer: edgevoxOnnxOnlineTransducerModelConfig(
      encoder: encoder,
      decoder: decoder,
      joiner: joiner),
    numThreads: 2,
    modelType: "zipformer"
  )
}

// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-paraformer/index.html
func getBilingualStreamingZhEnParaformer() -> EdgevoxOnnxOnlineModelConfig {
  let encoder = getResource("encoder.int8", "onnx")
  let decoder = getResource("decoder.int8", "onnx")
  let tokens = getResource("tokens", "txt")

  return edgevoxOnnxOnlineModelConfig(
    tokens: tokens,
    paraformer: edgevoxOnnxOnlineParaformerModelConfig(
      encoder: encoder,
      decoder: decoder),
    numThreads: 1,
    modelType: "paraformer"
  )
}

// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/whisper/tiny.en.html#tiny-en
//
func getLanguageIdentificationTiny() -> EdgevoxOnnxSpokenLanguageIdentificationConfig
 {
  let encoder = getResource("tiny-encoder.int8", "onnx")
  let decoder = getResource("tiny-decoder.int8", "onnx")
    
    let whisperConfig = edgevoxOnnxSpokenLanguageIdentificationWhisperConfig(
      encoder: encoder,
      decoder: decoder
    )

    let config = edgevoxOnnxSpokenLanguageIdentificationConfig(
      whisper: whisperConfig,
      numThreads: 1,
      debug: 1,
      provider: "cpu"
    )
    return config
}


/// Please refer to
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/index.html
/// to add more models if you need
