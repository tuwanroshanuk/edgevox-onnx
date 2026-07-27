// Copyright (c)  2024  Xiaomi Corporation
import 'dart:ffi';
import 'package:ffi/ffi.dart';

final class EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineSpeechDenoiserModelConfig extends Struct {
  external EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig gtcrn;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;

  external EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig dpdfnet;
}

final class EdgevoxOnnxOfflineSpeechDenoiserConfig extends Struct {
  external EdgevoxOnnxOfflineSpeechDenoiserModelConfig model;
}

final class EdgevoxOnnxOnlineSpeechDenoiserConfig extends Struct {
  external EdgevoxOnnxOfflineSpeechDenoiserModelConfig model;
}

final class EdgevoxOnnxDenoisedAudio extends Struct {
  external Pointer<Float> samples;

  @Int32()
  external int n;

  @Int32()
  external int sampleRate;
}

final class EdgevoxOnnxSpeakerEmbeddingExtractorConfig extends Struct {
  external Pointer<Utf8> model;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxOfflineSpeakerDiarizationSegment extends Struct {
  @Float()
  external double start;

  @Float()
  external double end;

  @Int32()
  external int speaker;
}

final class EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig
    extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineSpeakerSegmentationModelConfig extends Struct {
  external EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig pyannote;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxFastClusteringConfig extends Struct {
  @Int32()
  external int numClusters;

  @Float()
  external double threshold;
}

final class EdgevoxOnnxOfflineSpeakerDiarizationConfig extends Struct {
  external EdgevoxOnnxOfflineSpeakerSegmentationModelConfig segmentation;
  external EdgevoxOnnxSpeakerEmbeddingExtractorConfig embedding;
  external EdgevoxOnnxFastClusteringConfig clustering;

  @Float()
  external double minDurationOn;

  @Float()
  external double minDurationOff;
}

final class EdgevoxOnnxOfflinePunctuationModelConfig extends Struct {
  external Pointer<Utf8> ctTransformer;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxOfflinePunctuationConfig extends Struct {
  external EdgevoxOnnxOfflinePunctuationModelConfig model;
}

final class EdgevoxOnnxOnlinePunctuationModelConfig extends Struct {
  external Pointer<Utf8> cnnBiLstm;
  external Pointer<Utf8> bpeVocab;
  @Int32()
  external int numThreads;
  @Int32()
  external int debug;
  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxOnlinePunctuationConfig extends Struct {
  external EdgevoxOnnxOnlinePunctuationModelConfig model;
}

final class EdgevoxOnnxOfflineZipformerAudioTaggingModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxAudioTaggingModelConfig extends Struct {
  external EdgevoxOnnxOfflineZipformerAudioTaggingModelConfig zipformer;
  external Pointer<Utf8> ced;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxAudioTaggingConfig extends Struct {
  external EdgevoxOnnxAudioTaggingModelConfig model;
  external Pointer<Utf8> labels;

  @Int32()
  external int topK;
}

final class EdgevoxOnnxAudioEvent extends Struct {
  external Pointer<Utf8> name;

  @Int32()
  external int index;

  @Float()
  external double prob;
}

final class EdgevoxOnnxOfflineTtsVitsModelConfig extends Struct {
  external Pointer<Utf8> model;
  external Pointer<Utf8> lexicon;
  external Pointer<Utf8> tokens;
  external Pointer<Utf8> dataDir;

  @Float()
  external double noiseScale;

  @Float()
  external double noiseScaleW;

  @Float()
  external double lengthScale;

  external Pointer<Utf8> dictDir;
}

final class EdgevoxOnnxOfflineTtsMatchaModelConfig extends Struct {
  external Pointer<Utf8> acousticModel;
  external Pointer<Utf8> vocoder;
  external Pointer<Utf8> lexicon;
  external Pointer<Utf8> tokens;
  external Pointer<Utf8> dataDir;

  @Float()
  external double noiseScale;

  @Float()
  external double lengthScale;

  external Pointer<Utf8> dictDir;
}

final class EdgevoxOnnxOfflineTtsKokoroModelConfig extends Struct {
  external Pointer<Utf8> model;
  external Pointer<Utf8> voices;
  external Pointer<Utf8> tokens;
  external Pointer<Utf8> dataDir;

  @Float()
  external double lengthScale;
  external Pointer<Utf8> dictDir;
  external Pointer<Utf8> lexicon;
  external Pointer<Utf8> lang;
}

final class EdgevoxOnnxOfflineTtsKittenModelConfig extends Struct {
  external Pointer<Utf8> model;
  external Pointer<Utf8> voices;
  external Pointer<Utf8> tokens;
  external Pointer<Utf8> dataDir;

  @Float()
  external double lengthScale;
}

final class EdgevoxOnnxOfflineTtsZipVoiceModelConfig extends Struct {
  external Pointer<Utf8> tokens;
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> vocoder;
  external Pointer<Utf8> dataDir;
  external Pointer<Utf8> lexicon;
  external Pointer<Utf8> espeakVoice;

  @Float()
  external double featScale;

  @Float()
  external double tShift;

  @Float()
  external double targetRms;

  @Float()
  external double guidanceScale;
}

final class EdgevoxOnnxOfflineTtsPocketModelConfig extends Struct {
  external Pointer<Utf8> lmFlow;
  external Pointer<Utf8> lmMain;
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> textConditioner;
  external Pointer<Utf8> vocabJson;
  external Pointer<Utf8> tokenScoresJson;

  @Int32()
  external int voiceEmbeddingCacheCapacity;
}

final class EdgevoxOnnxOfflineTtsSupertonicModelConfig extends Struct {
  external Pointer<Utf8> durationPredictor;
  external Pointer<Utf8> textEncoder;
  external Pointer<Utf8> vectorEstimator;
  external Pointer<Utf8> vocoder;
  external Pointer<Utf8> ttsJson;
  external Pointer<Utf8> unicodeIndexer;
  external Pointer<Utf8> voiceStyle;
}

final class EdgevoxOnnxOfflineTtsModelConfig extends Struct {
  external EdgevoxOnnxOfflineTtsVitsModelConfig vits;
  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
  external EdgevoxOnnxOfflineTtsMatchaModelConfig matcha;
  external EdgevoxOnnxOfflineTtsKokoroModelConfig kokoro;
  external EdgevoxOnnxOfflineTtsKittenModelConfig kitten;
  external EdgevoxOnnxOfflineTtsZipVoiceModelConfig zipvoice;
  external EdgevoxOnnxOfflineTtsPocketModelConfig pocket;
  external EdgevoxOnnxOfflineTtsSupertonicModelConfig supertonic;
}

final class EdgevoxOnnxOfflineTtsConfig extends Struct {
  external EdgevoxOnnxOfflineTtsModelConfig model;
  external Pointer<Utf8> ruleFsts;

  @Int32()
  external int maxNumSenetences;

  external Pointer<Utf8> ruleFars;

  @Float()
  external double silenceScale;
}

final class EdgevoxOnnxGenerationConfig extends Struct {
  @Float()
  external double silenceScale;

  @Float()
  external double speed;

  @Int32()
  external int sid;

  external Pointer<Float> referenceAudio;

  @Int32()
  external int referenceAudioLength;

  @Int32()
  external int referenceSampleRate;

  external Pointer<Utf8> referenceText;

  @Int32()
  external int numSteps;

  external Pointer<Utf8> extra;
}

final class EdgevoxOnnxGeneratedAudio extends Struct {
  external Pointer<Float> samples;

  @Int32()
  external int n;

  @Int32()
  external int sampleRate;
}

final class EdgevoxOnnxFeatureConfig extends Struct {
  @Int32()
  external int sampleRate;

  @Int32()
  external int featureDim;
}

final class EdgevoxOnnxOfflineTransducerModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> joiner;
}

final class EdgevoxOnnxOfflineParaformerModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineNemoEncDecCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineDolphinModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineZipformerCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineWenetCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineMedAsrCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineFireRedAsrCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineFunAsrNanoModelConfig extends Struct {
  external Pointer<Utf8> encoderAdaptor;
  external Pointer<Utf8> llm;
  external Pointer<Utf8> embedding;
  external Pointer<Utf8> tokenizer;
  external Pointer<Utf8> systemPrompt;
  external Pointer<Utf8> userPrompt;

  @Int32()
  external int maxNewTokens;

  @Float()
  external double temperature;

  @Float()
  external double topP;

  @Int32()
  external int seed;

  external Pointer<Utf8> language;

  @Int32()
  external int itn;

  external Pointer<Utf8> hotwords;
}

final class EdgevoxOnnxOfflineQwen3AsrModelConfig extends Struct {
  external Pointer<Utf8> convFrontend;
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> tokenizer;

  @Int32()
  external int maxTotalLen;

  @Int32()
  external int maxNewTokens;

  @Float()
  external double temperature;

  @Float()
  external double topP;

  @Int32()
  external int seed;

  external Pointer<Utf8> hotwords;
}

final class EdgevoxOnnxOfflineWhisperModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> language;
  external Pointer<Utf8> task;

  @Int32()
  external int tailPaddings;

  @Int32()
  external int enableTokenTimestamps;

  @Int32()
  external int enableSegmentTimestamps;
}

final class EdgevoxOnnxOfflineCanaryModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> srcLang;
  external Pointer<Utf8> tgtLang;

  @Int32()
  external int usePnc;
}

final class EdgevoxOnnxOfflineCohereTranscribeModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> language;

  @Int32()
  external int usePunct;

  @Int32()
  external int useItn;
}

final class EdgevoxOnnxOfflineMoonshineModelConfig extends Struct {
  external Pointer<Utf8> preprocessor;
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> uncachedDecoder;
  external Pointer<Utf8> cachedDecoder;
  external Pointer<Utf8> mergedDecoder;
}

final class EdgevoxOnnxOfflineFireRedAsrModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
}

final class EdgevoxOnnxOfflineTdnnModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOfflineSenseVoiceModelConfig extends Struct {
  external Pointer<Utf8> model;
  external Pointer<Utf8> language;

  @Int32()
  external int useInverseTextNormalization;
}

final class EdgevoxOnnxOfflineLMConfig extends Struct {
  external Pointer<Utf8> model;

  @Float()
  external double scale;
}

final class EdgevoxOnnxOfflineModelConfig extends Struct {
  external EdgevoxOnnxOfflineTransducerModelConfig transducer;
  external EdgevoxOnnxOfflineParaformerModelConfig paraformer;
  external EdgevoxOnnxOfflineNemoEncDecCtcModelConfig nemoCtc;
  external EdgevoxOnnxOfflineWhisperModelConfig whisper;
  external EdgevoxOnnxOfflineTdnnModelConfig tdnn;

  external Pointer<Utf8> tokens;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;

  external Pointer<Utf8> modelType;
  external Pointer<Utf8> modelingUnit;
  external Pointer<Utf8> bpeVocab;
  external Pointer<Utf8> telespeechCtc;

  external EdgevoxOnnxOfflineSenseVoiceModelConfig senseVoice;
  external EdgevoxOnnxOfflineMoonshineModelConfig moonshine;
  external EdgevoxOnnxOfflineFireRedAsrModelConfig fireRedAsr;
  external EdgevoxOnnxOfflineDolphinModelConfig dolphin;
  external EdgevoxOnnxOfflineZipformerCtcModelConfig zipformerCtc;
  external EdgevoxOnnxOfflineCanaryModelConfig canary;
  external EdgevoxOnnxOfflineWenetCtcModelConfig wenetCtc;
  external EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig omnilingual;
  external EdgevoxOnnxOfflineMedAsrCtcModelConfig medasr;
  external EdgevoxOnnxOfflineFunAsrNanoModelConfig funasrNano;
  external EdgevoxOnnxOfflineFireRedAsrCtcModelConfig fireRedAsrCtc;
  external EdgevoxOnnxOfflineQwen3AsrModelConfig qwen3Asr;
  external EdgevoxOnnxOfflineCohereTranscribeModelConfig cohereTranscribe;
}

final class EdgevoxOnnxOfflineRecognizerConfig extends Struct {
  external EdgevoxOnnxFeatureConfig feat;
  external EdgevoxOnnxOfflineModelConfig model;
  external EdgevoxOnnxOfflineLMConfig lm;
  external Pointer<Utf8> decodingMethod;

  @Int32()
  external int maxActivePaths;

  external Pointer<Utf8> hotwordsFile;

  @Float()
  external double hotwordsScore;

  external Pointer<Utf8> ruleFsts;
  external Pointer<Utf8> ruleFars;

  @Float()
  external double blankPenalty;
  external EdgevoxOnnxHomophoneReplacerConfig hr;
}

final class EdgevoxOnnxOnlineTransducerModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
  external Pointer<Utf8> joiner;
}

final class EdgevoxOnnxOnlineParaformerModelConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;
}

final class EdgevoxOnnxOnlineZipformer2CtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOnlineNemoCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOnlineToneCtcModelConfig extends Struct {
  external Pointer<Utf8> model;
}

final class EdgevoxOnnxOnlineModelConfig extends Struct {
  external EdgevoxOnnxOnlineTransducerModelConfig transducer;
  external EdgevoxOnnxOnlineParaformerModelConfig paraformer;
  external EdgevoxOnnxOnlineZipformer2CtcModelConfig zipformer2Ctc;

  external Pointer<Utf8> tokens;

  @Int32()
  external int numThreads;

  external Pointer<Utf8> provider;

  @Int32()
  external int debug;

  external Pointer<Utf8> modelType;

  external Pointer<Utf8> modelingUnit;

  external Pointer<Utf8> bpeVocab;

  external Pointer<Utf8> tokensBuf;

  @Int32()
  external int tokensBufSize;

  external EdgevoxOnnxOnlineNemoCtcModelConfig nemoCtc;

  external EdgevoxOnnxOnlineToneCtcModelConfig toneCtc;
}

final class EdgevoxOnnxOnlineCtcFstDecoderConfig extends Struct {
  external Pointer<Utf8> graph;

  @Int32()
  external int maxActive;
}

final class EdgevoxOnnxHomophoneReplacerConfig extends Struct {
  external Pointer<Utf8> dictDir;
  external Pointer<Utf8> lexicon;
  external Pointer<Utf8> ruleFsts;
}

final class EdgevoxOnnxOnlineRecognizerConfig extends Struct {
  external EdgevoxOnnxFeatureConfig feat;
  external EdgevoxOnnxOnlineModelConfig model;
  external Pointer<Utf8> decodingMethod;

  @Int32()
  external int maxActivePaths;

  @Int32()
  external int enableEndpoint;

  @Float()
  external double rule1MinTrailingSilence;

  @Float()
  external double rule2MinTrailingSilence;

  @Float()
  external double rule3MinUtteranceLength;

  external Pointer<Utf8> hotwordsFile;

  @Float()
  external double hotwordsScore;

  external EdgevoxOnnxOnlineCtcFstDecoderConfig ctcFstDecoderConfig;

  external Pointer<Utf8> ruleFsts;
  external Pointer<Utf8> ruleFars;

  @Float()
  external double blankPenalty;

  external Pointer<Utf8> hotwordsBuf;

  @Int32()
  external int hotwordsBufSize;
  external EdgevoxOnnxHomophoneReplacerConfig hr;
}

final class EdgevoxOnnxSileroVadModelConfig extends Struct {
  external Pointer<Utf8> model;

  @Float()
  external double threshold;

  @Float()
  external double minSilenceDuration;

  @Float()
  external double minSpeechDuration;

  @Int32()
  external int windowSize;

  @Float()
  external double maxSpeechDuration;
}

final class EdgevoxOnnxTenVadModelConfig extends Struct {
  external Pointer<Utf8> model;

  @Float()
  external double threshold;

  @Float()
  external double minSilenceDuration;

  @Float()
  external double minSpeechDuration;

  @Int32()
  external int windowSize;

  @Float()
  external double maxSpeechDuration;
}

final class EdgevoxOnnxVadModelConfig extends Struct {
  external EdgevoxOnnxSileroVadModelConfig sileroVad;

  @Int32()
  external int sampleRate;

  @Int32()
  external int numThreads;

  external Pointer<Utf8> provider;

  @Int32()
  external int debug;

  external EdgevoxOnnxTenVadModelConfig tenVad;
}

final class EdgevoxOnnxSpeechSegment extends Struct {
  @Int32()
  external int start;

  external Pointer<Float> samples;

  @Int32()
  external int n;
}

final class EdgevoxOnnxWave extends Struct {
  external Pointer<Float> samples;

  @Int32()
  external int sampleRate;

  @Int32()
  external int numSamples;
}

final class EdgevoxOnnxKeywordSpotterConfig extends Struct {
  external EdgevoxOnnxFeatureConfig feat;

  external EdgevoxOnnxOnlineModelConfig model;

  @Int32()
  external int maxActivePaths;

  @Int32()
  external int numTrailingBlanks;

  @Float()
  external double keywordsScore;

  @Float()
  external double keywordsThreshold;

  external Pointer<Utf8> keywordsFile;

  external Pointer<Utf8> keywordsBuf;

  @Int32()
  external int keywordsBufSize;
}

final class EdgevoxOnnxOfflinePunctuation extends Opaque {}

final class EdgevoxOnnxOnlinePunctuation extends Opaque {}

final class EdgevoxOnnxAudioTagging extends Opaque {}

final class EdgevoxOnnxKeywordSpotter extends Opaque {}

final class EdgevoxOnnxOfflineTts extends Opaque {}

final class EdgevoxOnnxCircularBuffer extends Opaque {}

final class EdgevoxOnnxVoiceActivityDetector extends Opaque {}

final class EdgevoxOnnxOnlineStream extends Opaque {}

final class EdgevoxOnnxOnlineRecognizer extends Opaque {}

final class EdgevoxOnnxOfflineRecognizer extends Opaque {}

final class EdgevoxOnnxOfflineStream extends Opaque {}

final class EdgevoxOnnxSpeakerEmbeddingExtractor extends Opaque {}

final class EdgevoxOnnxSpeakerEmbeddingManager extends Opaque {}

final class EdgevoxOnnxOfflineSpeakerDiarization extends Opaque {}

final class EdgevoxOnnxOfflineSpeakerDiarizationResult extends Opaque {}

final class EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig extends Struct {
  external Pointer<Utf8> encoder;
  external Pointer<Utf8> decoder;

  @Int32()
  external int tailPaddings;
}

final class EdgevoxOnnxSpokenLanguageIdentificationConfig extends Struct {
  external EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig whisper;

  @Int32()
  external int numThreads;

  @Int32()
  external int debug;

  external Pointer<Utf8> provider;
}

final class EdgevoxOnnxSpokenLanguageIdentificationResult extends Struct {
  external Pointer<Utf8> lang;
}

final class EdgevoxOnnxSpokenLanguageIdentification extends Opaque {}

final class EdgevoxOnnxOfflineSpeechDenoiser extends Opaque {}

final class EdgevoxOnnxOnlineSpeechDenoiser extends Opaque {}

typedef EdgevoxOnnxCreateOfflineSpeechDenoiserNative =
    Pointer<EdgevoxOnnxOfflineSpeechDenoiser> Function(
      Pointer<EdgevoxOnnxOfflineSpeechDenoiserConfig>,
    );

typedef EdgevoxOnnxCreateOfflineSpeechDenoiser =
    EdgevoxOnnxCreateOfflineSpeechDenoiserNative;

typedef EdgevoxOnnxDestroyOfflineSpeechDenoiserNative =
    Void Function(Pointer<EdgevoxOnnxOfflineSpeechDenoiser>);

typedef EdgevoxOnnxDestroyOfflineSpeechDenoiser =
    void Function(Pointer<EdgevoxOnnxOfflineSpeechDenoiser>);

typedef EdgevoxOnnxOfflineSpeechDenoiserGetSampleRateNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineSpeechDenoiser>);

typedef EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate =
    int Function(Pointer<EdgevoxOnnxOfflineSpeechDenoiser>);

typedef EdgevoxOnnxOfflineSpeechDenoiserRunNative =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOfflineSpeechDenoiser>,
      Pointer<Float>,
      Int32,
      Int32,
    );

typedef EdgevoxOnnxOfflineSpeechDenoiserRun =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOfflineSpeechDenoiser>,
      Pointer<Float>,
      int,
      int,
    );

typedef EdgevoxOnnxDestroyDenoisedAudioNative =
    Void Function(Pointer<EdgevoxOnnxDenoisedAudio>);

typedef EdgevoxOnnxDestroyDenoisedAudio =
    void Function(Pointer<EdgevoxOnnxDenoisedAudio>);

typedef EdgevoxOnnxCreateOnlineSpeechDenoiserNative =
    Pointer<EdgevoxOnnxOnlineSpeechDenoiser> Function(
      Pointer<EdgevoxOnnxOnlineSpeechDenoiserConfig>,
    );

typedef EdgevoxOnnxCreateOnlineSpeechDenoiser =
    EdgevoxOnnxCreateOnlineSpeechDenoiserNative;

typedef EdgevoxOnnxDestroyOnlineSpeechDenoiserNative =
    Void Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxDestroyOnlineSpeechDenoiser =
    void Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserGetSampleRateNative =
    Int32 Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate =
    int Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamplesNative =
    Int32 Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples =
    int Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserRunNative =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOnlineSpeechDenoiser>,
      Pointer<Float>,
      Int32,
      Int32,
    );

typedef EdgevoxOnnxOnlineSpeechDenoiserRun =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOnlineSpeechDenoiser>,
      Pointer<Float>,
      int,
      int,
    );

typedef EdgevoxOnnxOnlineSpeechDenoiserFlushNative =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOnlineSpeechDenoiser>,
    );

typedef EdgevoxOnnxOnlineSpeechDenoiserFlush =
    Pointer<EdgevoxOnnxDenoisedAudio> Function(
      Pointer<EdgevoxOnnxOnlineSpeechDenoiser>,
    );

typedef EdgevoxOnnxOnlineSpeechDenoiserResetNative =
    Void Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxOnlineSpeechDenoiserReset =
    void Function(Pointer<EdgevoxOnnxOnlineSpeechDenoiser>);

typedef EdgevoxOnnxCreateSpokenLanguageIdentificationNative =
    Pointer<EdgevoxOnnxSpokenLanguageIdentification> Function(
      Pointer<EdgevoxOnnxSpokenLanguageIdentificationConfig>,
    );

typedef EdgevoxOnnxCreateSpokenLanguageIdentification =
    EdgevoxOnnxCreateSpokenLanguageIdentificationNative;

typedef EdgevoxOnnxDestroySpokenLanguageIdentificationNative =
    Void Function(Pointer<EdgevoxOnnxSpokenLanguageIdentification>);

typedef EdgevoxOnnxDestroySpokenLanguageIdentification =
    void Function(Pointer<EdgevoxOnnxSpokenLanguageIdentification>);

typedef EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStreamNative =
    Pointer<EdgevoxOnnxOfflineStream> Function(
      Pointer<EdgevoxOnnxSpokenLanguageIdentification>,
    );

typedef EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream =
    EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStreamNative;

typedef EdgevoxOnnxSpokenLanguageIdentificationComputeNative =
    Pointer<EdgevoxOnnxSpokenLanguageIdentificationResult> Function(
      Pointer<EdgevoxOnnxSpokenLanguageIdentification>,
      Pointer<EdgevoxOnnxOfflineStream>,
    );

typedef EdgevoxOnnxSpokenLanguageIdentificationCompute =
    EdgevoxOnnxSpokenLanguageIdentificationComputeNative;

typedef EdgevoxOnnxDestroySpokenLanguageIdentificationResultNative =
    Void Function(Pointer<EdgevoxOnnxSpokenLanguageIdentificationResult>);

typedef EdgevoxOnnxDestroySpokenLanguageIdentificationResult =
    void Function(Pointer<EdgevoxOnnxSpokenLanguageIdentificationResult>);

typedef EdgevoxOnnxCreateOfflineSpeakerDiarizationNative =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarization> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarizationConfig>,
    );

typedef EdgevoxOnnxCreateOfflineSpeakerDiarization =
    EdgevoxOnnxCreateOfflineSpeakerDiarizationNative;

typedef EdgevoxOnnxDestroyOfflineSpeakerDiarizationNative =
    Void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarization>);

typedef EdgevoxOnnxDestroyOfflineSpeakerDiarization =
    void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarization>);

typedef EdgevoxOnnxCreateOfflinePunctuationNative =
    Pointer<EdgevoxOnnxOfflinePunctuation> Function(
      Pointer<EdgevoxOnnxOfflinePunctuationConfig>,
    );

typedef EdgevoxOnnxCreateOnlinePunctuationNative =
    Pointer<EdgevoxOnnxOnlinePunctuation> Function(
      Pointer<EdgevoxOnnxOnlinePunctuationConfig>,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRateNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarization>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate =
    int Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarization>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationSetConfigNative =
    Void Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<EdgevoxOnnxOfflineSpeakerDiarizationConfig>,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakersNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers =
    int Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegmentsNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments =
    int Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTimeNative =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarizationSegment> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime =
    EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTimeNative;

typedef EdgevoxOnnxOfflineSpeakerDiarizationDestroySegmentNative =
    Void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationSegment>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment =
    void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationSegment>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationProcessNative =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<Float>,
      Int32,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationProcess =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<Float>,
      int,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArgNative =
    Int32 Function(Int32, Int32);

typedef EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArgNative =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<Float>,
      Int32,
      Pointer<
        NativeFunction<
          EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArgNative
        >
      >,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg =
    Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult> Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<Float>,
      int,
      Pointer<
        NativeFunction<
          EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArgNative
        >
      >,
    );

typedef EdgevoxOnnxOfflineSpeakerDiarizationDestroyResultNative =
    Void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult =
    void Function(Pointer<EdgevoxOnnxOfflineSpeakerDiarizationResult>);

typedef EdgevoxOnnxOfflineSpeakerDiarizationSetConfig =
    void Function(
      Pointer<EdgevoxOnnxOfflineSpeakerDiarization>,
      Pointer<EdgevoxOnnxOfflineSpeakerDiarizationConfig>,
    );

typedef EdgevoxOnnxCreateOfflinePunctuation =
    EdgevoxOnnxCreateOfflinePunctuationNative;

typedef EdgevoxOnnxDestroyOfflinePunctuationNative =
    Void Function(Pointer<EdgevoxOnnxOfflinePunctuation>);

typedef EdgevoxOnnxDestroyOfflinePunctuation =
    void Function(Pointer<EdgevoxOnnxOfflinePunctuation>);

typedef SherpaOfflinePunctuationAddPunctNative =
    Pointer<Utf8> Function(
      Pointer<EdgevoxOnnxOfflinePunctuation>,
      Pointer<Utf8>,
    );

typedef SherpaOfflinePunctuationAddPunct =
    SherpaOfflinePunctuationAddPunctNative;

typedef SherpaOfflinePunctuationFreeTextNative = Void Function(Pointer<Utf8>);

typedef SherpaOfflinePunctuationFreeText = void Function(Pointer<Utf8>);

typedef EdgevoxOnnxCreateOnlinePunctuation =
    EdgevoxOnnxCreateOnlinePunctuationNative;

typedef EdgevoxOnnxDestroyOnlinePunctuationNative =
    Void Function(Pointer<EdgevoxOnnxOnlinePunctuation>);

typedef EdgevoxOnnxDestroyOnlinePunctuation =
    void Function(Pointer<EdgevoxOnnxOnlinePunctuation>);

typedef EdgevoxOnnxOnlinePunctuationAddPunctNative =
    Pointer<Utf8> Function(Pointer<EdgevoxOnnxOnlinePunctuation>, Pointer<Utf8>);

typedef EdgevoxOnnxOnlinePunctuationAddPunct =
    EdgevoxOnnxOnlinePunctuationAddPunctNative;

typedef EdgevoxOnnxOnlinePunctuationFreeTextNative =
    Void Function(Pointer<Utf8>);

typedef EdgevoxOnnxOnlinePunctuationFreeText = void Function(Pointer<Utf8>);

typedef EdgevoxOnnxCreateAudioTaggingNative =
    Pointer<EdgevoxOnnxAudioTagging> Function(
      Pointer<EdgevoxOnnxAudioTaggingConfig>,
    );

typedef EdgevoxOnnxCreateAudioTagging = EdgevoxOnnxCreateAudioTaggingNative;

typedef EdgevoxOnnxDestroyAudioTaggingNative =
    Void Function(Pointer<EdgevoxOnnxAudioTagging>);

typedef EdgevoxOnnxDestroyAudioTagging =
    void Function(Pointer<EdgevoxOnnxAudioTagging>);

typedef EdgevoxOnnxAudioTaggingCreateOfflineStreamNative =
    Pointer<EdgevoxOnnxOfflineStream> Function(Pointer<EdgevoxOnnxAudioTagging>);

typedef EdgevoxOnnxAudioTaggingCreateOfflineStream =
    EdgevoxOnnxAudioTaggingCreateOfflineStreamNative;

typedef EdgevoxOnnxAudioTaggingComputeNative =
    Pointer<Pointer<EdgevoxOnnxAudioEvent>> Function(
      Pointer<EdgevoxOnnxAudioTagging>,
      Pointer<EdgevoxOnnxOfflineStream>,
      Int32,
    );

typedef EdgevoxOnnxAudioTaggingCompute =
    Pointer<Pointer<EdgevoxOnnxAudioEvent>> Function(
      Pointer<EdgevoxOnnxAudioTagging>,
      Pointer<EdgevoxOnnxOfflineStream>,
      int,
    );

typedef EdgevoxOnnxAudioTaggingFreeResultsNative =
    Void Function(Pointer<Pointer<EdgevoxOnnxAudioEvent>>);

typedef EdgevoxOnnxAudioTaggingFreeResults =
    void Function(Pointer<Pointer<EdgevoxOnnxAudioEvent>>);

typedef CreateKeywordSpotterNative =
    Pointer<EdgevoxOnnxKeywordSpotter> Function(
      Pointer<EdgevoxOnnxKeywordSpotterConfig>,
    );

typedef CreateKeywordSpotter = CreateKeywordSpotterNative;

typedef DestroyKeywordSpotterNative =
    Void Function(Pointer<EdgevoxOnnxKeywordSpotter>);

typedef DestroyKeywordSpotter =
    void Function(Pointer<EdgevoxOnnxKeywordSpotter>);

typedef CreateKeywordStreamNative =
    Pointer<EdgevoxOnnxOnlineStream> Function(Pointer<EdgevoxOnnxKeywordSpotter>);

typedef CreateKeywordStream = CreateKeywordStreamNative;

typedef CreateKeywordStreamWithKeywordsNative =
    Pointer<EdgevoxOnnxOnlineStream> Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<Utf8>,
    );

typedef CreateKeywordStreamWithKeywords = CreateKeywordStreamWithKeywordsNative;

typedef IsKeywordStreamReadyNative =
    Int32 Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef IsKeywordStreamReady =
    int Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef DecodeKeywordStreamNative =
    Void Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef DecodeKeywordStream =
    void Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef ResetKeywordStreamNative =
    Void Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef ResetKeywordStream =
    void Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef GetKeywordResultAsJsonNative =
    Pointer<Utf8> Function(
      Pointer<EdgevoxOnnxKeywordSpotter>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef GetKeywordResultAsJson = GetKeywordResultAsJsonNative;

typedef FreeKeywordResultJsonNative = Void Function(Pointer<Utf8>);

typedef FreeKeywordResultJson = void Function(Pointer<Utf8>);

typedef EdgevoxOnnxCreateOfflineTtsNative =
    Pointer<EdgevoxOnnxOfflineTts> Function(Pointer<EdgevoxOnnxOfflineTtsConfig>);

typedef EdgevoxOnnxCreateOfflineTts = EdgevoxOnnxCreateOfflineTtsNative;

typedef EdgevoxOnnxDestroyOfflineTtsNative =
    Void Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxDestroyOfflineTts =
    void Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxOfflineTtsSampleRateNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxOfflineTtsSampleRate =
    int Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxOfflineTtsNumSpeakersNative =
    Int32 Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxOfflineTtsNumSpeakers =
    int Function(Pointer<EdgevoxOnnxOfflineTts>);

typedef EdgevoxOnnxOfflineTtsGenerateNative =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      Int32,
      Float,
    );

typedef EdgevoxOnnxOfflineTtsGenerate =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      int,
      double,
    );

typedef EdgevoxOnnxDestroyOfflineTtsGeneratedAudioNative =
    Void Function(Pointer<EdgevoxOnnxGeneratedAudio>);

typedef EdgevoxOnnxDestroyOfflineTtsGeneratedAudio =
    void Function(Pointer<EdgevoxOnnxGeneratedAudio>);

typedef EdgevoxOnnxGeneratedAudioCallbackNative =
    Int32 Function(Pointer<Float>, Int32);

typedef EdgevoxOnnxGeneratedAudioProgressCallbackWithArgNative =
    Int32 Function(Pointer<Float> samples, Int32 n, Float p, Pointer<Void> arg);

typedef EdgevoxOnnxGeneratedAudioProgressCallbackWithArg =
    int Function(Pointer<Float> samples, int n, double p, Pointer<Void> arg);

typedef EdgevoxOnnxOfflineTtsGenerateWithCallbackNative =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      Int32,
      Float,
      Pointer<NativeFunction<EdgevoxOnnxGeneratedAudioCallbackNative>>,
    );

typedef EdgevoxOnnxOfflineTtsGenerateWithCallback =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      int,
      double,
      Pointer<NativeFunction<EdgevoxOnnxGeneratedAudioCallbackNative>>,
    );

typedef EdgevoxOnnxOfflineTtsGenerateWithConfigNative =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      Pointer<EdgevoxOnnxGenerationConfig>,
      Pointer<
        NativeFunction<EdgevoxOnnxGeneratedAudioProgressCallbackWithArgNative>
      >,
      Pointer<Void>,
    );

typedef EdgevoxOnnxOfflineTtsGenerateWithConfig =
    Pointer<EdgevoxOnnxGeneratedAudio> Function(
      Pointer<EdgevoxOnnxOfflineTts>,
      Pointer<Utf8>,
      Pointer<EdgevoxOnnxGenerationConfig>,
      Pointer<
        NativeFunction<EdgevoxOnnxGeneratedAudioProgressCallbackWithArgNative>
      >,
      Pointer<Void>,
    );

typedef CreateOfflineRecognizerNative =
    Pointer<EdgevoxOnnxOfflineRecognizer> Function(
      Pointer<EdgevoxOnnxOfflineRecognizerConfig>,
    );

typedef CreateOfflineRecognizer = CreateOfflineRecognizerNative;

typedef OfflineRecognizerSetConfigNative =
    Void Function(
      Pointer<EdgevoxOnnxOfflineRecognizer>,
      Pointer<EdgevoxOnnxOfflineRecognizerConfig>,
    );

typedef OfflineRecognizerSetConfig =
    void Function(
      Pointer<EdgevoxOnnxOfflineRecognizer>,
      Pointer<EdgevoxOnnxOfflineRecognizerConfig>,
    );

typedef DestroyOfflineRecognizerNative =
    Void Function(Pointer<EdgevoxOnnxOfflineRecognizer>);

typedef DestroyOfflineRecognizer =
    void Function(Pointer<EdgevoxOnnxOfflineRecognizer>);

typedef CreateOfflineStreamNative =
    Pointer<EdgevoxOnnxOfflineStream> Function(
      Pointer<EdgevoxOnnxOfflineRecognizer>,
    );

typedef CreateOfflineStream = CreateOfflineStreamNative;

typedef DestroyOfflineStreamNative =
    Void Function(Pointer<EdgevoxOnnxOfflineStream>);

typedef DestroyOfflineStream = void Function(Pointer<EdgevoxOnnxOfflineStream>);

typedef AcceptWaveformOfflineNative =
    Void Function(
      Pointer<EdgevoxOnnxOfflineStream>,
      Int32,
      Pointer<Float>,
      Int32,
    );

typedef AcceptWaveformOffline =
    void Function(Pointer<EdgevoxOnnxOfflineStream>, int, Pointer<Float>, int);

typedef OfflineStreamSetOptionNative =
    Void Function(
      Pointer<EdgevoxOnnxOfflineStream>,
      Pointer<Utf8>,
      Pointer<Utf8>,
    );

typedef OfflineStreamSetOption =
    void Function(
      Pointer<EdgevoxOnnxOfflineStream>,
      Pointer<Utf8>,
      Pointer<Utf8>,
    );

typedef DecodeOfflineStreamNative =
    Void Function(
      Pointer<EdgevoxOnnxOfflineRecognizer>,
      Pointer<EdgevoxOnnxOfflineStream>,
    );

typedef DecodeOfflineStream =
    void Function(
      Pointer<EdgevoxOnnxOfflineRecognizer>,
      Pointer<EdgevoxOnnxOfflineStream>,
    );

typedef GetOfflineStreamResultAsJsonNative =
    Pointer<Utf8> Function(Pointer<EdgevoxOnnxOfflineStream>);

typedef GetOfflineStreamResultAsJson = GetOfflineStreamResultAsJsonNative;

typedef DestroyOfflineStreamResultJsonNative = Void Function(Pointer<Utf8>);

typedef DestroyOfflineStreamResultJson = void Function(Pointer<Utf8>);

typedef EdgevoxOnnxCreateOnlineRecognizerNative =
    Pointer<EdgevoxOnnxOnlineRecognizer> Function(
      Pointer<EdgevoxOnnxOnlineRecognizerConfig>,
    );

typedef EdgevoxOnnxCreateOnlineRecognizer =
    EdgevoxOnnxCreateOnlineRecognizerNative;

typedef EdgevoxOnnxDestroyOnlineRecognizerNative =
    Void Function(Pointer<EdgevoxOnnxOnlineRecognizer>);

typedef EdgevoxOnnxDestroyOnlineRecognizer =
    void Function(Pointer<EdgevoxOnnxOnlineRecognizer>);

typedef EdgevoxOnnxCreateOnlineStreamNative =
    Pointer<EdgevoxOnnxOnlineStream> Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
    );

typedef EdgevoxOnnxCreateOnlineStream = EdgevoxOnnxCreateOnlineStreamNative;

typedef EdgevoxOnnxCreateOnlineStreamWithHotwordsNative =
    Pointer<EdgevoxOnnxOnlineStream> Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<Utf8>,
    );

typedef EdgevoxOnnxCreateOnlineStreamWithHotwords =
    EdgevoxOnnxCreateOnlineStreamWithHotwordsNative;

typedef IsOnlineStreamReadyNative =
    Int32 Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef IsOnlineStreamReady =
    int Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef EdgevoxOnnxDecodeOnlineStreamNative =
    Void Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef EdgevoxOnnxDecodeOnlineStream =
    void Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef GetOnlineStreamResultAsJsonNative =
    Pointer<Utf8> Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef GetOnlineStreamResultAsJson = GetOnlineStreamResultAsJsonNative;

typedef ResetNative =
    Void Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef Reset =
    void Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef IsEndpointNative =
    Int32 Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef IsEndpoint =
    int Function(
      Pointer<EdgevoxOnnxOnlineRecognizer>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef DestroyOnlineStreamResultJsonNative = Void Function(Pointer<Utf8>);

typedef DestroyOnlineStreamResultJson = void Function(Pointer<Utf8>);

typedef EdgevoxOnnxCreateVoiceActivityDetectorNative =
    Pointer<EdgevoxOnnxVoiceActivityDetector> Function(
      Pointer<EdgevoxOnnxVadModelConfig>,
      Float,
    );

typedef EdgevoxOnnxCreateVoiceActivityDetector =
    Pointer<EdgevoxOnnxVoiceActivityDetector> Function(
      Pointer<EdgevoxOnnxVadModelConfig>,
      double,
    );

typedef EdgevoxOnnxDestroyVoiceActivityDetectorNative =
    Void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxDestroyVoiceActivityDetector =
    void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorAcceptWaveformNative =
    Void Function(
      Pointer<EdgevoxOnnxVoiceActivityDetector>,
      Pointer<Float>,
      Int32,
    );

typedef EdgevoxOnnxVoiceActivityDetectorAcceptWaveform =
    void Function(
      Pointer<EdgevoxOnnxVoiceActivityDetector>,
      Pointer<Float>,
      int,
    );

typedef EdgevoxOnnxVoiceActivityDetectorEmptyNative =
    Int32 Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorEmpty =
    int Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorDetectedNative =
    Int32 Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorDetected =
    int Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorPopNative =
    Void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorPop =
    void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorClearNative =
    Void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorClear =
    void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorResetNative =
    Void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorReset =
    void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorFlushNative =
    Void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorFlush =
    void Function(Pointer<EdgevoxOnnxVoiceActivityDetector>);

typedef EdgevoxOnnxVoiceActivityDetectorFrontNative =
    Pointer<EdgevoxOnnxSpeechSegment> Function(
      Pointer<EdgevoxOnnxVoiceActivityDetector>,
    );

typedef EdgevoxOnnxVoiceActivityDetectorFront =
    EdgevoxOnnxVoiceActivityDetectorFrontNative;

typedef EdgevoxOnnxDestroySpeechSegmentNative =
    Void Function(Pointer<EdgevoxOnnxSpeechSegment>);

typedef EdgevoxOnnxDestroySpeechSegment =
    void Function(Pointer<EdgevoxOnnxSpeechSegment>);

typedef EdgevoxOnnxCreateCircularBufferNative =
    Pointer<EdgevoxOnnxCircularBuffer> Function(Int32);

typedef EdgevoxOnnxCreateCircularBuffer =
    Pointer<EdgevoxOnnxCircularBuffer> Function(int);

typedef EdgevoxOnnxDestroyCircularBufferNative =
    Void Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxDestroyCircularBuffer =
    void Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferPushNative =
    Void Function(Pointer<EdgevoxOnnxCircularBuffer>, Pointer<Float>, Int32);

typedef EdgevoxOnnxCircularBufferPush =
    void Function(Pointer<EdgevoxOnnxCircularBuffer>, Pointer<Float>, int);

typedef EdgevoxOnnxCircularBufferGetNative =
    Pointer<Float> Function(Pointer<EdgevoxOnnxCircularBuffer>, Int32, Int32);

typedef EdgevoxOnnxCircularBufferGet =
    Pointer<Float> Function(Pointer<EdgevoxOnnxCircularBuffer>, int, int);

typedef EdgevoxOnnxCircularBufferFreeNative = Void Function(Pointer<Float>);

typedef EdgevoxOnnxCircularBufferFree = void Function(Pointer<Float>);

typedef EdgevoxOnnxCircularBufferPopNative =
    Void Function(Pointer<EdgevoxOnnxCircularBuffer>, Int32);

typedef EdgevoxOnnxCircularBufferPop =
    void Function(Pointer<EdgevoxOnnxCircularBuffer>, int);

typedef EdgevoxOnnxCircularBufferSizeNative =
    Int32 Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferSize =
    int Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferHeadNative =
    Int32 Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferHead =
    int Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferResetNative =
    Void Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCircularBufferReset =
    void Function(Pointer<EdgevoxOnnxCircularBuffer>);

typedef EdgevoxOnnxCreateSpeakerEmbeddingManagerNative =
    Pointer<EdgevoxOnnxSpeakerEmbeddingManager> Function(Int32);

typedef EdgevoxOnnxCreateSpeakerEmbeddingManager =
    Pointer<EdgevoxOnnxSpeakerEmbeddingManager> Function(int);

typedef EdgevoxOnnxDestroySpeakerEmbeddingManagerNative =
    Void Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>);

typedef EdgevoxOnnxDestroySpeakerEmbeddingManager =
    void Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerAddNative =
    Int32 Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerAdd =
    int Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattenedNative =
    Int32 Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
      Int32,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened =
    int Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
      int,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerRemoveNative =
    Int32 Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>, Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerRemove =
    int Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>, Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerContainsNative =
    Int32 Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>, Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerContains =
    int Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>, Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerSearchNative =
    Pointer<Utf8> Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Float>,
      Float,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerSearch =
    Pointer<Utf8> Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Float>,
      double,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerFreeSearchNative =
    Void Function(Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch =
    void Function(Pointer<Utf8>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakersNative =
    Int32 Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers =
    int Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerVerifyNative =
    Int32 Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
      Float,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerVerify =
    int Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingManager>,
      Pointer<Utf8>,
      Pointer<Float>,
      double,
    );

typedef EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakersNative =
    Pointer<Pointer<Utf8>> Function(Pointer<EdgevoxOnnxSpeakerEmbeddingManager>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers =
    EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakersNative;

typedef EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakersNative =
    Void Function(Pointer<Pointer<Utf8>>);

typedef EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers =
    void Function(Pointer<Pointer<Utf8>>);

typedef EdgevoxOnnxCreateSpeakerEmbeddingExtractorNative =
    Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor> Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingExtractorConfig>,
    );

typedef EdgevoxOnnxCreateSpeakerEmbeddingExtractor =
    EdgevoxOnnxCreateSpeakerEmbeddingExtractorNative;

typedef EdgevoxOnnxDestroySpeakerEmbeddingExtractorNative =
    Void Function(Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>);

typedef EdgevoxOnnxDestroySpeakerEmbeddingExtractor =
    void Function(Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>);

typedef EdgevoxOnnxSpeakerEmbeddingExtractorDimNative =
    Int32 Function(Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>);

typedef EdgevoxOnnxSpeakerEmbeddingExtractorDim =
    int Function(Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>);

typedef EdgevoxOnnxSpeakerEmbeddingExtractorCreateStreamNative =
    Pointer<EdgevoxOnnxOnlineStream> Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream =
    EdgevoxOnnxSpeakerEmbeddingExtractorCreateStreamNative;

typedef EdgevoxOnnxDestroyOnlineStreamNative =
    Void Function(Pointer<EdgevoxOnnxOnlineStream>);

typedef EdgevoxOnnxDestroyOnlineStream =
    void Function(Pointer<EdgevoxOnnxOnlineStream>);

typedef OnlineStreamAcceptWaveformNative =
    Void Function(
      Pointer<EdgevoxOnnxOnlineStream>,
      Int32,
      Pointer<Float>,
      Int32,
    );

typedef OnlineStreamAcceptWaveform =
    void Function(Pointer<EdgevoxOnnxOnlineStream>, int, Pointer<Float>, int);

typedef OnlineStreamInputFinishedNative =
    Void Function(Pointer<EdgevoxOnnxOnlineStream>);

typedef OnlineStreamInputFinished =
    void Function(Pointer<EdgevoxOnnxOnlineStream>);

typedef OnlineStreamSetOptionNative =
    Void Function(
      Pointer<EdgevoxOnnxOnlineStream>,
      Pointer<Utf8>,
      Pointer<Utf8>,
    );

typedef OnlineStreamSetOption =
    void Function(
      Pointer<EdgevoxOnnxOnlineStream>,
      Pointer<Utf8>,
      Pointer<Utf8>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingExtractorIsReadyNative =
    Int32 Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingExtractorIsReady =
    int Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbeddingNative =
    Pointer<Float> Function(
      Pointer<EdgevoxOnnxSpeakerEmbeddingExtractor>,
      Pointer<EdgevoxOnnxOnlineStream>,
    );

typedef EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding =
    EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbeddingNative;

typedef EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbeddingNative =
    Void Function(Pointer<Float>);

typedef EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding =
    void Function(Pointer<Float>);

typedef EdgevoxOnnxReadWaveNative =
    Pointer<EdgevoxOnnxWave> Function(Pointer<Utf8>);

typedef EdgevoxOnnxReadWave = EdgevoxOnnxReadWaveNative;

typedef EdgevoxOnnxWriteWaveNative =
    Int32 Function(Pointer<Float>, Int32, Int32, Pointer<Utf8>);

typedef EdgevoxOnnxWriteWave =
    int Function(Pointer<Float>, int, int, Pointer<Utf8>);

typedef EdgevoxOnnxFreeWaveNative = Void Function(Pointer<EdgevoxOnnxWave>);

typedef EdgevoxOnnxFreeWave = void Function(Pointer<EdgevoxOnnxWave>);

typedef EdgevoxOnnxGetVersionStr = Pointer<Utf8> Function();
typedef EdgevoxOnnxGetVersionStrNative = EdgevoxOnnxGetVersionStr;

typedef EdgevoxOnnxGetGitSha1Native = Pointer<Utf8> Function();
typedef EdgevoxOnnxGetGitSha1 = EdgevoxOnnxGetGitSha1Native;

typedef EdgevoxOnnxGetGitDateNative = Pointer<Utf8> Function();
typedef EdgevoxOnnxGetGitDate = EdgevoxOnnxGetGitDateNative;

class EdgevoxOnnxBindings {
  static EdgevoxOnnxCreateOfflineSpeechDenoiser?
  edgevoxOnnxCreateOfflineSpeechDenoiser;

  static EdgevoxOnnxDestroyOfflineSpeechDenoiser?
  edgevoxOnnxDestroyOfflineSpeechDenoiser;

  static EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate?
  edgevoxOnnxOfflineSpeechDenoiserGetSampleRate;
  static EdgevoxOnnxOfflineSpeechDenoiserRun? edgevoxOnnxOfflineSpeechDenoiserRun;
  static EdgevoxOnnxDestroyDenoisedAudio? edgevoxOnnxDestroyDenoisedAudio;
  static EdgevoxOnnxCreateOnlineSpeechDenoiser?
  edgevoxOnnxCreateOnlineSpeechDenoiser;
  static EdgevoxOnnxDestroyOnlineSpeechDenoiser?
  edgevoxOnnxDestroyOnlineSpeechDenoiser;
  static EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate?
  edgevoxOnnxOnlineSpeechDenoiserGetSampleRate;
  static EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples?
  edgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples;
  static EdgevoxOnnxOnlineSpeechDenoiserRun? edgevoxOnnxOnlineSpeechDenoiserRun;
  static EdgevoxOnnxOnlineSpeechDenoiserFlush?
  edgevoxOnnxOnlineSpeechDenoiserFlush;
  static EdgevoxOnnxOnlineSpeechDenoiserReset?
  edgevoxOnnxOnlineSpeechDenoiserReset;

  static EdgevoxOnnxCreateSpokenLanguageIdentification?
  edgevoxOnnxCreateSpokenLanguageIdentification;
  static EdgevoxOnnxDestroySpokenLanguageIdentification?
  edgevoxOnnxDestroySpokenLanguageIdentification;
  static EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream?
  edgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream;
  static EdgevoxOnnxSpokenLanguageIdentificationCompute?
  edgevoxOnnxSpokenLanguageIdentificationCompute;
  static EdgevoxOnnxDestroySpokenLanguageIdentificationResult?
  edgevoxOnnxDestroySpokenLanguageIdentificationResult;

  static EdgevoxOnnxCreateOfflineSpeakerDiarization?
  edgevoxOnnxCreateOfflineSpeakerDiarization;
  static EdgevoxOnnxDestroyOfflineSpeakerDiarization?
  edgevoxOnnxDestroyOfflineSpeakerDiarization;
  static EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate?
  edgevoxOnnxOfflineSpeakerDiarizationGetSampleRate;
  static EdgevoxOnnxOfflineSpeakerDiarizationSetConfig?
  edgevoxOnnxOfflineSpeakerDiarizationSetConfig;
  static EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers?
  edgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers;
  static EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments?
  edgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments;
  static EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime?
  edgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime;
  static EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment?
  edgevoxOnnxOfflineSpeakerDiarizationDestroySegment;
  static EdgevoxOnnxOfflineSpeakerDiarizationProcess?
  edgevoxOnnxOfflineSpeakerDiarizationProcess;
  static EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult?
  edgevoxOnnxOfflineSpeakerDiarizationDestroyResult;
  static EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg?
  edgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg;

  static EdgevoxOnnxCreateOfflinePunctuation? edgevoxOnnxCreateOfflinePunctuation;
  static EdgevoxOnnxDestroyOfflinePunctuation?
  edgevoxOnnxDestroyOfflinePunctuation;
  static SherpaOfflinePunctuationAddPunct? sherpaOfflinePunctuationAddPunct;
  static SherpaOfflinePunctuationFreeText? sherpaOfflinePunctuationFreeText;

  static EdgevoxOnnxCreateOnlinePunctuation? edgevoxOnnxCreateOnlinePunctuation;
  static EdgevoxOnnxDestroyOnlinePunctuation? edgevoxOnnxDestroyOnlinePunctuation;
  static EdgevoxOnnxOnlinePunctuationAddPunct?
  edgevoxOnnxOnlinePunctuationAddPunct;
  static EdgevoxOnnxOnlinePunctuationFreeText?
  edgevoxOnnxOnlinePunctuationFreeText;

  static EdgevoxOnnxCreateAudioTagging? edgevoxOnnxCreateAudioTagging;
  static EdgevoxOnnxDestroyAudioTagging? edgevoxOnnxDestroyAudioTagging;
  static EdgevoxOnnxAudioTaggingCreateOfflineStream?
  edgevoxOnnxAudioTaggingCreateOfflineStream;
  static EdgevoxOnnxAudioTaggingCompute? edgevoxOnnxAudioTaggingCompute;
  static EdgevoxOnnxAudioTaggingFreeResults? edgevoxOnnxAudioTaggingFreeResults;

  static CreateKeywordSpotter? createKeywordSpotter;
  static DestroyKeywordSpotter? destroyKeywordSpotter;
  static CreateKeywordStream? createKeywordStream;
  static CreateKeywordStreamWithKeywords? createKeywordStreamWithKeywords;
  static IsKeywordStreamReady? isKeywordStreamReady;
  static DecodeKeywordStream? decodeKeywordStream;
  static ResetKeywordStream? resetKeywordStream;
  static GetKeywordResultAsJson? getKeywordResultAsJson;
  static FreeKeywordResultJson? freeKeywordResultJson;

  static EdgevoxOnnxCreateOfflineTts? createOfflineTts;
  static EdgevoxOnnxDestroyOfflineTts? destroyOfflineTts;
  static EdgevoxOnnxOfflineTtsSampleRate? offlineTtsSampleRate;
  static EdgevoxOnnxOfflineTtsNumSpeakers? offlineTtsNumSpeakers;
  static EdgevoxOnnxOfflineTtsGenerate? offlineTtsGenerate;
  static EdgevoxOnnxDestroyOfflineTtsGeneratedAudio?
  destroyOfflineTtsGeneratedAudio;
  static EdgevoxOnnxOfflineTtsGenerateWithCallback?
  offlineTtsGenerateWithCallback;

  static EdgevoxOnnxOfflineTtsGenerateWithConfig? offlineTtsGenerateWithConfig;

  static CreateOfflineRecognizer? createOfflineRecognizer;
  static DestroyOfflineRecognizer? destroyOfflineRecognizer;
  static OfflineRecognizerSetConfig? offlineRecognizerSetConfig;
  static CreateOfflineStream? createOfflineStream;
  static DestroyOfflineStream? destroyOfflineStream;
  static AcceptWaveformOffline? acceptWaveformOffline;
  static OfflineStreamSetOption? offlineStreamSetOption;
  static DecodeOfflineStream? decodeOfflineStream;
  static GetOfflineStreamResultAsJson? getOfflineStreamResultAsJson;
  static DestroyOfflineStreamResultJson? destroyOfflineStreamResultJson;

  static EdgevoxOnnxCreateOnlineRecognizer? createOnlineRecognizer;

  static EdgevoxOnnxDestroyOnlineRecognizer? destroyOnlineRecognizer;

  static EdgevoxOnnxCreateOnlineStream? createOnlineStream;

  static EdgevoxOnnxCreateOnlineStreamWithHotwords?
  createOnlineStreamWithHotwords;

  static IsOnlineStreamReady? isOnlineStreamReady;

  static EdgevoxOnnxDecodeOnlineStream? decodeOnlineStream;

  static GetOnlineStreamResultAsJson? getOnlineStreamResultAsJson;

  static Reset? reset;

  static IsEndpoint? isEndpoint;

  static DestroyOnlineStreamResultJson? destroyOnlineStreamResultJson;

  static EdgevoxOnnxCreateVoiceActivityDetector? createVoiceActivityDetector;

  static EdgevoxOnnxDestroyVoiceActivityDetector? destroyVoiceActivityDetector;

  static EdgevoxOnnxVoiceActivityDetectorAcceptWaveform?
  voiceActivityDetectorAcceptWaveform;

  static EdgevoxOnnxVoiceActivityDetectorEmpty? voiceActivityDetectorEmpty;

  static EdgevoxOnnxVoiceActivityDetectorDetected? voiceActivityDetectorDetected;

  static EdgevoxOnnxVoiceActivityDetectorPop? voiceActivityDetectorPop;

  static EdgevoxOnnxVoiceActivityDetectorClear? voiceActivityDetectorClear;

  static EdgevoxOnnxVoiceActivityDetectorFront? voiceActivityDetectorFront;

  static EdgevoxOnnxDestroySpeechSegment? destroySpeechSegment;

  static EdgevoxOnnxVoiceActivityDetectorReset? voiceActivityDetectorReset;

  static EdgevoxOnnxVoiceActivityDetectorFlush? voiceActivityDetectorFlush;

  static EdgevoxOnnxCreateCircularBuffer? createCircularBuffer;

  static EdgevoxOnnxDestroyCircularBuffer? destroyCircularBuffer;

  static EdgevoxOnnxCircularBufferPush? circularBufferPush;

  static EdgevoxOnnxCircularBufferGet? circularBufferGet;

  static EdgevoxOnnxCircularBufferFree? circularBufferFree;

  static EdgevoxOnnxCircularBufferPop? circularBufferPop;

  static EdgevoxOnnxCircularBufferSize? circularBufferSize;

  static EdgevoxOnnxCircularBufferHead? circularBufferHead;

  static EdgevoxOnnxCircularBufferReset? circularBufferReset;

  static EdgevoxOnnxCreateSpeakerEmbeddingExtractor?
  createSpeakerEmbeddingExtractor;

  static EdgevoxOnnxDestroySpeakerEmbeddingExtractor?
  destroySpeakerEmbeddingExtractor;

  static EdgevoxOnnxSpeakerEmbeddingExtractorDim? speakerEmbeddingExtractorDim;

  static EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream?
  speakerEmbeddingExtractorCreateStream;

  static EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding?
  speakerEmbeddingExtractorComputeEmbedding;

  static EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding?
  speakerEmbeddingExtractorDestroyEmbedding;

  static EdgevoxOnnxDestroyOnlineStream? destroyOnlineStream;

  static OnlineStreamAcceptWaveform? onlineStreamAcceptWaveform;

  static OnlineStreamInputFinished? onlineStreamInputFinished;

  static OnlineStreamSetOption? onlineStreamSetOption;

  static EdgevoxOnnxSpeakerEmbeddingExtractorIsReady?
  speakerEmbeddingExtractorIsReady;

  static EdgevoxOnnxCreateSpeakerEmbeddingManager? createSpeakerEmbeddingManager;

  static EdgevoxOnnxDestroySpeakerEmbeddingManager?
  destroySpeakerEmbeddingManager;

  static EdgevoxOnnxSpeakerEmbeddingManagerAdd? speakerEmbeddingManagerAdd;

  static EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened?
  speakerEmbeddingManagerAddListFlattened;

  static EdgevoxOnnxSpeakerEmbeddingManagerRemove? speakerEmbeddingManagerRemove;

  static EdgevoxOnnxSpeakerEmbeddingManagerContains?
  speakerEmbeddingManagerContains;

  static EdgevoxOnnxSpeakerEmbeddingManagerSearch? speakerEmbeddingManagerSearch;

  static EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch?
  speakerEmbeddingManagerFreeSearch;

  static EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers?
  speakerEmbeddingManagerNumSpeakers;

  static EdgevoxOnnxSpeakerEmbeddingManagerVerify? speakerEmbeddingManagerVerify;

  static EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers?
  speakerEmbeddingManagerGetAllSpeakers;

  static EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers?
  speakerEmbeddingManagerFreeAllSpeakers;

  static EdgevoxOnnxReadWave? readWave;

  static EdgevoxOnnxWriteWave? writeWave;

  static EdgevoxOnnxFreeWave? freeWave;

  static EdgevoxOnnxGetVersionStr? getVersionStr;
  static EdgevoxOnnxGetGitSha1? getGitSha1;
  static EdgevoxOnnxGetGitDate? getGitDate;

  static void init(DynamicLibrary dynamicLibrary) {
    edgevoxOnnxCreateOfflineSpeechDenoiser ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOfflineSpeechDenoiserNative>>(
          'EdgevoxOnnxCreateOfflineSpeechDenoiser',
        )
        .asFunction();

    edgevoxOnnxDestroyOfflineSpeechDenoiser ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOfflineSpeechDenoiserNative>>(
          'EdgevoxOnnxDestroyOfflineSpeechDenoiser',
        )
        .asFunction();

    edgevoxOnnxOfflineSpeechDenoiserGetSampleRate ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOfflineSpeechDenoiserGetSampleRateNative>
        >('EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate')
        .asFunction();

    edgevoxOnnxOfflineSpeechDenoiserRun ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineSpeechDenoiserRunNative>>(
          'EdgevoxOnnxOfflineSpeechDenoiserRun',
        )
        .asFunction();

    edgevoxOnnxDestroyDenoisedAudio ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyDenoisedAudioNative>>(
          'EdgevoxOnnxDestroyDenoisedAudio',
        )
        .asFunction();

    edgevoxOnnxCreateOnlineSpeechDenoiser ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOnlineSpeechDenoiserNative>>(
          'EdgevoxOnnxCreateOnlineSpeechDenoiser',
        )
        .asFunction();

    edgevoxOnnxDestroyOnlineSpeechDenoiser ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOnlineSpeechDenoiserNative>>(
          'EdgevoxOnnxDestroyOnlineSpeechDenoiser',
        )
        .asFunction();

    edgevoxOnnxOnlineSpeechDenoiserGetSampleRate ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOnlineSpeechDenoiserGetSampleRateNative>
        >('EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate')
        .asFunction();

    edgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamplesNative
          >
        >('EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples')
        .asFunction();

    edgevoxOnnxOnlineSpeechDenoiserRun ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOnlineSpeechDenoiserRunNative>>(
          'EdgevoxOnnxOnlineSpeechDenoiserRun',
        )
        .asFunction();

    edgevoxOnnxOnlineSpeechDenoiserFlush ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOnlineSpeechDenoiserFlushNative>>(
          'EdgevoxOnnxOnlineSpeechDenoiserFlush',
        )
        .asFunction();

    edgevoxOnnxOnlineSpeechDenoiserReset ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOnlineSpeechDenoiserResetNative>>(
          'EdgevoxOnnxOnlineSpeechDenoiserReset',
        )
        .asFunction();

    edgevoxOnnxCreateSpokenLanguageIdentification ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxCreateSpokenLanguageIdentificationNative>
        >('EdgevoxOnnxCreateSpokenLanguageIdentification')
        .asFunction();

    edgevoxOnnxDestroySpokenLanguageIdentification ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxDestroySpokenLanguageIdentificationNative>
        >('EdgevoxOnnxDestroySpokenLanguageIdentification')
        .asFunction();

    edgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStreamNative
          >
        >('EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream')
        .asFunction();

    edgevoxOnnxSpokenLanguageIdentificationCompute ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpokenLanguageIdentificationComputeNative>
        >('EdgevoxOnnxSpokenLanguageIdentificationCompute')
        .asFunction();

    edgevoxOnnxDestroySpokenLanguageIdentificationResult ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxDestroySpokenLanguageIdentificationResultNative
          >
        >('EdgevoxOnnxDestroySpokenLanguageIdentificationResult')
        .asFunction();

    edgevoxOnnxCreateOfflineSpeakerDiarization ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxCreateOfflineSpeakerDiarizationNative>
        >('EdgevoxOnnxCreateOfflineSpeakerDiarization')
        .asFunction();

    edgevoxOnnxDestroyOfflineSpeakerDiarization ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxDestroyOfflineSpeakerDiarizationNative>
        >('EdgevoxOnnxDestroyOfflineSpeakerDiarization')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationGetSampleRate ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRateNative>
        >('EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationSetConfig ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOfflineSpeakerDiarizationSetConfigNative>
        >('EdgevoxOnnxOfflineSpeakerDiarizationSetConfig')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakersNative
          >
        >('EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegmentsNative
          >
        >('EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTimeNative
          >
        >('EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationDestroySegment ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxOfflineSpeakerDiarizationDestroySegmentNative
          >
        >('EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationProcess ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOfflineSpeakerDiarizationProcessNative>
        >('EdgevoxOnnxOfflineSpeakerDiarizationProcess')
        .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg ??=
        dynamicLibrary
            .lookup<
              NativeFunction<
                EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArgNative
              >
            >('EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg')
            .asFunction();

    edgevoxOnnxOfflineSpeakerDiarizationDestroyResult ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxOfflineSpeakerDiarizationDestroyResultNative>
        >('EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult')
        .asFunction();

    edgevoxOnnxCreateOfflinePunctuation ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOfflinePunctuationNative>>(
          'EdgevoxOnnxCreateOfflinePunctuation',
        )
        .asFunction();

    edgevoxOnnxDestroyOfflinePunctuation ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOfflinePunctuationNative>>(
          'EdgevoxOnnxDestroyOfflinePunctuation',
        )
        .asFunction();

    sherpaOfflinePunctuationAddPunct ??= dynamicLibrary
        .lookup<NativeFunction<SherpaOfflinePunctuationAddPunctNative>>(
          'SherpaOfflinePunctuationAddPunct',
        )
        .asFunction();

    sherpaOfflinePunctuationFreeText ??= dynamicLibrary
        .lookup<NativeFunction<SherpaOfflinePunctuationFreeTextNative>>(
          'SherpaOfflinePunctuationFreeText',
        )
        .asFunction();

    edgevoxOnnxCreateOnlinePunctuation ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOnlinePunctuationNative>>(
          'EdgevoxOnnxCreateOnlinePunctuation',
        )
        .asFunction();

    edgevoxOnnxDestroyOnlinePunctuation ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOnlinePunctuationNative>>(
          'EdgevoxOnnxDestroyOnlinePunctuation',
        )
        .asFunction();

    edgevoxOnnxOnlinePunctuationAddPunct ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOnlinePunctuationAddPunctNative>>(
          'EdgevoxOnnxOnlinePunctuationAddPunct',
        )
        .asFunction();

    edgevoxOnnxOnlinePunctuationFreeText ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOnlinePunctuationFreeTextNative>>(
          'EdgevoxOnnxOnlinePunctuationFreeText',
        )
        .asFunction();

    edgevoxOnnxCreateAudioTagging ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateAudioTaggingNative>>(
          'EdgevoxOnnxCreateAudioTagging',
        )
        .asFunction();

    edgevoxOnnxDestroyAudioTagging ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyAudioTaggingNative>>(
          'EdgevoxOnnxDestroyAudioTagging',
        )
        .asFunction();

    edgevoxOnnxAudioTaggingCreateOfflineStream ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxAudioTaggingCreateOfflineStreamNative>
        >('EdgevoxOnnxAudioTaggingCreateOfflineStream')
        .asFunction();

    edgevoxOnnxAudioTaggingCompute ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxAudioTaggingComputeNative>>(
          'EdgevoxOnnxAudioTaggingCompute',
        )
        .asFunction();

    edgevoxOnnxAudioTaggingFreeResults ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxAudioTaggingFreeResultsNative>>(
          'EdgevoxOnnxAudioTaggingFreeResults',
        )
        .asFunction();

    createKeywordSpotter ??= dynamicLibrary
        .lookup<NativeFunction<CreateKeywordSpotterNative>>(
          'EdgevoxOnnxCreateKeywordSpotter',
        )
        .asFunction();

    destroyKeywordSpotter ??= dynamicLibrary
        .lookup<NativeFunction<DestroyKeywordSpotterNative>>(
          'EdgevoxOnnxDestroyKeywordSpotter',
        )
        .asFunction();

    createKeywordStream ??= dynamicLibrary
        .lookup<NativeFunction<CreateKeywordStreamNative>>(
          'EdgevoxOnnxCreateKeywordStream',
        )
        .asFunction();

    createKeywordStreamWithKeywords ??= dynamicLibrary
        .lookup<NativeFunction<CreateKeywordStreamWithKeywordsNative>>(
          'EdgevoxOnnxCreateKeywordStreamWithKeywords',
        )
        .asFunction();

    isKeywordStreamReady ??= dynamicLibrary
        .lookup<NativeFunction<IsKeywordStreamReadyNative>>(
          'EdgevoxOnnxIsKeywordStreamReady',
        )
        .asFunction();

    decodeKeywordStream ??= dynamicLibrary
        .lookup<NativeFunction<DecodeKeywordStreamNative>>(
          'EdgevoxOnnxDecodeKeywordStream',
        )
        .asFunction();

    resetKeywordStream ??= dynamicLibrary
        .lookup<NativeFunction<ResetKeywordStreamNative>>(
          'EdgevoxOnnxResetKeywordStream',
        )
        .asFunction();

    getKeywordResultAsJson ??= dynamicLibrary
        .lookup<NativeFunction<GetKeywordResultAsJsonNative>>(
          'EdgevoxOnnxGetKeywordResultAsJson',
        )
        .asFunction();

    freeKeywordResultJson ??= dynamicLibrary
        .lookup<NativeFunction<FreeKeywordResultJsonNative>>(
          'EdgevoxOnnxFreeKeywordResultJson',
        )
        .asFunction();

    createOfflineTts ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOfflineTtsNative>>(
          'EdgevoxOnnxCreateOfflineTts',
        )
        .asFunction();

    destroyOfflineTts ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOfflineTtsNative>>(
          'EdgevoxOnnxDestroyOfflineTts',
        )
        .asFunction();

    offlineTtsSampleRate ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineTtsSampleRateNative>>(
          'EdgevoxOnnxOfflineTtsSampleRate',
        )
        .asFunction();

    offlineTtsNumSpeakers ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineTtsNumSpeakersNative>>(
          'EdgevoxOnnxOfflineTtsNumSpeakers',
        )
        .asFunction();

    offlineTtsGenerate ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineTtsGenerateNative>>(
          'EdgevoxOnnxOfflineTtsGenerate',
        )
        .asFunction();

    destroyOfflineTtsGeneratedAudio ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxDestroyOfflineTtsGeneratedAudioNative>
        >('EdgevoxOnnxDestroyOfflineTtsGeneratedAudio')
        .asFunction();

    offlineTtsGenerateWithCallback ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineTtsGenerateWithCallbackNative>>(
          'EdgevoxOnnxOfflineTtsGenerateWithCallback',
        )
        .asFunction();

    offlineTtsGenerateWithConfig ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxOfflineTtsGenerateWithConfigNative>>(
          'EdgevoxOnnxOfflineTtsGenerateWithConfig',
        )
        .asFunction();

    createOfflineRecognizer ??= dynamicLibrary
        .lookup<NativeFunction<CreateOfflineRecognizerNative>>(
          'EdgevoxOnnxCreateOfflineRecognizer',
        )
        .asFunction();

    destroyOfflineRecognizer ??= dynamicLibrary
        .lookup<NativeFunction<DestroyOfflineRecognizerNative>>(
          'EdgevoxOnnxDestroyOfflineRecognizer',
        )
        .asFunction();

    offlineRecognizerSetConfig ??= dynamicLibrary
        .lookup<NativeFunction<OfflineRecognizerSetConfigNative>>(
          'EdgevoxOnnxOfflineRecognizerSetConfig',
        )
        .asFunction();

    createOfflineStream ??= dynamicLibrary
        .lookup<NativeFunction<CreateOfflineStreamNative>>(
          'EdgevoxOnnxCreateOfflineStream',
        )
        .asFunction();

    destroyOfflineStream ??= dynamicLibrary
        .lookup<NativeFunction<DestroyOfflineStreamNative>>(
          'EdgevoxOnnxDestroyOfflineStream',
        )
        .asFunction();

    acceptWaveformOffline ??= dynamicLibrary
        .lookup<NativeFunction<AcceptWaveformOfflineNative>>(
          'EdgevoxOnnxAcceptWaveformOffline',
        )
        .asFunction();

    offlineStreamSetOption ??= dynamicLibrary
        .lookup<NativeFunction<OfflineStreamSetOptionNative>>(
          'EdgevoxOnnxOfflineStreamSetOption',
        )
        .asFunction();

    decodeOfflineStream ??= dynamicLibrary
        .lookup<NativeFunction<DecodeOfflineStreamNative>>(
          'EdgevoxOnnxDecodeOfflineStream',
        )
        .asFunction();

    getOfflineStreamResultAsJson ??= dynamicLibrary
        .lookup<NativeFunction<GetOfflineStreamResultAsJsonNative>>(
          'EdgevoxOnnxGetOfflineStreamResultAsJson',
        )
        .asFunction();

    destroyOfflineStreamResultJson ??= dynamicLibrary
        .lookup<NativeFunction<DestroyOfflineStreamResultJsonNative>>(
          'EdgevoxOnnxDestroyOfflineStreamResultJson',
        )
        .asFunction();

    createOnlineRecognizer ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOnlineRecognizerNative>>(
          'EdgevoxOnnxCreateOnlineRecognizer',
        )
        .asFunction();

    destroyOnlineRecognizer ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOnlineRecognizerNative>>(
          'EdgevoxOnnxDestroyOnlineRecognizer',
        )
        .asFunction();

    createOnlineStream ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOnlineStreamNative>>(
          'EdgevoxOnnxCreateOnlineStream',
        )
        .asFunction();

    createOnlineStreamWithHotwords ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateOnlineStreamWithHotwordsNative>>(
          'EdgevoxOnnxCreateOnlineStreamWithHotwords',
        )
        .asFunction();

    isOnlineStreamReady ??= dynamicLibrary
        .lookup<NativeFunction<IsOnlineStreamReadyNative>>(
          'EdgevoxOnnxIsOnlineStreamReady',
        )
        .asFunction();

    decodeOnlineStream ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDecodeOnlineStreamNative>>(
          'EdgevoxOnnxDecodeOnlineStream',
        )
        .asFunction();

    getOnlineStreamResultAsJson ??= dynamicLibrary
        .lookup<NativeFunction<GetOnlineStreamResultAsJsonNative>>(
          'EdgevoxOnnxGetOnlineStreamResultAsJson',
        )
        .asFunction();

    reset ??= dynamicLibrary
        .lookup<NativeFunction<ResetNative>>('EdgevoxOnnxOnlineStreamReset')
        .asFunction();

    isEndpoint ??= dynamicLibrary
        .lookup<NativeFunction<IsEndpointNative>>(
          'EdgevoxOnnxOnlineStreamIsEndpoint',
        )
        .asFunction();

    destroyOnlineStreamResultJson ??= dynamicLibrary
        .lookup<NativeFunction<DestroyOnlineStreamResultJsonNative>>(
          'EdgevoxOnnxDestroyOnlineStreamResultJson',
        )
        .asFunction();

    createVoiceActivityDetector ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateVoiceActivityDetectorNative>>(
          'EdgevoxOnnxCreateVoiceActivityDetector',
        )
        .asFunction();

    destroyVoiceActivityDetector ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyVoiceActivityDetectorNative>>(
          'EdgevoxOnnxDestroyVoiceActivityDetector',
        )
        .asFunction();

    voiceActivityDetectorAcceptWaveform ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxVoiceActivityDetectorAcceptWaveformNative>
        >('EdgevoxOnnxVoiceActivityDetectorAcceptWaveform')
        .asFunction();

    voiceActivityDetectorEmpty ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorEmptyNative>>(
          'EdgevoxOnnxVoiceActivityDetectorEmpty',
        )
        .asFunction();

    voiceActivityDetectorDetected ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorDetectedNative>>(
          'EdgevoxOnnxVoiceActivityDetectorDetected',
        )
        .asFunction();

    voiceActivityDetectorPop ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorPopNative>>(
          'EdgevoxOnnxVoiceActivityDetectorPop',
        )
        .asFunction();

    voiceActivityDetectorClear ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorClearNative>>(
          'EdgevoxOnnxVoiceActivityDetectorClear',
        )
        .asFunction();

    voiceActivityDetectorFront ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorFrontNative>>(
          'EdgevoxOnnxVoiceActivityDetectorFront',
        )
        .asFunction();

    destroySpeechSegment ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroySpeechSegmentNative>>(
          'EdgevoxOnnxDestroySpeechSegment',
        )
        .asFunction();

    voiceActivityDetectorReset ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorResetNative>>(
          'EdgevoxOnnxVoiceActivityDetectorReset',
        )
        .asFunction();

    voiceActivityDetectorFlush ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxVoiceActivityDetectorFlushNative>>(
          'EdgevoxOnnxVoiceActivityDetectorFlush',
        )
        .asFunction();

    createCircularBuffer ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateCircularBufferNative>>(
          'EdgevoxOnnxCreateCircularBuffer',
        )
        .asFunction();

    destroyCircularBuffer ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyCircularBufferNative>>(
          'EdgevoxOnnxDestroyCircularBuffer',
        )
        .asFunction();

    circularBufferPush ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferPushNative>>(
          'EdgevoxOnnxCircularBufferPush',
        )
        .asFunction();

    circularBufferGet ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferGetNative>>(
          'EdgevoxOnnxCircularBufferGet',
        )
        .asFunction();

    circularBufferFree ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferFreeNative>>(
          'EdgevoxOnnxCircularBufferFree',
        )
        .asFunction();

    circularBufferPop ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferPopNative>>(
          'EdgevoxOnnxCircularBufferPop',
        )
        .asFunction();

    circularBufferSize ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferSizeNative>>(
          'EdgevoxOnnxCircularBufferSize',
        )
        .asFunction();

    circularBufferHead ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferHeadNative>>(
          'EdgevoxOnnxCircularBufferHead',
        )
        .asFunction();

    circularBufferReset ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCircularBufferResetNative>>(
          'EdgevoxOnnxCircularBufferReset',
        )
        .asFunction();

    createSpeakerEmbeddingExtractor ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxCreateSpeakerEmbeddingExtractorNative>
        >('EdgevoxOnnxCreateSpeakerEmbeddingExtractor')
        .asFunction();

    destroySpeakerEmbeddingExtractor ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxDestroySpeakerEmbeddingExtractorNative>
        >('EdgevoxOnnxDestroySpeakerEmbeddingExtractor')
        .asFunction();

    speakerEmbeddingExtractorDim ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxSpeakerEmbeddingExtractorDimNative>>(
          'EdgevoxOnnxSpeakerEmbeddingExtractorDim',
        )
        .asFunction();

    speakerEmbeddingExtractorCreateStream ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingExtractorCreateStreamNative>
        >('EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream')
        .asFunction();

    speakerEmbeddingExtractorComputeEmbedding ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbeddingNative
          >
        >('EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding')
        .asFunction();

    speakerEmbeddingExtractorDestroyEmbedding ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbeddingNative
          >
        >('EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding')
        .asFunction();

    destroyOnlineStream ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroyOnlineStreamNative>>(
          'EdgevoxOnnxDestroyOnlineStream',
        )
        .asFunction();

    onlineStreamAcceptWaveform ??= dynamicLibrary
        .lookup<NativeFunction<OnlineStreamAcceptWaveformNative>>(
          'EdgevoxOnnxOnlineStreamAcceptWaveform',
        )
        .asFunction();

    onlineStreamInputFinished ??= dynamicLibrary
        .lookup<NativeFunction<OnlineStreamInputFinishedNative>>(
          'EdgevoxOnnxOnlineStreamInputFinished',
        )
        .asFunction();

    onlineStreamSetOption ??= dynamicLibrary
        .lookup<NativeFunction<OnlineStreamSetOptionNative>>(
          'EdgevoxOnnxOnlineStreamSetOption',
        )
        .asFunction();

    speakerEmbeddingExtractorIsReady ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingExtractorIsReadyNative>
        >('EdgevoxOnnxSpeakerEmbeddingExtractorIsReady')
        .asFunction();

    createSpeakerEmbeddingManager ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxCreateSpeakerEmbeddingManagerNative>>(
          'EdgevoxOnnxCreateSpeakerEmbeddingManager',
        )
        .asFunction();

    destroySpeakerEmbeddingManager ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxDestroySpeakerEmbeddingManagerNative>>(
          'EdgevoxOnnxDestroySpeakerEmbeddingManager',
        )
        .asFunction();

    speakerEmbeddingManagerAdd ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerAddNative>>(
          'EdgevoxOnnxSpeakerEmbeddingManagerAdd',
        )
        .asFunction();

    speakerEmbeddingManagerAddListFlattened ??= dynamicLibrary
        .lookup<
          NativeFunction<
            EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattenedNative
          >
        >('EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened')
        .asFunction();

    speakerEmbeddingManagerRemove ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerRemoveNative>>(
          'EdgevoxOnnxSpeakerEmbeddingManagerRemove',
        )
        .asFunction();

    speakerEmbeddingManagerContains ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerContainsNative>
        >('EdgevoxOnnxSpeakerEmbeddingManagerContains')
        .asFunction();

    speakerEmbeddingManagerSearch ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerSearchNative>>(
          'EdgevoxOnnxSpeakerEmbeddingManagerSearch',
        )
        .asFunction();

    speakerEmbeddingManagerFreeSearch ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerFreeSearchNative>
        >('EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch')
        .asFunction();

    speakerEmbeddingManagerNumSpeakers ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakersNative>
        >('EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers')
        .asFunction();

    speakerEmbeddingManagerVerify ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerVerifyNative>>(
          'EdgevoxOnnxSpeakerEmbeddingManagerVerify',
        )
        .asFunction();

    speakerEmbeddingManagerGetAllSpeakers ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakersNative>
        >('EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers')
        .asFunction();

    speakerEmbeddingManagerFreeAllSpeakers ??= dynamicLibrary
        .lookup<
          NativeFunction<EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakersNative>
        >('EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers')
        .asFunction();

    readWave ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxReadWaveNative>>('EdgevoxOnnxReadWave')
        .asFunction();

    writeWave ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxWriteWaveNative>>(
          'EdgevoxOnnxWriteWave',
        )
        .asFunction();

    freeWave ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxFreeWaveNative>>('EdgevoxOnnxFreeWave')
        .asFunction();

    getVersionStr ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxGetVersionStrNative>>(
          'EdgevoxOnnxGetVersionStr',
        )
        .asFunction();

    getGitSha1 ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxGetGitSha1Native>>(
          'EdgevoxOnnxGetGitSha1',
        )
        .asFunction();

    getGitDate ??= dynamicLibrary
        .lookup<NativeFunction<EdgevoxOnnxGetGitDateNative>>(
          'EdgevoxOnnxGetGitDate',
        )
        .asFunction();
  }
}
