/// swift-api-examples/EdgevoxOnnx.swift
/// Copyright (c)  2023  Xiaomi Corporation

import Foundation  // For NSString

/// Convert a String from swift to a `const char*` so that we can pass it to
/// the C language.
///
/// - Parameters:
///   - s: The String to convert.
/// - Returns: A pointer that can be passed to C as `const char*`

func toCPointer(_ s: String) -> UnsafePointer<Int8>! {
  let cs = (s as NSString).utf8String
  return UnsafePointer<Int8>(cs)
}

/// Return an instance of EdgevoxOnnxOnlineTransducerModelConfig.
///
/// Please refer to
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/index.html
/// to download the required `.onnx` files.
///
/// - Parameters:
///   - encoder: Path to encoder.onnx
///   - decoder: Path to decoder.onnx
///   - joiner: Path to joiner.onnx
///
/// - Returns: Return an instance of EdgevoxOnnxOnlineTransducerModelConfig
func edgevoxOnnxOnlineTransducerModelConfig(
  encoder: String = "",
  decoder: String = "",
  joiner: String = ""
) -> EdgevoxOnnxOnlineTransducerModelConfig {
  return EdgevoxOnnxOnlineTransducerModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    joiner: toCPointer(joiner)
  )
}

/// Return an instance of EdgevoxOnnxOnlineParaformerModelConfig.
///
/// Please refer to
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-paraformer/index.html
/// to download the required `.onnx` files.
///
/// - Parameters:
///   - encoder: Path to encoder.onnx
///   - decoder: Path to decoder.onnx
///
/// - Returns: Return an instance of EdgevoxOnnxOnlineParaformerModelConfig
func edgevoxOnnxOnlineParaformerModelConfig(
  encoder: String = "",
  decoder: String = ""
) -> EdgevoxOnnxOnlineParaformerModelConfig {
  return EdgevoxOnnxOnlineParaformerModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder)
  )
}

func edgevoxOnnxOnlineZipformer2CtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOnlineZipformer2CtcModelConfig {
  return EdgevoxOnnxOnlineZipformer2CtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOnlineNemoCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOnlineNemoCtcModelConfig {
  return EdgevoxOnnxOnlineNemoCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOnlineToneCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOnlineToneCtcModelConfig {
  return EdgevoxOnnxOnlineToneCtcModelConfig(
    model: toCPointer(model)
  )
}

/// Return an instance of EdgevoxOnnxOnlineModelConfig.
///
/// Please refer to
/// https://k2-fsa.github.io/sherpa/onnx/pretrained_models/index.html
/// to download the required `.onnx` files.
///
/// - Parameters:
///   - tokens: Path to tokens.txt
///   - numThreads:  Number of threads to use for neural network computation.
///
/// - Returns: Return an instance of EdgevoxOnnxOnlineTransducerModelConfig
func edgevoxOnnxOnlineModelConfig(
  tokens: String,
  transducer: EdgevoxOnnxOnlineTransducerModelConfig = edgevoxOnnxOnlineTransducerModelConfig(),
  paraformer: EdgevoxOnnxOnlineParaformerModelConfig = edgevoxOnnxOnlineParaformerModelConfig(),
  zipformer2Ctc: EdgevoxOnnxOnlineZipformer2CtcModelConfig =
    edgevoxOnnxOnlineZipformer2CtcModelConfig(),
  numThreads: Int = 1,
  provider: String = "cpu",
  debug: Int = 0,
  modelType: String = "",
  modelingUnit: String = "cjkchar",
  bpeVocab: String = "",
  tokensBuf: String = "",
  tokensBufSize: Int = 0,
  nemoCtc: EdgevoxOnnxOnlineNemoCtcModelConfig = edgevoxOnnxOnlineNemoCtcModelConfig(),
  toneCtc: EdgevoxOnnxOnlineToneCtcModelConfig = edgevoxOnnxOnlineToneCtcModelConfig()
) -> EdgevoxOnnxOnlineModelConfig {
  return EdgevoxOnnxOnlineModelConfig(
    transducer: transducer,
    paraformer: paraformer,
    zipformer2_ctc: zipformer2Ctc,
    tokens: toCPointer(tokens),
    num_threads: Int32(numThreads),
    provider: toCPointer(provider),
    debug: Int32(debug),
    model_type: toCPointer(modelType),
    modeling_unit: toCPointer(modelingUnit),
    bpe_vocab: toCPointer(bpeVocab),
    tokens_buf: toCPointer(tokensBuf),
    tokens_buf_size: Int32(tokensBufSize),
    nemo_ctc: nemoCtc,
    t_one_ctc: toneCtc
  )
}

func edgevoxOnnxFeatureConfig(
  sampleRate: Int = 16000,
  featureDim: Int = 80
) -> EdgevoxOnnxFeatureConfig {
  return EdgevoxOnnxFeatureConfig(
    sample_rate: Int32(sampleRate),
    feature_dim: Int32(featureDim))
}

func edgevoxOnnxOnlineCtcFstDecoderConfig(
  graph: String = "",
  maxActive: Int = 3000
) -> EdgevoxOnnxOnlineCtcFstDecoderConfig {
  return EdgevoxOnnxOnlineCtcFstDecoderConfig(
    graph: toCPointer(graph),
    max_active: Int32(maxActive))
}

func edgevoxOnnxHomophoneReplacerConfig(
  dictDir: String = "",
  lexicon: String = "",
  ruleFsts: String = ""
) -> EdgevoxOnnxHomophoneReplacerConfig {
  return EdgevoxOnnxHomophoneReplacerConfig(
    dict_dir: toCPointer(dictDir),
    lexicon: toCPointer(lexicon),
    rule_fsts: toCPointer(ruleFsts))
}

func edgevoxOnnxOnlineRecognizerConfig(
  featConfig: EdgevoxOnnxFeatureConfig,
  modelConfig: EdgevoxOnnxOnlineModelConfig,
  enableEndpoint: Bool = false,
  rule1MinTrailingSilence: Float = 2.4,
  rule2MinTrailingSilence: Float = 1.2,
  rule3MinUtteranceLength: Float = 30,
  decodingMethod: String = "greedy_search",
  maxActivePaths: Int = 4,
  hotwordsFile: String = "",
  hotwordsScore: Float = 1.5,
  ctcFstDecoderConfig: EdgevoxOnnxOnlineCtcFstDecoderConfig = edgevoxOnnxOnlineCtcFstDecoderConfig(),
  ruleFsts: String = "",
  ruleFars: String = "",
  blankPenalty: Float = 0.0,
  hotwordsBuf: String = "",
  hotwordsBufSize: Int = 0,
  hr: EdgevoxOnnxHomophoneReplacerConfig = edgevoxOnnxHomophoneReplacerConfig()
) -> EdgevoxOnnxOnlineRecognizerConfig {
  return EdgevoxOnnxOnlineRecognizerConfig(
    feat_config: featConfig,
    model_config: modelConfig,
    decoding_method: toCPointer(decodingMethod),
    max_active_paths: Int32(maxActivePaths),
    enable_endpoint: enableEndpoint ? 1 : 0,
    rule1_min_trailing_silence: rule1MinTrailingSilence,
    rule2_min_trailing_silence: rule2MinTrailingSilence,
    rule3_min_utterance_length: rule3MinUtteranceLength,
    hotwords_file: toCPointer(hotwordsFile),
    hotwords_score: hotwordsScore,
    ctc_fst_decoder_config: ctcFstDecoderConfig,
    rule_fsts: toCPointer(ruleFsts),
    rule_fars: toCPointer(ruleFars),
    blank_penalty: blankPenalty,
    hotwords_buf: toCPointer(hotwordsBuf),
    hotwords_buf_size: Int32(hotwordsBufSize),
    hr: hr
  )
}

/// Wrapper for recognition result.
///
/// Usage:
///
///  let result = recognizer.getResult()
///  print("text: \(result.text)")
///
class EdgevoxOnnxOnlineRecongitionResult {
  /// A pointer to the underlying counterpart in C
  private let result: UnsafePointer<EdgevoxOnnxOnlineRecognizerResult>

  private lazy var _text: String = {
    guard let cstr = result.pointee.text else { return "" }
    return String(cString: cstr)
  }()

  private lazy var _tokens: [String] = {
    guard let tokensPointer = result.pointee.tokens_arr else { return [] }
    return (0..<count).compactMap { index in
      guard let ptr = tokensPointer[index] else { return nil }
      return String(cString: ptr)
    }
  }()

  private lazy var _timestamps: [Float] = {
    guard let timestampsPointer = result.pointee.timestamps else { return [] }
    return (0..<count).map { index in timestampsPointer[index] }
  }()

  init(result: UnsafePointer<EdgevoxOnnxOnlineRecognizerResult>) {
    self.result = result
  }

  deinit {
    EdgevoxOnnxDestroyOnlineRecognizerResult(result)
  }

  /// Return the actual recognition result.
  /// For English models, it contains words separated by spaces.
  /// For Chinese models, it contains Chinese words.
  var text: String { _text }

  var count: Int { Int(result.pointee.count) }

  var tokens: [String] { _tokens }

  var timestamps: [Float] { _timestamps }
}

class EdgevoxOnnxRecognizer {
  /// A pointer to the underlying counterpart in C
  private let recognizer: OpaquePointer
  private var stream: OpaquePointer
  private let lock = NSLock()  // for thread-safe stream replacement

  /// Constructor taking a model config
  init(
    config: UnsafePointer<EdgevoxOnnxOnlineRecognizerConfig>
  ) {
    self.recognizer = EdgevoxOnnxCreateOnlineRecognizer(config)
    self.stream = EdgevoxOnnxCreateOnlineStream(recognizer)
  }

  deinit {
    EdgevoxOnnxDestroyOnlineStream(stream)
    EdgevoxOnnxDestroyOnlineRecognizer(recognizer)
  }

  /// Decode wave samples.
  ///
  /// - Parameters:
  ///   - samples: Audio samples normalized to the range [-1, 1]
  ///   - sampleRate: Sample rate of the input audio samples. Must match
  ///                 the one expected by the model.
  func acceptWaveform(samples: [Float], sampleRate: Int = 16_000) {
    EdgevoxOnnxOnlineStreamAcceptWaveform(stream, Int32(sampleRate), samples, Int32(samples.count))
  }

  func isReady() -> Bool {
    return EdgevoxOnnxIsOnlineStreamReady(recognizer, stream) != 0
  }

  /// If there are enough number of feature frames, it invokes the neural
  /// network computation and decoding. Otherwise, it is a no-op.
  func decode() {
    EdgevoxOnnxDecodeOnlineStream(recognizer, stream)
  }

  /// Get the decoding results so far
  func getResult() -> EdgevoxOnnxOnlineRecongitionResult {
    guard let result = EdgevoxOnnxGetOnlineStreamResult(recognizer, stream) else {
      fatalError("EdgevoxOnnxGetOnlineStreamResult returned nil")
    }
    return EdgevoxOnnxOnlineRecongitionResult(result: result)
  }

  /// Reset the recognizer, which clears the neural network model state
  /// and the state for decoding.
  /// If hotwords is an empty string, it just recreates the decoding stream
  /// If hotwords is not empty, it will create a new decoding stream with
  /// the given hotWords appended to the default hotwords.
  func reset(hotwords: String? = nil) {
    guard let words = hotwords, !words.isEmpty else {
      EdgevoxOnnxOnlineStreamReset(recognizer, stream)
      return
    }

    words.withCString { cString in
      guard let newStream = EdgevoxOnnxCreateOnlineStreamWithHotwords(recognizer, cString) else {
        fatalError("EdgevoxOnnxCreateOnlineStreamWithHotwords returned nil")
      }
      lock.lock()
      // lock while release and replace stream
      EdgevoxOnnxDestroyOnlineStream(stream)
      stream = newStream
      lock.unlock()
    }
  }

  /// Signal that no more audio samples would be available.
  /// After this call, you cannot call acceptWaveform() any more.
  func inputFinished() {
    EdgevoxOnnxOnlineStreamInputFinished(stream)
  }

  /// Return true is an endpoint has been detected.
  func isEndpoint() -> Bool {
    return EdgevoxOnnxOnlineStreamIsEndpoint(recognizer, stream) != 0
  }
}

// For offline APIs

func edgevoxOnnxOfflineTransducerModelConfig(
  encoder: String = "",
  decoder: String = "",
  joiner: String = ""
) -> EdgevoxOnnxOfflineTransducerModelConfig {
  return EdgevoxOnnxOfflineTransducerModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    joiner: toCPointer(joiner)
  )
}

func edgevoxOnnxOfflineParaformerModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineParaformerModelConfig {
  return EdgevoxOnnxOfflineParaformerModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineZipformerCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineZipformerCtcModelConfig {
  return EdgevoxOnnxOfflineZipformerCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineWenetCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineWenetCtcModelConfig {
  return EdgevoxOnnxOfflineWenetCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineOmnilingualAsrCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig {
  return EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineMedAsrCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineMedAsrCtcModelConfig {
  return EdgevoxOnnxOfflineMedAsrCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineFireRedAsrCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineFireRedAsrCtcModelConfig {
  return EdgevoxOnnxOfflineFireRedAsrCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineNemoEncDecCtcModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineNemoEncDecCtcModelConfig {
  return EdgevoxOnnxOfflineNemoEncDecCtcModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineDolphinModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineDolphinModelConfig {
  return EdgevoxOnnxOfflineDolphinModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineWhisperModelConfig(
  encoder: String = "",
  decoder: String = "",
  language: String = "",
  task: String = "transcribe",
  tailPaddings: Int = -1,
  enableTokenTimestamps: Bool = false,
  enableSegmentTimestamps: Bool = false
) -> EdgevoxOnnxOfflineWhisperModelConfig {
  return EdgevoxOnnxOfflineWhisperModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    language: toCPointer(language),
    task: toCPointer(task),
    tail_paddings: Int32(tailPaddings),
    enable_token_timestamps: enableTokenTimestamps ? 1 : 0,
    enable_segment_timestamps: enableSegmentTimestamps ? 1 : 0
  )
}

func edgevoxOnnxOfflineCanaryModelConfig(
  encoder: String = "",
  decoder: String = "",
  srcLang: String = "en",
  tgtLang: String = "en",
  usePnc: Bool = true
) -> EdgevoxOnnxOfflineCanaryModelConfig {
  return EdgevoxOnnxOfflineCanaryModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    src_lang: toCPointer(srcLang),
    tgt_lang: toCPointer(tgtLang),
    use_pnc: usePnc ? 1 : 0
  )
}

func edgevoxOnnxOfflineCohereTranscribeModelConfig(
  encoder: String = "",
  decoder: String = "",
  language: String = "",
  usePunct: Bool = true,
  useInverseTextNormalization: Bool = true
) -> EdgevoxOnnxOfflineCohereTranscribeModelConfig {
  return EdgevoxOnnxOfflineCohereTranscribeModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    language: toCPointer(language),
    use_punct: usePunct ? 1 : 0,
    use_itn: useInverseTextNormalization ? 1 : 0
  )
}

func edgevoxOnnxOfflineFireRedAsrModelConfig(
  encoder: String = "",
  decoder: String = ""
) -> EdgevoxOnnxOfflineFireRedAsrModelConfig {
  return EdgevoxOnnxOfflineFireRedAsrModelConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder)
  )
}

// there are two versions of Moonshine
// For v1, you need four models: preprocessor, encoder, uncachedDecoder, cachedDecoder
// For v2, you need two models: encoder, mergedDecoder
func edgevoxOnnxOfflineMoonshineModelConfig(
  preprocessor: String = "",
  encoder: String = "",
  uncachedDecoder: String = "",
  cachedDecoder: String = "",
  mergedDecoder: String = ""
) -> EdgevoxOnnxOfflineMoonshineModelConfig {
  return EdgevoxOnnxOfflineMoonshineModelConfig(
    preprocessor: toCPointer(preprocessor),
    encoder: toCPointer(encoder),
    uncached_decoder: toCPointer(uncachedDecoder),
    cached_decoder: toCPointer(cachedDecoder),
    merged_decoder: toCPointer(mergedDecoder)
  )
}

func edgevoxOnnxOfflineQwen3ASRModelConfig(
  convFrontend: String = "",
  encoder: String = "",
  decoder: String = "",
  tokenizer: String = "",
  maxTotalLen: Int = 512,
  maxNewTokens: Int = 128,
  temperature: Float = 1e-6,
  topP: Float = 0.8,
  seed: Int = 42,
  hotwords: String = ""
) -> EdgevoxOnnxOfflineQwen3ASRModelConfig {
  return EdgevoxOnnxOfflineQwen3ASRModelConfig(
    conv_frontend: toCPointer(convFrontend),
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    tokenizer: toCPointer(tokenizer),
    max_total_len: Int32(maxTotalLen),
    max_new_tokens: Int32(maxNewTokens),
    temperature: temperature,
    top_p: topP,
    seed: Int32(seed),
    hotwords: toCPointer(hotwords)
  )
}

func edgevoxOnnxOfflineTdnnModelConfig(
  model: String = ""
) -> EdgevoxOnnxOfflineTdnnModelConfig {
  return EdgevoxOnnxOfflineTdnnModelConfig(
    model: toCPointer(model)
  )
}

func edgevoxOnnxOfflineSenseVoiceModelConfig(
  model: String = "",
  language: String = "",
  useInverseTextNormalization: Bool = false
) -> EdgevoxOnnxOfflineSenseVoiceModelConfig {
  return EdgevoxOnnxOfflineSenseVoiceModelConfig(
    model: toCPointer(model),
    language: toCPointer(language),
    use_itn: useInverseTextNormalization ? 1 : 0
  )
}

func edgevoxOnnxOfflineLMConfig(
  model: String = "",
  scale: Float = 1.0
) -> EdgevoxOnnxOfflineLMConfig {
  return EdgevoxOnnxOfflineLMConfig(
    model: toCPointer(model),
    scale: scale
  )
}

func edgevoxOnnxOfflineFunASRNanoModelConfig(
  encoderAdaptor: String = "",
  llm: String = "",
  embedding: String = "",
  tokenizer: String = "",
  systemPrompt: String = "You are a helpful assistant.",
  userPrompt: String = "语音转写：",
  maxNewTokens: Int = 512,
  temperature: Float = 1e-6,
  topP: Float = 0.8,
  seed: Int = 42,
  language: String = "",
  itn: Bool = true,
  hotwords: String = ""
) -> EdgevoxOnnxOfflineFunASRNanoModelConfig {
  return EdgevoxOnnxOfflineFunASRNanoModelConfig(
    encoder_adaptor: toCPointer(encoderAdaptor),
    llm: toCPointer(llm),
    embedding: toCPointer(embedding),
    tokenizer: toCPointer(tokenizer),
    system_prompt: toCPointer(systemPrompt),
    user_prompt: toCPointer(userPrompt),
    max_new_tokens: Int32(maxNewTokens),
    temperature: temperature,
    top_p: topP,
    seed: Int32(seed),
    language: toCPointer(language),
    itn: itn ? 1 : 0,
    hotwords: toCPointer(hotwords)
  )
}

func edgevoxOnnxOfflineModelConfig(
  tokens: String,
  transducer: EdgevoxOnnxOfflineTransducerModelConfig = edgevoxOnnxOfflineTransducerModelConfig(),
  paraformer: EdgevoxOnnxOfflineParaformerModelConfig = edgevoxOnnxOfflineParaformerModelConfig(),
  nemoCtc: EdgevoxOnnxOfflineNemoEncDecCtcModelConfig = edgevoxOnnxOfflineNemoEncDecCtcModelConfig(),
  whisper: EdgevoxOnnxOfflineWhisperModelConfig = edgevoxOnnxOfflineWhisperModelConfig(),
  tdnn: EdgevoxOnnxOfflineTdnnModelConfig = edgevoxOnnxOfflineTdnnModelConfig(),
  numThreads: Int = 1,
  provider: String = "cpu",
  debug: Int = 0,
  modelType: String = "",
  modelingUnit: String = "cjkchar",
  bpeVocab: String = "",
  teleSpeechCtc: String = "",
  senseVoice: EdgevoxOnnxOfflineSenseVoiceModelConfig = edgevoxOnnxOfflineSenseVoiceModelConfig(),
  moonshine: EdgevoxOnnxOfflineMoonshineModelConfig = edgevoxOnnxOfflineMoonshineModelConfig(),
  fireRedAsr: EdgevoxOnnxOfflineFireRedAsrModelConfig = edgevoxOnnxOfflineFireRedAsrModelConfig(),
  dolphin: EdgevoxOnnxOfflineDolphinModelConfig = edgevoxOnnxOfflineDolphinModelConfig(),
  zipformerCtc: EdgevoxOnnxOfflineZipformerCtcModelConfig =
    edgevoxOnnxOfflineZipformerCtcModelConfig(),
  canary: EdgevoxOnnxOfflineCanaryModelConfig = edgevoxOnnxOfflineCanaryModelConfig(),
  wenetCtc: EdgevoxOnnxOfflineWenetCtcModelConfig =
    edgevoxOnnxOfflineWenetCtcModelConfig(),
  omnilingual: EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig =
    edgevoxOnnxOfflineOmnilingualAsrCtcModelConfig(),
  medasr: EdgevoxOnnxOfflineMedAsrCtcModelConfig =
    edgevoxOnnxOfflineMedAsrCtcModelConfig(),
  funasrNano: EdgevoxOnnxOfflineFunASRNanoModelConfig =
    edgevoxOnnxOfflineFunASRNanoModelConfig(),
  fireRedAsrCtc: EdgevoxOnnxOfflineFireRedAsrCtcModelConfig =
    edgevoxOnnxOfflineFireRedAsrCtcModelConfig(),
  qwen3Asr: EdgevoxOnnxOfflineQwen3ASRModelConfig =
    edgevoxOnnxOfflineQwen3ASRModelConfig(),
  cohereTranscribe: EdgevoxOnnxOfflineCohereTranscribeModelConfig =
    edgevoxOnnxOfflineCohereTranscribeModelConfig()
) -> EdgevoxOnnxOfflineModelConfig {
  return EdgevoxOnnxOfflineModelConfig(
    transducer: transducer,
    paraformer: paraformer,
    nemo_ctc: nemoCtc,
    whisper: whisper,
    tdnn: tdnn,
    tokens: toCPointer(tokens),
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider),
    model_type: toCPointer(modelType),
    modeling_unit: toCPointer(modelingUnit),
    bpe_vocab: toCPointer(bpeVocab),
    telespeech_ctc: toCPointer(teleSpeechCtc),
    sense_voice: senseVoice,
    moonshine: moonshine,
    fire_red_asr: fireRedAsr,
    dolphin: dolphin,
    zipformer_ctc: zipformerCtc,
    canary: canary,
    wenet_ctc: wenetCtc,
    omnilingual: omnilingual,
    medasr: medasr,
    funasr_nano: funasrNano,
    fire_red_asr_ctc: fireRedAsrCtc,
    qwen3_asr: qwen3Asr,
    cohere_transcribe: cohereTranscribe
  )
}

func edgevoxOnnxOfflineRecognizerConfig(
  featConfig: EdgevoxOnnxFeatureConfig,
  modelConfig: EdgevoxOnnxOfflineModelConfig,
  lmConfig: EdgevoxOnnxOfflineLMConfig = edgevoxOnnxOfflineLMConfig(),
  decodingMethod: String = "greedy_search",
  maxActivePaths: Int = 4,
  hotwordsFile: String = "",
  hotwordsScore: Float = 1.5,
  ruleFsts: String = "",
  ruleFars: String = "",
  blankPenalty: Float = 0.0,
  hr: EdgevoxOnnxHomophoneReplacerConfig = edgevoxOnnxHomophoneReplacerConfig()
) -> EdgevoxOnnxOfflineRecognizerConfig {
  return EdgevoxOnnxOfflineRecognizerConfig(
    feat_config: featConfig,
    model_config: modelConfig,
    lm_config: lmConfig,
    decoding_method: toCPointer(decodingMethod),
    max_active_paths: Int32(maxActivePaths),
    hotwords_file: toCPointer(hotwordsFile),
    hotwords_score: hotwordsScore,
    rule_fsts: toCPointer(ruleFsts),
    rule_fars: toCPointer(ruleFars),
    blank_penalty: blankPenalty,
    hr: hr
  )
}

class EdgevoxOnnxOfflineRecongitionResult {
  /// A pointer to the underlying counterpart in C
  let result: UnsafePointer<EdgevoxOnnxOfflineRecognizerResult>

  private lazy var _text: String = {
    guard let cstr = result.pointee.text else { return "" }
    return String(cString: cstr)
  }()

  private lazy var _timestamps: [Float] = {
    guard let p = result.pointee.timestamps else { return [] }
    return (0..<result.pointee.count).map { p[Int($0)] }
  }()

  private lazy var _durations: [Float] = {
    guard let p = result.pointee.durations else { return [] }
    return (0..<result.pointee.count).map { p[Int($0)] }
  }()

  private lazy var _lang: String = {
    guard let cstr = result.pointee.lang else { return "" }
    return String(cString: cstr)
  }()

  private lazy var _emotion: String = {
    guard let cstr = result.pointee.emotion else { return "" }
    return String(cString: cstr)
  }()

  private lazy var _event: String = {
    guard let cstr = result.pointee.event else { return "" }
    return String(cString: cstr)
  }()

  private lazy var _segmentTimestamps: [Float] = {
    guard let p = result.pointee.segment_timestamps else { return [] }
    return (0..<result.pointee.segment_count).map { p[Int($0)] }
  }()

  private lazy var _segmentDurations: [Float] = {
    guard let p = result.pointee.segment_durations else { return [] }
    return (0..<result.pointee.segment_count).map { p[Int($0)] }
  }()

  private lazy var _segmentTexts: [String] = {
    guard let arr = result.pointee.segment_texts_arr else { return [] }
    return (0..<result.pointee.segment_count).compactMap { idx -> String? in
      guard let ptr = arr[Int(idx)] else { return nil }
      return String(cString: ptr)
    }
  }()

  /// Return the actual recognition result.
  /// For English models, it contains words separated by spaces.
  /// For Chinese models, it contains Chinese words.
  var text: String { _text }
  var count: Int { Int(result.pointee.count) }
  var timestamps: [Float] { _timestamps }

  // Non-empty for TDT models. Empty for all other non-TDT models
  var durations: [Float] { _durations }

  // For SenseVoice models, it can be zh, en, ja, yue, ko
  // where zh is for Chinese
  // en is for English
  // ja is for Japanese
  // yue is for Cantonese
  // ko is for Korean
  var lang: String { _lang }

  // for SenseVoice models
  var emotion: String { _emotion }

  // for SenseVoice models
  var event: String { _event }

  // Segment-level timestamps (for Whisper with segment timestamps enabled)
  var segmentCount: Int { Int(result.pointee.segment_count) }
  var segmentTimestamps: [Float] { _segmentTimestamps }
  var segmentDurations: [Float] { _segmentDurations }
  var segmentTexts: [String] { _segmentTexts }

  init(result: UnsafePointer<EdgevoxOnnxOfflineRecognizerResult>) {
    self.result = result
  }

  deinit {
    EdgevoxOnnxDestroyOfflineRecognizerResult(result)
  }
}

class EdgevoxOnnxOfflineRecognizer {
  /// A pointer to the underlying counterpart in C
  private let recognizer: OpaquePointer

  init(
    config: UnsafePointer<EdgevoxOnnxOfflineRecognizerConfig>
  ) {
    guard let ptr = EdgevoxOnnxCreateOfflineRecognizer(config) else {
      fatalError("Failed to create EdgevoxOnnxOfflineRecognizer")
    }
    self.recognizer = ptr
  }

  deinit {
    EdgevoxOnnxDestroyOfflineRecognizer(recognizer)
  }

  /// Decode wave samples.
  ///
  /// - Parameters:
  ///   - samples: Audio samples normalized to the range [-1, 1]
  ///   - sampleRate: Sample rate of the input audio samples. Must match
  ///                 the one expected by the model.
  func decode(samples: [Float], sampleRate: Int = 16_000) -> EdgevoxOnnxOfflineRecongitionResult {
    let stream = createStream()
    stream.acceptWaveform(samples: samples, sampleRate: sampleRate)
    decode(stream: stream)
    return getResult(stream: stream)
  }

  func setConfig(config: UnsafePointer<EdgevoxOnnxOfflineRecognizerConfig>) {
    EdgevoxOnnxOfflineRecognizerSetConfig(recognizer, config)
  }

  func createStream() -> EdgevoxOnnxOfflineStreamWrapper {
    guard let stream = EdgevoxOnnxCreateOfflineStream(recognizer) else {
      fatalError("Failed to create offline stream")
    }

    return EdgevoxOnnxOfflineStreamWrapper(stream: stream)
  }

  func decode(stream: EdgevoxOnnxOfflineStreamWrapper) {
    EdgevoxOnnxDecodeOfflineStream(recognizer, stream.stream)
  }

  func getResult(stream: EdgevoxOnnxOfflineStreamWrapper) -> EdgevoxOnnxOfflineRecongitionResult {
    guard let resultPtr = EdgevoxOnnxGetOfflineStreamResult(stream.stream) else {
      fatalError("Failed to get offline recognition result")
    }

    return EdgevoxOnnxOfflineRecongitionResult(result: resultPtr)
  }
}

class EdgevoxOnnxOfflineStreamWrapper {
  let stream: OpaquePointer

  init(stream: OpaquePointer) {
    self.stream = stream
  }

  deinit {
    EdgevoxOnnxDestroyOfflineStream(stream)
  }

  func setOption(key: String, value: String) {
    EdgevoxOnnxOfflineStreamSetOption(stream, toCPointer(key), toCPointer(value))
  }

  func acceptWaveform(samples: [Float], sampleRate: Int = 16_000) {
    EdgevoxOnnxAcceptWaveformOffline(stream, Int32(sampleRate), samples, Int32(samples.count))
  }
}

func edgevoxOnnxSileroVadModelConfig(
  model: String = "",
  threshold: Float = 0.5,
  minSilenceDuration: Float = 0.25,
  minSpeechDuration: Float = 0.5,
  windowSize: Int = 512,
  maxSpeechDuration: Float = 5.0
) -> EdgevoxOnnxSileroVadModelConfig {
  return EdgevoxOnnxSileroVadModelConfig(
    model: toCPointer(model),
    threshold: threshold,
    min_silence_duration: minSilenceDuration,
    min_speech_duration: minSpeechDuration,
    window_size: Int32(windowSize),
    max_speech_duration: maxSpeechDuration
  )
}

func edgevoxOnnxTenVadModelConfig(
  model: String = "",
  threshold: Float = 0.5,
  minSilenceDuration: Float = 0.25,
  minSpeechDuration: Float = 0.5,
  windowSize: Int = 256,
  maxSpeechDuration: Float = 5.0
) -> EdgevoxOnnxTenVadModelConfig {
  return EdgevoxOnnxTenVadModelConfig(
    model: toCPointer(model),
    threshold: threshold,
    min_silence_duration: minSilenceDuration,
    min_speech_duration: minSpeechDuration,
    window_size: Int32(windowSize),
    max_speech_duration: maxSpeechDuration
  )
}

func edgevoxOnnxVadModelConfig(
  sileroVad: EdgevoxOnnxSileroVadModelConfig = edgevoxOnnxSileroVadModelConfig(),
  sampleRate: Int32 = 16000,
  numThreads: Int = 1,
  provider: String = "cpu",
  debug: Int = 0,
  tenVad: EdgevoxOnnxTenVadModelConfig = edgevoxOnnxTenVadModelConfig()
) -> EdgevoxOnnxVadModelConfig {
  return EdgevoxOnnxVadModelConfig(
    silero_vad: sileroVad,
    sample_rate: sampleRate,
    num_threads: Int32(numThreads),
    provider: toCPointer(provider),
    debug: Int32(debug),
    ten_vad: tenVad
  )
}

class EdgevoxOnnxCircularBufferWrapper {
  private let buffer: OpaquePointer

  init(capacity: Int) {
    guard let ptr = EdgevoxOnnxCreateCircularBuffer(Int32(capacity)) else {
      fatalError("Failed to create EdgevoxOnnxCircularBuffer")
    }
    self.buffer = ptr
  }

  deinit {
    EdgevoxOnnxDestroyCircularBuffer(buffer)
  }

  func push(samples: [Float]) {
    guard !samples.isEmpty else { return }
    EdgevoxOnnxCircularBufferPush(buffer, samples, Int32(samples.count))
  }

  func get(startIndex: Int, n: Int) -> [Float] {
    guard startIndex >= 0 else { return [] }
    guard n > 0 else { return [] }

    guard let ptr = EdgevoxOnnxCircularBufferGet(buffer, Int32(startIndex), Int32(n)) else {
      return []
    }
    defer { EdgevoxOnnxCircularBufferFree(ptr) }

    return Array(UnsafeBufferPointer(start: ptr, count: n))
  }

  func pop(n: Int) {
    guard n > 0 else { return }
    EdgevoxOnnxCircularBufferPop(buffer, Int32(n))
  }

  func size() -> Int {
    return Int(EdgevoxOnnxCircularBufferSize(buffer))
  }

  func reset() {
    EdgevoxOnnxCircularBufferReset(buffer)
  }
}

class EdgevoxOnnxSpeechSegmentWrapper {
  private let p: UnsafePointer<EdgevoxOnnxSpeechSegment>

  init(p: UnsafePointer<EdgevoxOnnxSpeechSegment>) {
    self.p = p
  }

  deinit {
    EdgevoxOnnxDestroySpeechSegment(p)
  }

  var start: Int {
    Int(p.pointee.start)
  }

  var n: Int {
    Int(p.pointee.n)
  }

  lazy var samples: [Float] = {
    Array(UnsafeBufferPointer(start: p.pointee.samples, count: n))
  }()
}

class EdgevoxOnnxVoiceActivityDetectorWrapper {
  /// A pointer to the underlying counterpart in C
  private let vad: OpaquePointer

  init(config: UnsafePointer<EdgevoxOnnxVadModelConfig>, buffer_size_in_seconds: Float) {
    guard let vad = EdgevoxOnnxCreateVoiceActivityDetector(config, buffer_size_in_seconds) else {
      fatalError("EdgevoxOnnxCreateVoiceActivityDetector returned nil")
    }
    self.vad = vad
  }

  deinit {
    EdgevoxOnnxDestroyVoiceActivityDetector(vad)
  }

  func acceptWaveform(samples: [Float]) {
    EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(vad, samples, Int32(samples.count))
  }

  func isEmpty() -> Bool {
    return EdgevoxOnnxVoiceActivityDetectorEmpty(vad) == 1
  }

  func isSpeechDetected() -> Bool {
    return EdgevoxOnnxVoiceActivityDetectorDetected(vad) == 1
  }

  func pop() {
    EdgevoxOnnxVoiceActivityDetectorPop(vad)
  }

  func clear() {
    EdgevoxOnnxVoiceActivityDetectorClear(vad)
  }

  func front() -> EdgevoxOnnxSpeechSegmentWrapper {
    guard let p = EdgevoxOnnxVoiceActivityDetectorFront(vad) else {
      fatalError("EdgevoxOnnxVoiceActivityDetectorFront returned nil")
    }
    return EdgevoxOnnxSpeechSegmentWrapper(p: p)
  }

  func reset() {
    EdgevoxOnnxVoiceActivityDetectorReset(vad)
  }

  func flush() {
    EdgevoxOnnxVoiceActivityDetectorFlush(vad)
  }
}

// offline tts
func edgevoxOnnxOfflineTtsVitsModelConfig(
  model: String = "",
  lexicon: String = "",
  tokens: String = "",
  dataDir: String = "",
  noiseScale: Float = 0.667,
  noiseScaleW: Float = 0.8,
  lengthScale: Float = 1.0,
  dictDir: String = ""
) -> EdgevoxOnnxOfflineTtsVitsModelConfig {
  return EdgevoxOnnxOfflineTtsVitsModelConfig(
    model: toCPointer(model),
    lexicon: toCPointer(lexicon),
    tokens: toCPointer(tokens),
    data_dir: toCPointer(dataDir),
    noise_scale: noiseScale,
    noise_scale_w: noiseScaleW,
    length_scale: lengthScale,
    dict_dir: toCPointer(dictDir)
  )
}

func edgevoxOnnxOfflineTtsMatchaModelConfig(
  acousticModel: String = "",
  vocoder: String = "",
  lexicon: String = "",
  tokens: String = "",
  dataDir: String = "",
  noiseScale: Float = 0.667,
  lengthScale: Float = 1.0,
  dictDir: String = ""
) -> EdgevoxOnnxOfflineTtsMatchaModelConfig {
  return EdgevoxOnnxOfflineTtsMatchaModelConfig(
    acoustic_model: toCPointer(acousticModel),
    vocoder: toCPointer(vocoder),
    lexicon: toCPointer(lexicon),
    tokens: toCPointer(tokens),
    data_dir: toCPointer(dataDir),
    noise_scale: noiseScale,
    length_scale: lengthScale,
    dict_dir: toCPointer(dictDir)
  )
}

func edgevoxOnnxOfflineTtsKokoroModelConfig(
  model: String = "",
  voices: String = "",
  tokens: String = "",
  dataDir: String = "",
  lengthScale: Float = 1.0,
  dictDir: String = "",
  lexicon: String = "",
  lang: String = ""
) -> EdgevoxOnnxOfflineTtsKokoroModelConfig {
  return EdgevoxOnnxOfflineTtsKokoroModelConfig(
    model: toCPointer(model),
    voices: toCPointer(voices),
    tokens: toCPointer(tokens),
    data_dir: toCPointer(dataDir),
    length_scale: lengthScale,
    dict_dir: toCPointer(dictDir),
    lexicon: toCPointer(lexicon),
    lang: toCPointer(lang)
  )
}

func edgevoxOnnxOfflineTtsKittenModelConfig(
  model: String = "",
  voices: String = "",
  tokens: String = "",
  dataDir: String = "",
  lengthScale: Float = 1.0
) -> EdgevoxOnnxOfflineTtsKittenModelConfig {
  return EdgevoxOnnxOfflineTtsKittenModelConfig(
    model: toCPointer(model),
    voices: toCPointer(voices),
    tokens: toCPointer(tokens),
    data_dir: toCPointer(dataDir),
    length_scale: lengthScale
  )
}

func edgevoxOnnxOfflineTtsZipvoiceModelConfig(
  tokens: String = "",
  encoder: String = "",
  decoder: String = "",
  vocoder: String = "",
  dataDir: String = "",
  lexicon: String = "",
  espeakVoice: String = "en-us",
  featScale: Float = 0.1,
  tShift: Float = 0.5,
  targetRms: Float = 0.1,
  guidanceScale: Float = 1.0
) -> EdgevoxOnnxOfflineTtsZipvoiceModelConfig {
  return EdgevoxOnnxOfflineTtsZipvoiceModelConfig(
    tokens: toCPointer(tokens),
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    vocoder: toCPointer(vocoder),
    data_dir: toCPointer(dataDir),
    lexicon: toCPointer(lexicon),
    espeak_voice: toCPointer(espeakVoice),
    feat_scale: featScale,
    t_shift: tShift,
    target_rms: targetRms,
    guidance_scale: guidanceScale
  )
}

func edgevoxOnnxOfflineTtsPocketModelConfig(
  lmFlow: String = "",
  lmMain: String = "",
  encoder: String = "",
  decoder: String = "",
  textConditioner: String = "",
  vocabJson: String = "",
  tokenScoresJson: String = "",
  voiceEmbeddingCacheCapacity: Int = 50
) -> EdgevoxOnnxOfflineTtsPocketModelConfig {
  return EdgevoxOnnxOfflineTtsPocketModelConfig(
    lm_flow: toCPointer(lmFlow),
    lm_main: toCPointer(lmMain),
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    text_conditioner: toCPointer(textConditioner),
    vocab_json: toCPointer(vocabJson),
    token_scores_json: toCPointer(tokenScoresJson),
    voice_embedding_cache_capacity: Int32(voiceEmbeddingCacheCapacity)
  )
}

func edgevoxOnnxOfflineTtsSupertonicModelConfig(
  durationPredictor: String = "",
  textEncoder: String = "",
  vectorEstimator: String = "",
  vocoder: String = "",
  ttsJson: String = "",
  unicodeIndexer: String = "",
  voiceStyle: String = ""
) -> EdgevoxOnnxOfflineTtsSupertonicModelConfig {
  return EdgevoxOnnxOfflineTtsSupertonicModelConfig(
    duration_predictor: toCPointer(durationPredictor),
    text_encoder: toCPointer(textEncoder),
    vector_estimator: toCPointer(vectorEstimator),
    vocoder: toCPointer(vocoder),
    tts_json: toCPointer(ttsJson),
    unicode_indexer: toCPointer(unicodeIndexer),
    voice_style: toCPointer(voiceStyle)
  )
}

func edgevoxOnnxOfflineTtsModelConfig(
  vits: EdgevoxOnnxOfflineTtsVitsModelConfig = edgevoxOnnxOfflineTtsVitsModelConfig(),
  matcha: EdgevoxOnnxOfflineTtsMatchaModelConfig = edgevoxOnnxOfflineTtsMatchaModelConfig(),
  kokoro: EdgevoxOnnxOfflineTtsKokoroModelConfig = edgevoxOnnxOfflineTtsKokoroModelConfig(),
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu",
  kitten: EdgevoxOnnxOfflineTtsKittenModelConfig = edgevoxOnnxOfflineTtsKittenModelConfig(),
  zipvoice: EdgevoxOnnxOfflineTtsZipvoiceModelConfig = edgevoxOnnxOfflineTtsZipvoiceModelConfig(),
  pocket: EdgevoxOnnxOfflineTtsPocketModelConfig = edgevoxOnnxOfflineTtsPocketModelConfig(),
  supertonic: EdgevoxOnnxOfflineTtsSupertonicModelConfig =
    edgevoxOnnxOfflineTtsSupertonicModelConfig()
) -> EdgevoxOnnxOfflineTtsModelConfig {
  return EdgevoxOnnxOfflineTtsModelConfig(
    vits: vits,
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider),
    matcha: matcha,
    kokoro: kokoro,
    kitten: kitten,
    zipvoice: zipvoice,
    pocket: pocket,
    supertonic: supertonic
  )
}

func edgevoxOnnxOfflineTtsConfig(
  model: EdgevoxOnnxOfflineTtsModelConfig,
  ruleFsts: String = "",
  ruleFars: String = "",
  maxNumSentences: Int = 1,
  silenceScale: Float = 0.2
) -> EdgevoxOnnxOfflineTtsConfig {
  return EdgevoxOnnxOfflineTtsConfig(
    model: model,
    rule_fsts: toCPointer(ruleFsts),
    max_num_sentences: Int32(maxNumSentences),
    rule_fars: toCPointer(ruleFars),
    silence_scale: silenceScale
  )
}

class EdgevoxOnnxWaveWrapper {
  let wave: UnsafePointer<EdgevoxOnnxWave>!

  class func readWave(filename: String) -> EdgevoxOnnxWaveWrapper {
    let wave = EdgevoxOnnxReadWave(toCPointer(filename))
    return EdgevoxOnnxWaveWrapper(wave: wave)
  }

  init(wave: UnsafePointer<EdgevoxOnnxWave>!) {
    self.wave = wave
  }

  deinit {
    if let wave {
      EdgevoxOnnxFreeWave(wave)
    }
  }

  var numSamples: Int {
    return Int(wave.pointee.num_samples)
  }

  var sampleRate: Int {
    return Int(wave.pointee.sample_rate)
  }

  var samples: [Float] {
    if numSamples == 0 {
      return []
    } else {
      return [Float](UnsafeBufferPointer(start: wave.pointee.samples, count: numSamples))
    }
  }
}

class EdgevoxOnnxGeneratedAudioWrapper {
  /// A pointer to the underlying counterpart in C
  let audio: UnsafePointer<EdgevoxOnnxGeneratedAudio>!

  init(audio: UnsafePointer<EdgevoxOnnxGeneratedAudio>!) {
    self.audio = audio
  }

  deinit {
    if let audio {
      EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio)
    }
  }

  var n: Int32 {
    return audio.pointee.n
  }

  var sampleRate: Int32 {
    return audio.pointee.sample_rate
  }

  var samples: [Float] {
    if let p = audio.pointee.samples {
      return [Float](UnsafeBufferPointer(start: p, count: Int(n)))
    } else {
      return []
    }
  }

  func save(filename: String) -> Int32 {
    return EdgevoxOnnxWriteWave(audio.pointee.samples, n, sampleRate, toCPointer(filename))
  }
}

typealias TtsCallbackWithArg = (
  @convention(c) (
    UnsafePointer<Float>?,  // const float* samples
    Int32,  // int32_t n
    UnsafeMutableRawPointer?  // void *arg
  ) -> Int32
)?

class EdgevoxOnnxCallbackPair {
  var cb: TtsCallbackWithArg
  var arg: UnsafeMutableRawPointer?
  init(cb: TtsCallbackWithArg, arg: UnsafeMutableRawPointer?) {
    self.cb = cb
    self.arg = arg
  }
}

typealias TtsProgressCallbackWithArg =
  @convention(c) (
    UnsafePointer<Float>?, Int32, Float, UnsafeMutableRawPointer?
  ) -> Int32

struct EdgevoxOnnxGenerationConfigSwift {
  var silenceScale: Float = 0.2
  var speed: Float = 1.0
  var sid: Int = 0
  var referenceAudio: [Float] = []
  var referenceSampleRate: Int = 16000
  var referenceText: String = ""
  var numSteps: Int = 1
  var extra: [String: Any] = [:]  // Any can be String, Int, Float, Double

  /// Convert the extra dictionary into a JSON string
  func extraJsonString() -> String {
    var jsonCompatible: [String: Any] = [:]

    for (key, value) in extra {
      switch value {
      case let v as String:
        jsonCompatible[key] = v
      case let v as Int:
        jsonCompatible[key] = v
      case let v as Float:
        jsonCompatible[key] = v
      case let v as Double:
        jsonCompatible[key] = v
      default:
        // ignore unsupported types
        print("Warning: unsupported type for key '\(key)' in extra")
      }
    }

    guard let data = try? JSONSerialization.data(withJSONObject: jsonCompatible, options: []),
      let json = String(data: data, encoding: .utf8)
    else {
      return "{}"
    }

    return json
  }
}
final class EdgevoxOnnxGenerationConfigC {
  /// The underlying C struct
  var cConfig: EdgevoxOnnxGenerationConfig

  /// Storage for reference audio so the pointer stays valid during the C call
  private let referenceAudioStorage: [Float]

  /// Extra JSON string for C API
  let extraJson: String

  init(_ swiftConfig: EdgevoxOnnxGenerationConfigSwift) {
    let referenceAudio = swiftConfig.referenceAudio

    let extraJson = swiftConfig.extraJsonString()
    self.extraJson = extraJson

    self.referenceAudioStorage = referenceAudio

    self.cConfig = self.referenceAudioStorage.withUnsafeBufferPointer { buffer in
      EdgevoxOnnxGenerationConfig(
        silence_scale: swiftConfig.silenceScale,
        speed: swiftConfig.speed,
        sid: Int32(swiftConfig.sid),
        reference_audio: buffer.count > 0 ? buffer.baseAddress : nil,
        reference_audio_len: Int32(buffer.count),
        reference_sample_rate: Int32(swiftConfig.referenceSampleRate),
        reference_text: toCPointer(swiftConfig.referenceText),
        num_steps: Int32(swiftConfig.numSteps),
        extra: toCPointer(extraJson)
      )
    }
  }
}

class EdgevoxOnnxOfflineTtsWrapper {
  /// A pointer to the underlying counterpart in C
  let tts: OpaquePointer!

  /// Whether the model is a Supertonic TTS model
  let isSupertonic: Bool

  /// The sample rate of the generated audio
  var sampleRate: Int32 {
    return EdgevoxOnnxOfflineTtsSampleRate(tts)
  }

  /// The number of speakers supported by the model
  var numSpeakers: Int32 {
    return EdgevoxOnnxOfflineTtsNumSpeakers(tts)
  }

  /// Constructor taking a model config
  init(
    config: UnsafePointer<EdgevoxOnnxOfflineTtsConfig>!
  ) {
    isSupertonic = config.pointee.model.supertonic.duration_predictor != nil
      && config.pointee.model.supertonic.duration_predictor.pointee != 0
    tts = EdgevoxOnnxCreateOfflineTts(config)
  }

  deinit {
    if let tts {
      EdgevoxOnnxDestroyOfflineTts(tts)
    }
  }

  func generate(text: String, sid: Int = 0, speed: Float = 1.0) -> EdgevoxOnnxGeneratedAudioWrapper {
    let config = EdgevoxOnnxGenerationConfigSwift(speed: speed, sid: sid)
    return generateWithConfig(text: text, config: config, callback: nil, arg: nil)
  }

  func generateWithCallbackWithArg(
    text: String, callback: TtsCallbackWithArg, arg: UnsafeMutableRawPointer, sid: Int = 0,
    speed: Float = 1.0
  ) -> EdgevoxOnnxGeneratedAudioWrapper {
    let config = EdgevoxOnnxGenerationConfigSwift(speed: speed, sid: sid)

    let pair = EdgevoxOnnxCallbackPair(cb: callback, arg: arg)
    let unmanaged = Unmanaged.passRetained(pair)
    let wrapper: TtsProgressCallbackWithArg = { samples, n, progress, rawArg in
      let p = Unmanaged<EdgevoxOnnxCallbackPair>.fromOpaque(rawArg!).takeUnretainedValue()
      return p.cb!(samples, n, p.arg)
    }
    let result = generateWithConfig(
      text: text, config: config, callback: wrapper, arg: unmanaged.toOpaque())
    unmanaged.release()
    return result
  }

  func generateWithConfig(
    text: String,
    config: EdgevoxOnnxGenerationConfigSwift,
    callback: TtsProgressCallbackWithArg?,
    arg: UnsafeMutableRawPointer?
  ) -> EdgevoxOnnxGeneratedAudioWrapper {
    let bridge = EdgevoxOnnxGenerationConfigC(config)

    let audio: UnsafePointer<EdgevoxOnnxGeneratedAudio>? =
      withUnsafePointer(to: &bridge.cConfig) { configPtr in
        EdgevoxOnnxOfflineTtsGenerateWithConfig(
          tts,
          toCPointer(text),
          configPtr,
          callback,
          arg
        )
      }

    return EdgevoxOnnxGeneratedAudioWrapper(audio: audio)
  }

}

// spoken language identification

func edgevoxOnnxSpokenLanguageIdentificationWhisperConfig(
  encoder: String,
  decoder: String,
  tailPaddings: Int = -1
) -> EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig {
  return EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig(
    encoder: toCPointer(encoder),
    decoder: toCPointer(decoder),
    tail_paddings: Int32(tailPaddings))
}

func edgevoxOnnxSpokenLanguageIdentificationConfig(
  whisper: EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig,
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu"
) -> EdgevoxOnnxSpokenLanguageIdentificationConfig {
  return EdgevoxOnnxSpokenLanguageIdentificationConfig(
    whisper: whisper,
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider))
}

class EdgevoxOnnxSpokenLanguageIdentificationResultWrapper {
  /// A pointer to the underlying counterpart in C
  let result: UnsafePointer<EdgevoxOnnxSpokenLanguageIdentificationResult>!

  /// Return the detected language.
  /// en for English
  /// zh for Chinese
  /// es for Spanish
  /// de for German
  /// etc.
  var lang: String {
    return String(cString: result.pointee.lang)
  }

  init(result: UnsafePointer<EdgevoxOnnxSpokenLanguageIdentificationResult>!) {
    self.result = result
  }

  deinit {
    if let result {
      EdgevoxOnnxDestroySpokenLanguageIdentificationResult(result)
    }
  }
}

class EdgevoxOnnxSpokenLanguageIdentificationWrapper {
  /// A pointer to the underlying counterpart in C
  let slid: OpaquePointer!

  init(
    config: UnsafePointer<EdgevoxOnnxSpokenLanguageIdentificationConfig>!
  ) {
    slid = EdgevoxOnnxCreateSpokenLanguageIdentification(config)
  }

  deinit {
    if let slid {
      EdgevoxOnnxDestroySpokenLanguageIdentification(slid)
    }
  }

  func decode(samples: [Float], sampleRate: Int = 16000)
    -> EdgevoxOnnxSpokenLanguageIdentificationResultWrapper
  {
    let stream: OpaquePointer! = EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(slid)
    EdgevoxOnnxAcceptWaveformOffline(stream, Int32(sampleRate), samples, Int32(samples.count))

    let result: UnsafePointer<EdgevoxOnnxSpokenLanguageIdentificationResult>? =
      EdgevoxOnnxSpokenLanguageIdentificationCompute(
        slid,
        stream)

    EdgevoxOnnxDestroyOfflineStream(stream)
    return EdgevoxOnnxSpokenLanguageIdentificationResultWrapper(result: result)
  }
}

// keyword spotting

class EdgevoxOnnxKeywordResultWrapper {
  /// A pointer to the underlying counterpart in C
  let result: UnsafePointer<EdgevoxOnnxKeywordResult>!

  var keyword: String {
    return String(cString: result.pointee.keyword)
  }

  var count: Int32 {
    return result.pointee.count
  }

  var tokens: [String] {
    if let tokensPointer = result.pointee.tokens_arr {
      var tokens: [String] = []
      for index in 0..<count {
        if let tokenPointer = tokensPointer[Int(index)] {
          let token = String(cString: tokenPointer)
          tokens.append(token)
        }
      }
      return tokens
    } else {
      let tokens: [String] = []
      return tokens
    }
  }

  init(result: UnsafePointer<EdgevoxOnnxKeywordResult>!) {
    self.result = result
  }

  deinit {
    if let result {
      EdgevoxOnnxDestroyKeywordResult(result)
    }
  }
}

func edgevoxOnnxKeywordSpotterConfig(
  featConfig: EdgevoxOnnxFeatureConfig,
  modelConfig: EdgevoxOnnxOnlineModelConfig,
  keywordsFile: String,
  maxActivePaths: Int = 4,
  numTrailingBlanks: Int = 1,
  keywordsScore: Float = 1.0,
  keywordsThreshold: Float = 0.25,
  keywordsBuf: String = "",
  keywordsBufSize: Int = 0
) -> EdgevoxOnnxKeywordSpotterConfig {
  return EdgevoxOnnxKeywordSpotterConfig(
    feat_config: featConfig,
    model_config: modelConfig,
    max_active_paths: Int32(maxActivePaths),
    num_trailing_blanks: Int32(numTrailingBlanks),
    keywords_score: keywordsScore,
    keywords_threshold: keywordsThreshold,
    keywords_file: toCPointer(keywordsFile),
    keywords_buf: toCPointer(keywordsBuf),
    keywords_buf_size: Int32(keywordsBufSize)
  )
}

class EdgevoxOnnxKeywordSpotterWrapper {
  /// A pointer to the underlying counterpart in C
  let spotter: OpaquePointer!
  var stream: OpaquePointer!

  init(
    config: UnsafePointer<EdgevoxOnnxKeywordSpotterConfig>!
  ) {
    spotter = EdgevoxOnnxCreateKeywordSpotter(config)
    stream = EdgevoxOnnxCreateKeywordStream(spotter)
  }

  deinit {
    if let stream {
      EdgevoxOnnxDestroyOnlineStream(stream)
    }

    if let spotter {
      EdgevoxOnnxDestroyKeywordSpotter(spotter)
    }
  }

  func acceptWaveform(samples: [Float], sampleRate: Int = 16000) {
    EdgevoxOnnxOnlineStreamAcceptWaveform(stream, Int32(sampleRate), samples, Int32(samples.count))
  }

  func isReady() -> Bool {
    return EdgevoxOnnxIsKeywordStreamReady(spotter, stream) == 1 ? true : false
  }

  func decode() {
    EdgevoxOnnxDecodeKeywordStream(spotter, stream)
  }

  func reset() {
    EdgevoxOnnxResetKeywordStream(spotter, stream)
  }

  func getResult() -> EdgevoxOnnxKeywordResultWrapper {
    let result: UnsafePointer<EdgevoxOnnxKeywordResult>? = EdgevoxOnnxGetKeywordResult(
      spotter, stream)
    return EdgevoxOnnxKeywordResultWrapper(result: result)
  }

  /// Signal that no more audio samples would be available.
  /// After this call, you cannot call acceptWaveform() any more.
  func inputFinished() {
    EdgevoxOnnxOnlineStreamInputFinished(stream)
  }
}

// Punctuation

func edgevoxOnnxOfflinePunctuationModelConfig(
  ctTransformer: String,
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu"
) -> EdgevoxOnnxOfflinePunctuationModelConfig {
  return EdgevoxOnnxOfflinePunctuationModelConfig(
    ct_transformer: toCPointer(ctTransformer),
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider)
  )
}

func edgevoxOnnxOfflinePunctuationConfig(
  model: EdgevoxOnnxOfflinePunctuationModelConfig
) -> EdgevoxOnnxOfflinePunctuationConfig {
  return EdgevoxOnnxOfflinePunctuationConfig(
    model: model
  )
}

class EdgevoxOnnxOfflinePunctuationWrapper {
  /// A pointer to the underlying counterpart in C
  let ptr: OpaquePointer!

  /// Constructor taking a model config
  init(
    config: UnsafePointer<EdgevoxOnnxOfflinePunctuationConfig>!
  ) {
    ptr = EdgevoxOnnxCreateOfflinePunctuation(config)
  }

  deinit {
    if let ptr {
      EdgevoxOnnxDestroyOfflinePunctuation(ptr)
    }
  }

  func addPunct(text: String) -> String {
    let cText = SherpaOfflinePunctuationAddPunct(ptr, toCPointer(text))
    let ans = String(cString: cText!)
    SherpaOfflinePunctuationFreeText(cText)
    return ans
  }
}

func edgevoxOnnxOnlinePunctuationModelConfig(
  cnnBiLstm: String,
  bpeVocab: String,
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu"
) -> EdgevoxOnnxOnlinePunctuationModelConfig {
  return EdgevoxOnnxOnlinePunctuationModelConfig(
    cnn_bilstm: toCPointer(cnnBiLstm),
    bpe_vocab: toCPointer(bpeVocab),
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider))
}

func edgevoxOnnxOnlinePunctuationConfig(
  model: EdgevoxOnnxOnlinePunctuationModelConfig
) -> EdgevoxOnnxOnlinePunctuationConfig {
  return EdgevoxOnnxOnlinePunctuationConfig(model: model)
}

class EdgevoxOnnxOnlinePunctuationWrapper {
  /// A pointer to the underlying counterpart in C
  let ptr: OpaquePointer!

  /// Constructor taking a model config
  init(
    config: UnsafePointer<EdgevoxOnnxOnlinePunctuationConfig>!
  ) {
    ptr = EdgevoxOnnxCreateOnlinePunctuation(config)
  }

  deinit {
    if let ptr {
      EdgevoxOnnxDestroyOnlinePunctuation(ptr)
    }
  }

  func addPunct(text: String) -> String {
    let cText = EdgevoxOnnxOnlinePunctuationAddPunct(ptr, toCPointer(text))
    let ans = String(cString: cText!)
    EdgevoxOnnxOnlinePunctuationFreeText(cText)
    return ans
  }
}

func edgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig(model: String)
  -> EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig
{
  return EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig(model: toCPointer(model))
}

func edgevoxOnnxOfflineSpeakerSegmentationModelConfig(
  pyannote: EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig,
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu"
) -> EdgevoxOnnxOfflineSpeakerSegmentationModelConfig {
  return EdgevoxOnnxOfflineSpeakerSegmentationModelConfig(
    pyannote: pyannote,
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider)
  )
}

func edgevoxOnnxFastClusteringConfig(numClusters: Int = -1, threshold: Float = 0.5)
  -> EdgevoxOnnxFastClusteringConfig
{
  return EdgevoxOnnxFastClusteringConfig(num_clusters: Int32(numClusters), threshold: threshold)
}

func edgevoxOnnxSpeakerEmbeddingExtractorConfig(
  model: String,
  numThreads: Int = 1,
  debug: Int = 0,
  provider: String = "cpu"
) -> EdgevoxOnnxSpeakerEmbeddingExtractorConfig {
  return EdgevoxOnnxSpeakerEmbeddingExtractorConfig(
    model: toCPointer(model),
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider)
  )
}

func edgevoxOnnxOfflineSpeakerDiarizationConfig(
  segmentation: EdgevoxOnnxOfflineSpeakerSegmentationModelConfig,
  embedding: EdgevoxOnnxSpeakerEmbeddingExtractorConfig,
  clustering: EdgevoxOnnxFastClusteringConfig,
  minDurationOn: Float = 0.3,
  minDurationOff: Float = 0.5
) -> EdgevoxOnnxOfflineSpeakerDiarizationConfig {
  return EdgevoxOnnxOfflineSpeakerDiarizationConfig(
    segmentation: segmentation,
    embedding: embedding,
    clustering: clustering,
    min_duration_on: minDurationOn,
    min_duration_off: minDurationOff
  )
}

struct EdgevoxOnnxOfflineSpeakerDiarizationSegmentWrapper {
  var start: Float = 0
  var end: Float = 0
  var speaker: Int = 0
}

class EdgevoxOnnxOfflineSpeakerDiarizationWrapper {
  /// A pointer to the underlying counterpart in C
  let impl: OpaquePointer!

  init(
    config: UnsafePointer<EdgevoxOnnxOfflineSpeakerDiarizationConfig>!
  ) {
    impl = EdgevoxOnnxCreateOfflineSpeakerDiarization(config)
  }

  deinit {
    if let impl {
      EdgevoxOnnxDestroyOfflineSpeakerDiarization(impl)
    }
  }

  var sampleRate: Int {
    return Int(EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(impl))
  }

  // only config.clustering is used. All other fields are ignored
  func setConfig(config: UnsafePointer<EdgevoxOnnxOfflineSpeakerDiarizationConfig>!) {
    EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(impl, config)
  }

  func process(samples: [Float]) -> [EdgevoxOnnxOfflineSpeakerDiarizationSegmentWrapper] {
    let result = EdgevoxOnnxOfflineSpeakerDiarizationProcess(
      impl, samples, Int32(samples.count))

    if result == nil {
      return []
    }

    let numSegments = Int(EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(result))

    let p: UnsafePointer<EdgevoxOnnxOfflineSpeakerDiarizationSegment>? =
      EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(result)

    if p == nil {
      return []
    }

    var ans: [EdgevoxOnnxOfflineSpeakerDiarizationSegmentWrapper] = []
    for i in 0..<numSegments {
      ans.append(
        EdgevoxOnnxOfflineSpeakerDiarizationSegmentWrapper(
          start: p![i].start, end: p![i].end, speaker: Int(p![i].speaker)))
    }

    EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(p)
    EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(result)

    return ans
  }
}

class EdgevoxOnnxOnlineStreamWrapper {
  /// A pointer to the underlying counterpart in C
  let impl: OpaquePointer!
  init(impl: OpaquePointer!) {
    self.impl = impl
  }

  deinit {
    if let impl {
      EdgevoxOnnxDestroyOnlineStream(impl)
    }
  }

  func setOption(key: String, value: String) {
    EdgevoxOnnxOnlineStreamSetOption(impl, toCPointer(key), toCPointer(value))
  }

  func acceptWaveform(samples: [Float], sampleRate: Int = 16000) {
    EdgevoxOnnxOnlineStreamAcceptWaveform(impl, Int32(sampleRate), samples, Int32(samples.count))
  }

  func inputFinished() {
    EdgevoxOnnxOnlineStreamInputFinished(impl)
  }
}

class EdgevoxOnnxSpeakerEmbeddingExtractorWrapper {
  /// A pointer to the underlying counterpart in C
  let impl: OpaquePointer!

  init(
    config: UnsafePointer<EdgevoxOnnxSpeakerEmbeddingExtractorConfig>!
  ) {
    impl = EdgevoxOnnxCreateSpeakerEmbeddingExtractor(config)
  }

  deinit {
    if let impl {
      EdgevoxOnnxDestroySpeakerEmbeddingExtractor(impl)
    }
  }

  var dim: Int {
    return Int(EdgevoxOnnxSpeakerEmbeddingExtractorDim(impl))
  }

  func createStream() -> EdgevoxOnnxOnlineStreamWrapper {
    let newStream = EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(impl)
    return EdgevoxOnnxOnlineStreamWrapper(impl: newStream)
  }

  func isReady(stream: EdgevoxOnnxOnlineStreamWrapper) -> Bool {
    return EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(impl, stream.impl) == 1 ? true : false
  }

  func compute(stream: EdgevoxOnnxOnlineStreamWrapper) -> [Float] {
    if !isReady(stream: stream) {
      return []
    }

    let p = EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(impl, stream.impl)

    defer {
      EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(p)
    }

    return [Float](UnsafeBufferPointer(start: p, count: dim))
  }
}

func edgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig(model: String = "")
  -> EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig
{
  return EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig(model: toCPointer(model))
}

func edgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig(model: String = "")
  -> EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig
{
  return EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig(model: toCPointer(model))
}

func edgevoxOnnxOfflineSpeechDenoiserModelConfig(
  gtcrn: EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig =
    edgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig(),
  dpdfnet: EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig =
    edgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig(),
  numThreads: Int = 1,
  provider: String = "cpu",
  debug: Int = 0
) -> EdgevoxOnnxOfflineSpeechDenoiserModelConfig {
  return EdgevoxOnnxOfflineSpeechDenoiserModelConfig(
    gtcrn: gtcrn,
    num_threads: Int32(numThreads),
    debug: Int32(debug),
    provider: toCPointer(provider),
    dpdfnet: dpdfnet
  )
}

func edgevoxOnnxOfflineSpeechDenoiserConfig(
  model: EdgevoxOnnxOfflineSpeechDenoiserModelConfig =
    edgevoxOnnxOfflineSpeechDenoiserModelConfig()
) -> EdgevoxOnnxOfflineSpeechDenoiserConfig {
  return EdgevoxOnnxOfflineSpeechDenoiserConfig(
    model: model)
}

class EdgevoxOnnxDenoisedAudioWrapper {
  /// A pointer to the underlying counterpart in C
  let audio: UnsafePointer<EdgevoxOnnxDenoisedAudio>!

  init(audio: UnsafePointer<EdgevoxOnnxDenoisedAudio>!) {
    self.audio = audio
  }

  deinit {
    if let audio {
      EdgevoxOnnxDestroyDenoisedAudio(audio)
    }
  }

  var n: Int32 {
    guard let audio else {
      return 0
    }
    return audio.pointee.n
  }

  var sampleRate: Int32 {
    guard let audio else {
      return 0
    }
    return audio.pointee.sample_rate
  }

  var samples: [Float] {
    guard let audio else {
      return []
    }

    if let p = audio.pointee.samples {
      var samples: [Float] = []
      for index in 0..<n {
        samples.append(p[Int(index)])
      }
      return samples
    } else {
      let samples: [Float] = []
      return samples
    }
  }

  func save(filename: String) -> Int32 {
    guard let audio else {
      return 0
    }
    return EdgevoxOnnxWriteWave(audio.pointee.samples, n, sampleRate, toCPointer(filename))
  }
}

class EdgevoxOnnxOfflineSpeechDenoiserWrapper {
  /// A pointer to the underlying counterpart in C
  let impl: OpaquePointer!

  /// Constructor taking a model config
  init(
    config: UnsafePointer<EdgevoxOnnxOfflineSpeechDenoiserConfig>!
  ) {
    impl = EdgevoxOnnxCreateOfflineSpeechDenoiser(config)
  }

  deinit {
    if let impl {
      EdgevoxOnnxDestroyOfflineSpeechDenoiser(impl)
    }
  }

  func run(samples: [Float], sampleRate: Int) -> EdgevoxOnnxDenoisedAudioWrapper {
    let audio: UnsafePointer<EdgevoxOnnxDenoisedAudio>? = EdgevoxOnnxOfflineSpeechDenoiserRun(
      impl, samples, Int32(samples.count), Int32(sampleRate))

    return EdgevoxOnnxDenoisedAudioWrapper(audio: audio)
  }

  var sampleRate: Int {
    return Int(EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(impl))
  }
}

func edgevoxOnnxOnlineSpeechDenoiserConfig(
  model: EdgevoxOnnxOfflineSpeechDenoiserModelConfig =
    edgevoxOnnxOfflineSpeechDenoiserModelConfig()
) -> EdgevoxOnnxOnlineSpeechDenoiserConfig {
  return EdgevoxOnnxOnlineSpeechDenoiserConfig(model: model)
}

class EdgevoxOnnxOnlineSpeechDenoiserWrapper {
  let impl: OpaquePointer!

  init(
    config: UnsafePointer<EdgevoxOnnxOnlineSpeechDenoiserConfig>!
  ) {
    impl = EdgevoxOnnxCreateOnlineSpeechDenoiser(config)
  }

  deinit {
    if let impl {
      EdgevoxOnnxDestroyOnlineSpeechDenoiser(impl)
    }
  }

  func run(samples: [Float], sampleRate: Int) -> EdgevoxOnnxDenoisedAudioWrapper {
    let audio: UnsafePointer<EdgevoxOnnxDenoisedAudio>? = EdgevoxOnnxOnlineSpeechDenoiserRun(
      impl, samples, Int32(samples.count), Int32(sampleRate))
    return EdgevoxOnnxDenoisedAudioWrapper(audio: audio)
  }

  func flush() -> EdgevoxOnnxDenoisedAudioWrapper {
    let audio: UnsafePointer<EdgevoxOnnxDenoisedAudio>? = EdgevoxOnnxOnlineSpeechDenoiserFlush(impl)
    return EdgevoxOnnxDenoisedAudioWrapper(audio: audio)
  }

  func reset() {
    EdgevoxOnnxOnlineSpeechDenoiserReset(impl)
  }

  var sampleRate: Int {
    return Int(EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(impl))
  }

  var frameShiftInSamples: Int {
    return Int(EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(impl))
  }
}

func getEdgevoxOnnxVersion() -> String {
  return String(cString: EdgevoxOnnxGetVersionStr())
}

func getEdgevoxOnnxGitSha1() -> String {
  return String(cString: EdgevoxOnnxGetGitSha1())
}

func getEdgevoxOnnxGitDate() -> String {
  return String(cString: EdgevoxOnnxGetGitDate())
}
//---------------------------
// Source separation
//---------------------------

struct AudioData {
  private enum Storage {
    case owned([Float])
    case wrapped(ManagedWave)
  }

  private class ManagedWave {
    let pointer: UnsafePointer<EdgevoxOnnxMultiChannelWave>
    init(_ p: UnsafePointer<EdgevoxOnnxMultiChannelWave>) { self.pointer = p }
    deinit { EdgevoxOnnxFreeMultiChannelWave(pointer) }
  }

  private let storage: Storage
  let channelCount: Int
  let samplesPerChannel: Int
  let sampleRate: Int

  init(samples: [Float], channelCount: Int, sampleRate: Int) {
    self.storage = .owned(samples)
    self.channelCount = channelCount
    self.sampleRate = sampleRate
    self.samplesPerChannel = channelCount > 0 ? samples.count / channelCount : 0
  }

  init?(filename: String) {
    guard let ptr = EdgevoxOnnxReadWaveMultiChannel(filename) else { return nil }
    self.storage = .wrapped(ManagedWave(ptr))
    self.channelCount = Int(ptr.pointee.num_channels)
    self.samplesPerChannel = Int(ptr.pointee.num_samples)
    self.sampleRate = Int(ptr.pointee.sample_rate)
  }

  func withUnsafeBufferPointer<R>(_ body: (UnsafeBufferPointer<Float>) -> R) -> R {
    switch storage {
    case .owned(let array):
      return array.withUnsafeBufferPointer(body)
    case .wrapped(let managed):
      let total = Int(managed.pointer.pointee.num_channels * managed.pointer.pointee.num_samples)
      // Ensure we start from the first channel's pointer
      return body(UnsafeBufferPointer(start: managed.pointer.pointee.samples[0], count: total))
    }
  }

  @discardableResult
  func save(to filename: String) -> Bool {
    return withUnsafeBufferPointer { buf in
      guard let base = buf.baseAddress else { return false }
      // FIX: Explicitly type the array as Optional pointers to match C 'float* const*'
      var ptrs: [UnsafePointer<Float>?] = (0..<channelCount).map { base + ($0 * samplesPerChannel) }

      return EdgevoxOnnxWriteWaveMultiChannel(
        &ptrs,
        Int32(samplesPerChannel),
        Int32(sampleRate),
        Int32(channelCount),
        filename
      ) == 1
    }
  }
}

struct SourceSeparationConfig {
  struct Spleeter {
    var vocals: String
    var accompaniment: String
  }
  struct Uvr { var model: String }

  var spleeter: Spleeter?
  var uvr: Uvr?
  var numThreads: Int = 1
  var debug: Bool = false
  var provider: String = "cpu"

  func withCConfig<R>(_ body: (UnsafePointer<EdgevoxOnnxOfflineSourceSeparationConfig>) -> R) -> R {
    var cConfig = EdgevoxOnnxOfflineSourceSeparationConfig()
    cConfig.model.num_threads = Int32(self.numThreads)
    cConfig.model.debug = self.debug ? 1 : 0

    var s: [String: [Int8]] = [:]
    func b(_ k: String, _ v: String?) -> UnsafePointer<Int8>? {
      guard let v = v else { return nil }
      s[k] = Array(v.utf8CString)
      return s[k]!.withUnsafeBufferPointer { $0.baseAddress }
    }

    cConfig.model.provider = b("provider", self.provider)
    cConfig.model.spleeter.vocals = b("spleeter.vocals", self.spleeter?.vocals)
    cConfig.model.spleeter.accompaniment = b("spleeter.accompaniment", self.spleeter?.accompaniment)
    cConfig.model.uvr.model = b("uvr.model", self.uvr?.model)

    return body(&cConfig)
  }
}

class SourceSeparator {
  private var engine: OpaquePointer?

  init?(config: SourceSeparationConfig) {
    self.engine = config.withCConfig { EdgevoxOnnxCreateOfflineSourceSeparation($0) }

    if self.engine == nil { return nil }
  }

  deinit {
    if let e = engine {
      EdgevoxOnnxDestroyOfflineSourceSeparation(e)
    }
  }

  func process(buffer: AudioData) -> [AudioData]? {
    guard let engine = engine else { return nil }

    return buffer.withUnsafeBufferPointer { flatBuf in
      guard let base = flatBuf.baseAddress else { return nil }
      var ptrs: [UnsafePointer<Float>?] = (0..<buffer.channelCount).map {
        base + ($0 * buffer.samplesPerChannel)
      }

      guard
        let raw = EdgevoxOnnxOfflineSourceSeparationProcess(
          engine,
          &ptrs,
          Int32(buffer.channelCount),
          Int32(buffer.samplesPerChannel),
          Int32(buffer.sampleRate)
        )
      else { return nil }

      let stemCount = Int(raw.pointee.num_stems)
      let result = (0..<stemCount).map { i in
        let stem = raw.pointee.stems[i]
        let chs = Int(stem.num_channels)
        let n = Int(stem.n)
        var flat = [Float](repeating: 0, count: chs * n)

        for c in 0..<chs {
          if let src = stem.samples[c] {
            let offset = c * n
            flat.withUnsafeMutableBufferPointer { dest in
              let destPtr = dest.baseAddress!.advanced(by: offset)
              destPtr.initialize(from: src, count: n)
            }
          }
        }
        return AudioData(
          samples: flat, channelCount: chs, sampleRate: Int(raw.pointee.sample_rate))
      }

      EdgevoxOnnxDestroySourceSeparationOutput(raw)
      return result
    }
  }
}
