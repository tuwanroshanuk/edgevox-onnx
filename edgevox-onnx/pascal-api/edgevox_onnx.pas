{ Copyright (c)  2024  Xiaomi Corporation

Please see
https://github.com/k2-fsa/edgevox-onnx/tree/master/pascal-api-examples
for how to use APIs in this file.
}

unit edgevox_onnx;

{$IFDEF FPC}
  {$mode objfpc}
  {$modeSwitch advancedRecords} { to support records with methods }
{$ENDIF}

{$LongStrings ON}

interface
uses
  ctypes;

type
  TEdgevoxOnnxSamplesArray = array of Single;

  TEdgevoxOnnxLinearResampler = class
  private
    Handle: Pointer;
    InputSampleRate: Integer;
    OutputSampleRate: Integer;
  public
    constructor Create(SampleRateIn: Integer; SampleRateOut: Integer);
    destructor Destroy; override;

    function Resample(Samples: pcfloat;
      N: Integer; Flush: Boolean): TEdgevoxOnnxSamplesArray; overload;

    function Resample(const Samples: array of Single;
      Flush: Boolean): TEdgevoxOnnxSamplesArray; overload;

    procedure Reset;

    property GetInputSampleRate: Integer Read InputSampleRate;
    property GetOutputSampleRate: Integer Read OutputSampleRate;
  end;

  TEdgevoxOnnxGeneratedAudioCallbackWithArg = function(
      Samples: pcfloat; N: cint32;
      Arg: Pointer): cint32; cdecl;

  TEdgevoxOnnxGeneratedAudioProgressCallbackWithArg = function(
      Samples: pcfloat; N: cint32; P: cfloat;
      Arg: Pointer): cint32; cdecl;

  TEdgevoxOnnxOfflineTtsVitsModelConfig = record
    Model: AnsiString;
    Lexicon: AnsiString;
    Tokens: AnsiString;
    DataDir: AnsiString;
    NoiseScale: Single;
    NoiseScaleW: Single;
    LengthScale: Single;
    DictDir: AnsiString;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsVitsModelConfig);
  end;

  TEdgevoxOnnxGenerationConfig = record
    SilenceScale: Single;
    Speed: Single;
    Sid: Integer;
    ReferenceAudio: array of Single;
    ReferenceAudioLen: Integer;
    ReferenceSampleRate: Integer;
    ReferenceText: AnsiString;
    NumSteps: Integer;
    Extra: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxGenerationConfig);
  end;

  TEdgevoxOnnxOfflineTtsMatchaModelConfig = record
    AcousticModel: AnsiString;
    Vocoder: AnsiString;
    Lexicon: AnsiString;
    Tokens: AnsiString;
    DataDir: AnsiString;
    NoiseScale: Single;
    LengthScale: Single;
    DictDir: AnsiString;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsMatchaModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsKokoroModelConfig = record
    Model: AnsiString;
    Voices: AnsiString;
    Tokens: AnsiString;
    DataDir: AnsiString;
    LengthScale: Single;
    DictDir: AnsiString;
    Lexicon: AnsiString;
    Lang: AnsiString;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsKokoroModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsKittenModelConfig = record
    Model: AnsiString;
    Voices: AnsiString;
    Tokens: AnsiString;
    DataDir: AnsiString;
    LengthScale: Single;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsKittenModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsZipVoiceModelConfig = record
    Tokens: AnsiString;
    Encoder: AnsiString;
    Decoder: AnsiString;
    Vocoder: AnsiString;
    DataDir: AnsiString;
    Lexicon: AnsiString;
    EspeakVoice: AnsiString;
    FeatScale: Single;
    Tshift: Single;
    TargetRms: Single;
    GuidanceScale: Single;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsZipVoiceModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsPocketModelConfig = record
    LmFlow: AnsiString;
    LmMain: AnsiString;
    Encoder: AnsiString;
    Decoder: AnsiString;
    TextConditioner: AnsiString;
    VocabJson: AnsiString;
    TokenScoresJson: AnsiString;
    VoiceEmbeddingCacheCapacity: Integer;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsPocketModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsSupertonicModelConfig = record
    DurationPredictor: AnsiString;
    TextEncoder: AnsiString;
    VectorEstimator: AnsiString;
    Vocoder: AnsiString;
    TtsJson: AnsiString;
    UnicodeIndexer: AnsiString;
    VoiceStyle: AnsiString;

    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineTtsModelConfig = record
    Vits: TEdgevoxOnnxOfflineTtsVitsModelConfig;
    NumThreads: Integer;
    Debug: Boolean;
    Provider: AnsiString;
    Matcha: TEdgevoxOnnxOfflineTtsMatchaModelConfig;
    Kokoro: TEdgevoxOnnxOfflineTtsKokoroModelConfig;
    Kitten: TEdgevoxOnnxOfflineTtsKittenModelConfig;
    ZipVoice: TEdgevoxOnnxOfflineTtsZipVoiceModelConfig;
    Pocket: TEdgevoxOnnxOfflineTtsPocketModelConfig;
    Supertonic: TEdgevoxOnnxOfflineTtsSupertonicModelConfig;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsModelConfig);
  end;

  TEdgevoxOnnxOfflineTtsConfig = record
    Model: TEdgevoxOnnxOfflineTtsModelConfig;
    RuleFsts: AnsiString;
    MaxNumSentences: Integer;
    RuleFars: AnsiString;
    SilenceScale: Single;

    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsConfig);
  end;

  TEdgevoxOnnxGeneratedAudio = record
    Samples: array of Single;
    SampleRate: Integer;
  end;

  TEdgevoxOnnxOfflineTts = class
  private
   Handle: Pointer;
   SampleRate: Integer;
   NumSpeakers: Integer;
   _Config: TEdgevoxOnnxOfflineTtsConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOfflineTtsConfig);
    destructor Destroy; override;

    function Generate(Text: AnsiString; SpeakerId: Integer;
      Speed: Single): TEdgevoxOnnxGeneratedAudio; overload;

    function Generate(Text: AnsiString; SpeakerId: Integer;
      Speed: Single;
      Callback: TEdgevoxOnnxGeneratedAudioCallbackWithArg;
      Arg: Pointer
      ): TEdgevoxOnnxGeneratedAudio; overload;

    function Generate(Text: AnsiString;
      GenerationConfig: TEdgevoxOnnxGenerationConfig;
      Callback: TEdgevoxOnnxGeneratedAudioProgressCallbackWithArg;
      Arg: Pointer
      ): TEdgevoxOnnxGeneratedAudio; overload;

    property GetHandle: Pointer Read Handle;
    property GetSampleRate: Integer Read SampleRate;
    property GetNumSpeakers: Integer Read NumSpeakers;
  end;

  TEdgevoxOnnxWave = record
    Samples: array of Single; { normalized to the range [-1, 1] }
    SampleRate: Integer;
  end;

  TEdgevoxOnnxOnlineTransducerModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    Joiner: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineParaformerModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineZipformer2CtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineNemoCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineToneCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineModelConfig = record
    Transducer: TEdgevoxOnnxOnlineTransducerModelConfig;
    Paraformer: TEdgevoxOnnxOnlineParaformerModelConfig;
    Zipformer2Ctc: TEdgevoxOnnxOnlineZipformer2CtcModelConfig;
    Tokens: AnsiString;
    NumThreads: Integer;
    Provider: AnsiString;
    Debug: Boolean;
    ModelType: AnsiString;
    ModelingUnit: AnsiString;
    BpeVocab: AnsiString;
    TokensBuf: AnsiString;
    TokensBufSize: Integer;
    NemoCtc: TEdgevoxOnnxOnlineNemoCtcModelConfig;
    ToneCtc: TEdgevoxOnnxOnlineToneCtcModelConfig;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineModelConfig);
  end;

  TEdgevoxOnnxFeatureConfig = record
    SampleRate: Integer;
    FeatureDim: Integer;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxFeatureConfig);
  end;

  TEdgevoxOnnxOnlineCtcFstDecoderConfig = record
    Graph: AnsiString;
    MaxActive: Integer;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineCtcFstDecoderConfig);
  end;

  TEdgevoxOnnxHomophoneReplacerConfig = record
    DictDir: AnsiString;
    Lexicon: AnsiString;
    RuleFsts: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineRecognizerConfig = record
    FeatConfig: TEdgevoxOnnxFeatureConfig;
    ModelConfig: TEdgevoxOnnxOnlineModelConfig;
    DecodingMethod: AnsiString;
    MaxActivePaths: Integer;
    EnableEndpoint: Boolean;
    Rule1MinTrailingSilence: Single;
    Rule2MinTrailingSilence: Single;
    Rule3MinUtteranceLength: Single;
    HotwordsFile: AnsiString;
    HotwordsScore: Single;
    CtcFstDecoderConfig: TEdgevoxOnnxOnlineCtcFstDecoderConfig;
    RuleFsts: AnsiString;
    RuleFars: AnsiString;
    BlankPenalty: Single;
    HotwordsBuf: AnsiString;
    HotwordsBufSize: Integer;
    Hr: TEdgevoxOnnxHomophoneReplacerConfig;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineRecognizerConfig);
  end;

  TEdgevoxOnnxOnlineRecognizerResult = record
    Text: AnsiString;
    Tokens: array of AnsiString;
    Timestamps: array of Single;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineStream = class
  private
   Handle: Pointer;
  public
    constructor Create(P: Pointer);
    destructor Destroy; override;
    procedure AcceptWaveform(const Samples: array of Single; SampleRate: Integer);
    procedure InputFinished;
    property GetHandle: Pointer Read Handle;
  end;

  TEdgevoxOnnxOnlineRecognizer = class
  private
   Handle: Pointer;
   _Config: TEdgevoxOnnxOnlineRecognizerConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOnlineRecognizerConfig);
    destructor Destroy; override;

    function CreateStream: TEdgevoxOnnxOnlineStream; overload;
    function CreateStream(Hotwords: AnsiString): TEdgevoxOnnxOnlineStream; overload;
    function IsReady(Stream: TEdgevoxOnnxOnlineStream): Boolean;
    procedure Decode(Stream: TEdgevoxOnnxOnlineStream);
    procedure Reset(Stream: TEdgevoxOnnxOnlineStream);
    function IsEndpoint(Stream: TEdgevoxOnnxOnlineStream): Boolean;
    function GetResult(Stream: TEdgevoxOnnxOnlineStream): TEdgevoxOnnxOnlineRecognizerResult;
    property Config: TEdgevoxOnnxOnlineRecognizerConfig Read _Config;
    property GetHandle: Pointer Read Handle;
  end;

  TEdgevoxOnnxOfflineTransducerModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    Joiner: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineParaformerModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineNemoEncDecCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineDolphinModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineZipformerCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineWenetCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineMedAsrCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineFireRedAsrCtcModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineQwen3ASRModelConfig = record
    ConvFrontend: AnsiString;
    Encoder: AnsiString;
    Decoder: AnsiString;
    Tokenizer: AnsiString;
    MaxTotalLen: Integer;
    MaxNewTokens: Integer;
    Temperature: Single;
    TopP: Single;
    Seed: Integer;
    Hotwords: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineQwen3ASRModelConfig);
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineFunAsrNanoModelConfig = record
    EncoderAdaptor: AnsiString;
    LLM: AnsiString;
    Embedding: AnsiString;
    Tokenizer: AnsiString;
    SystemPrompt: AnsiString;
    UserPrompt: AnsiString;
    MaxNewTokens: Integer;
    Temperature: Single;
    TopP: Single;
    Seed: Integer;
    Language: AnsiString;
    UseItn: Boolean;
    Hotwords: AnsiString;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineFunAsrNanoModelConfig);
  end;

  TEdgevoxOnnxOfflineWhisperModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    Language: AnsiString;
    Task: AnsiString;
    TailPaddings: Integer;
    EnableTokenTimestamps: Boolean;
    EnableSegmentTimestamps: Boolean;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineWhisperModelConfig);
  end;

  TEdgevoxOnnxOfflineCanaryModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    SrcLang: AnsiString;
    TgtLang: AnsiString;
    UsePnc: Boolean;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineCanaryModelConfig);
  end;

  TEdgevoxOnnxOfflineCohereTranscribeModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    Language: AnsiString;
    UsePunct: Boolean;
    UseItn: Boolean;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineCohereTranscribeModelConfig);
  end;

  TEdgevoxOnnxOfflineMoonshineModelConfig = record
    Preprocessor: AnsiString;
    Encoder: AnsiString;
    UncachedDecoder: AnsiString;
    CachedDecoder: AnsiString;
    MergedDecoder: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineFireRedAsrModelConfig = record
    Encoder: AnsiString;
    Decoder: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineTdnnModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineLMConfig = record
    Model: AnsiString;
    Scale: Single;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineLMConfig);
  end;

  TEdgevoxOnnxOfflineSenseVoiceModelConfig = record
    Model: AnsiString;
    Language: AnsiString;
    UseItn: Boolean;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSenseVoiceModelConfig);
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineModelConfig = record
    Transducer: TEdgevoxOnnxOfflineTransducerModelConfig;
    Paraformer: TEdgevoxOnnxOfflineParaformerModelConfig;
    NeMoCtc: TEdgevoxOnnxOfflineNemoEncDecCtcModelConfig;
    Whisper: TEdgevoxOnnxOfflineWhisperModelConfig;
    Tdnn: TEdgevoxOnnxOfflineTdnnModelConfig;
    Tokens: AnsiString;
    NumThreads: Integer;
    Debug: Boolean;
    Provider: AnsiString;
    ModelType: AnsiString;
    ModelingUnit: AnsiString;
    BpeVocab: AnsiString;
    TeleSpeechCtc: AnsiString;
    SenseVoice: TEdgevoxOnnxOfflineSenseVoiceModelConfig;
    Moonshine: TEdgevoxOnnxOfflineMoonshineModelConfig;
    FireRedAsr: TEdgevoxOnnxOfflineFireRedAsrModelConfig;
    Dolphin: TEdgevoxOnnxOfflineDolphinModelConfig;
    ZipformerCtc: TEdgevoxOnnxOfflineZipformerCtcModelConfig;
    Canary: TEdgevoxOnnxOfflineCanaryModelConfig;
    WenetCtc: TEdgevoxOnnxOfflineWenetCtcModelConfig;
    Omnilingual: TEdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig;
    MedAsr: TEdgevoxOnnxOfflineMedAsrCtcModelConfig;
    FunAsrNano: TEdgevoxOnnxOfflineFunAsrNanoModelConfig;
    FireRedAsrCtc: TEdgevoxOnnxOfflineFireRedAsrCtcModelConfig;
    Qwen3Asr: TEdgevoxOnnxOfflineQwen3ASRModelConfig;
    CohereTranscribe: TEdgevoxOnnxOfflineCohereTranscribeModelConfig;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineModelConfig);
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineRecognizerConfig = record
    FeatConfig: TEdgevoxOnnxFeatureConfig;
    ModelConfig: TEdgevoxOnnxOfflineModelConfig;
    LMConfig: TEdgevoxOnnxOfflineLMConfig;
    DecodingMethod: AnsiString;
    MaxActivePaths: Integer;
    HotwordsFile: AnsiString;
    HotwordsScore: Single;
    RuleFsts: AnsiString;
    RuleFars: AnsiString;
    BlankPenalty: Single;
    Hr: TEdgevoxOnnxHomophoneReplacerConfig;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineRecognizerConfig);
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineRecognizerResult = record
    Text: AnsiString;
    Tokens: array of AnsiString;
    Timestamps: array of Single;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineStream = class
  private
   Handle: Pointer;
  public
    constructor Create(P: Pointer);
    destructor Destroy; override;
    procedure AcceptWaveform(const Samples: array of Single; SampleRate: Integer);
    procedure SetOption(const Key: AnsiString; const Value: AnsiString);
    property GetHandle: Pointer Read Handle;
  end;

  TEdgevoxOnnxOfflineRecognizer = class
  private
   Handle: Pointer;
   _Config: TEdgevoxOnnxOfflineRecognizerConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOfflineRecognizerConfig);
    destructor Destroy; override;
    function CreateStream: TEdgevoxOnnxOfflineStream;
    procedure Decode(Stream: TEdgevoxOnnxOfflineStream);
    procedure SetConfig(Config: TEdgevoxOnnxOfflineRecognizerConfig);
    function GetResult(Stream: TEdgevoxOnnxOfflineStream): TEdgevoxOnnxOfflineRecognizerResult;
    property Config: TEdgevoxOnnxOfflineRecognizerConfig Read _Config;
    property GetHandle: Pointer Read Handle;
  end;

  TEdgevoxOnnxSileroVadModelConfig = record
    Model: AnsiString;
    Threshold: Single;
    MinSilenceDuration: Single;
    MinSpeechDuration: Single;
    WindowSize: Integer;
    MaxSpeechDuration: Single;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxSileroVadModelConfig);
  end;

  TEdgevoxOnnxTenVadModelConfig = record
    Model: AnsiString;
    Threshold: Single;
    MinSilenceDuration: Single;
    MinSpeechDuration: Single;
    WindowSize: Integer;
    MaxSpeechDuration: Single;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxTenVadModelConfig);
  end;

  TEdgevoxOnnxVadModelConfig = record
    SileroVad: TEdgevoxOnnxSileroVadModelConfig;
    SampleRate: Integer;
    NumThreads: Integer;
    Provider: AnsiString;
    Debug: Boolean;
    TenVad: TEdgevoxOnnxTenVadModelConfig;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxVadModelConfig);
  end;


  TEdgevoxOnnxCircularBuffer = class
  private
    Handle: Pointer;
  public
    constructor Create(Capacity: Integer);
    destructor Destroy; override;
    procedure Push(Samples: array of Single); overload;
    procedure Push(Samples: pcfloat; N: Integer); overload;
    function Get(StartIndex: Integer; N: Integer): TEdgevoxOnnxSamplesArray;
    procedure Pop(N: Integer);
    procedure Reset;
    function Size: Integer;
    function Head: Integer;
    property GetHandle: Pointer Read Handle;
  end;

  TEdgevoxOnnxSpeechSegment = record
    Samples: array of Single;
    Start: Integer;
  end;

  TEdgevoxOnnxVoiceActivityDetector = class
  private
    Handle: Pointer;
    _Config: TEdgevoxOnnxVadModelConfig;
  public
    constructor Create(Config: TEdgevoxOnnxVadModelConfig; BufferSizeInSeconds: Single);
    destructor Destroy; override;
    procedure AcceptWaveform(const Samples: array of Single); overload;
    procedure AcceptWaveform(const Samples: array of Single; Offset: Integer; N: Integer); overload;
    function IsEmpty: Boolean;
    function IsDetected: Boolean;
    procedure Pop;
    procedure Clear;
    function Front: TEdgevoxOnnxSpeechSegment;
    procedure Reset;
    procedure Flush;
    property Config: TEdgevoxOnnxVadModelConfig Read _Config;
    property GetHandle: Pointer Read Handle;
  end;


  TEdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig = record
    Pyannote: TEdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig;
    NumThreads: Integer;
    Debug: Boolean;
    Provider: AnsiString;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig);
  end;

  TEdgevoxOnnxFastClusteringConfig = record
    NumClusters: Integer;
    Threshold: Single;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxFastClusteringConfig);
  end;

  TEdgevoxOnnxSpeakerEmbeddingExtractorConfig = record
    Model: AnsiString;
    NumThreads: Integer;
    Debug: Boolean;
    Provider: AnsiString;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxSpeakerEmbeddingExtractorConfig);
  end;

  TEdgevoxOnnxOfflineSpeakerDiarizationConfig = record
    Segmentation: TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig;
    Embedding: TEdgevoxOnnxSpeakerEmbeddingExtractorConfig;
    Clustering: TEdgevoxOnnxFastClusteringConfig;
    MinDurationOn: Single;
    MinDurationOff: Single;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
  end;

  TEdgevoxOnnxOfflineSpeakerDiarizationSegment = record
    Start: Single;
    Stop: Single;
    Speaker: Integer;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineSpeakerDiarizationSegmentArray = array of TEdgevoxOnnxOfflineSpeakerDiarizationSegment;

  PEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg = ^TEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg;

  TEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg = function(
      NumProcessChunks: cint32;
      NumTotalChunks: cint32): cint32; cdecl;

  TEdgevoxOnnxOfflineSpeakerDiarization = class
  private
    Handle: Pointer;
    SampleRate: Integer;
    _Config: TEdgevoxOnnxOfflineSpeakerDiarizationConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
    destructor Destroy; override;
    procedure SetConfig(Config: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
    function Process(const Samples: array of Single): TEdgevoxOnnxOfflineSpeakerDiarizationSegmentArray; overload;
    function Process(const Samples: array of Single; Callback: PEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg): TEdgevoxOnnxOfflineSpeakerDiarizationSegmentArray; overload;
    property GetHandle: Pointer Read Handle;
    property GetSampleRate: Integer Read SampleRate;
  end;

  TEdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig = record
    Model: AnsiString;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOfflineSpeechDenoiserModelConfig = record
    Gtcrn: TEdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig;
    DpdfNet: TEdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig;
    NumThreads: Integer;
    Debug: Boolean;
    Provider: AnsiString;
    function ToString: AnsiString;
    class operator Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeechDenoiserModelConfig);
  end;

  TEdgevoxOnnxOfflineSpeechDenoiserConfig = record
    Model: TEdgevoxOnnxOfflineSpeechDenoiserModelConfig;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxOnlineSpeechDenoiserConfig = record
    Model: TEdgevoxOnnxOfflineSpeechDenoiserModelConfig;
    function ToString: AnsiString;
  end;

  TEdgevoxOnnxDenoisedAudio = record
    Samples: array of Single;
    SampleRate: Integer;
  end;

  TEdgevoxOnnxOfflineSpeechDenoiser = class
  private
   Handle: Pointer;
   SampleRate: Integer;
   _Config: TEdgevoxOnnxOfflineSpeechDenoiserConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOfflineSpeechDenoiserConfig);
    destructor Destroy; override;

    function Run(const Samples: array of Single; InputSampleRate: Integer): TEdgevoxOnnxDenoisedAudio;

    property GetHandle: Pointer Read Handle;
    property GetSampleRate: Integer Read SampleRate;
  end;

  TEdgevoxOnnxOnlineSpeechDenoiser = class
  private
   Handle: Pointer;
   SampleRate: Integer;
   FrameShiftInSamples: Integer;
   _Config: TEdgevoxOnnxOnlineSpeechDenoiserConfig;
  public
    constructor Create(Config: TEdgevoxOnnxOnlineSpeechDenoiserConfig);
    destructor Destroy; override;

    function Run(const Samples: array of Single; InputSampleRate: Integer): TEdgevoxOnnxDenoisedAudio;
    function Flush: TEdgevoxOnnxDenoisedAudio;
    procedure Reset;

    property GetHandle: Pointer Read Handle;
    property GetSampleRate: Integer Read SampleRate;
    property GetFrameShiftInSamples: Integer Read FrameShiftInSamples;
  end;

  { It supports reading a single channel wave with 16-bit encoded samples.
    Samples are normalized to the range [-1, 1].
  }
  function EdgevoxOnnxReadWave(Filename: AnsiString): TEdgevoxOnnxWave;

  function EdgevoxOnnxWriteWave(Filename: AnsiString;
    const Samples: array of Single; SampleRate: Integer): Boolean;

  function EdgevoxOnnxGetVersionStr(): AnsiString;
  function EdgevoxOnnxGetGitSha1(): AnsiString;
  function EdgevoxOnnxGetGitDate(): AnsiString;

implementation

uses
  Math,
  fpjson,
    { See
      - https://wiki.freepascal.org/fcl-json
      - https://www.freepascal.org/daily/doc/fcl/fpjson/getjson.html
    }
  jsonparser,
  SysUtils;

const
  {
  See
   - https://www.freepascal.org/docs-html/prog/progap7.html
   - https://downloads.freepascal.org/fpc/docs-pdf/
   - https://downloads.freepascal.org/fpc/docs-pdf/CinFreePascal.pdf
  }

  {$if defined(WINDOWS)}
   { For windows, we always use dynamic link. See
     https://forum.lazarus.freepascal.org/index.php/topic,15712.msg84781.html#msg84781
     We need to rebuild the static lib for windows using Mingw or cygwin
   }
     EdgevoxOnnxLibName = 'edgevox-onnx-c-api.dll';
  {$elseif not defined(EDGEVOX_ONNX_USE_SHARED_LIBS)}
     {static link for linux and macos}
     {$linklib edgevox-onnx-c-api}
     {$linklib edgevox-onnx-core}
     {$linklib kaldi-decoder-core}
     {$linklib edgevox-onnx-kaldifst-core}
     {$linklib edgevox-onnx-fstfar}
     {$linklib edgevox-onnx-fst}
     {$linklib kissfft-float}
     {$linklib kaldi-native-fbank-core}
     {$linklib piper_phonemize}
     {$linklib espeak-ng}
     {$linklib ucd}
     {$linklib onnxruntime}
     {$linklib ssentencepiece_core}

     {$ifdef LINUX}
       {$linklib m}
       {$LINKLIB stdc++}
       {$LINKLIB gcc_s}
     {$endif}

     {$ifdef DARWIN}
       {$linklib c++}
     {$endif}
     EdgevoxOnnxLibName = '';
  {$else}
     {dynamic link for linux and macos}
     EdgevoxOnnxLibName = 'edgevox-onnx-c-api';
     {$linklib edgevox-onnx-c-api}
  {$endif}

type
  EdgevoxOnnxWave = record
    Samples: pcfloat;
    SampleRate: cint32;
    NumSamples: cint32;
  end;

  PEdgevoxOnnxWave = ^EdgevoxOnnxWave;

  EdgevoxOnnxOnlineTransducerModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Joiner: PAnsiChar;
  end;
  EdgevoxOnnxOnlineParaformerModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
  end;
  EdgevoxOnnxOnlineZipformer2CtcModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOnlineNemoCtcModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOnlineToneCtcModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOnlineModelConfig= record
    Transducer: EdgevoxOnnxOnlineTransducerModelConfig;
    Paraformer: EdgevoxOnnxOnlineParaformerModelConfig;
    Zipformer2Ctc: EdgevoxOnnxOnlineZipformer2CtcModelConfig;
    Tokens: PAnsiChar;
    NumThreads: cint32;
    Provider: PAnsiChar;
    Debug: cint32;
    ModelType: PAnsiChar;
    ModelingUnit: PAnsiChar;
    BpeVocab: PAnsiChar;
    TokensBuf: PAnsiChar;
    TokensBufSize: cint32;
    NemoCtc: EdgevoxOnnxOnlineNemoCtcModelConfig;
    ToneCtc: EdgevoxOnnxOnlineToneCtcModelConfig;
  end;
  EdgevoxOnnxFeatureConfig = record
    SampleRate: cint32;
    FeatureDim: cint32;
  end;
  EdgevoxOnnxOnlineCtcFstDecoderConfig = record
    Graph: PAnsiChar;
    MaxActive: cint32;
  end;

  EdgevoxOnnxHomophoneReplacerConfig = record
    DictDir: PAnsiChar;
    Lexicon: PAnsiChar;
    RuleFsts: PAnsiChar;
  end;

  EdgevoxOnnxOnlineRecognizerConfig = record
    FeatConfig: EdgevoxOnnxFeatureConfig;
    ModelConfig: EdgevoxOnnxOnlineModelConfig;
    DecodingMethod: PAnsiChar;
    MaxActivePaths: cint32;
    EnableEndpoint: cint32;
    Rule1MinTrailingSilence: cfloat;
    Rule2MinTrailingSilence: cfloat;
    Rule3MinUtteranceLength: cfloat;
    HotwordsFile: PAnsiChar;
    HotwordsScore: cfloat;
    CtcFstDecoderConfig: EdgevoxOnnxOnlineCtcFstDecoderConfig;
    RuleFsts: PAnsiChar;
    RuleFars: PAnsiChar;
    BlankPenalty: cfloat;
    HotwordsBuf: PAnsiChar;
    HotwordsBufSize: cint32;
    Hr: EdgevoxOnnxHomophoneReplacerConfig;
  end;

  PEdgevoxOnnxOnlineRecognizerConfig = ^EdgevoxOnnxOnlineRecognizerConfig;

  EdgevoxOnnxOfflineTransducerModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Joiner: PAnsiChar;
  end;
  EdgevoxOnnxOfflineParaformerModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineNemoEncDecCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineDolphinModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineZipformerCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineWenetCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineMedAsrCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineFunAsrNanoModelConfig = record
    EncoderAdaptor: PAnsiChar;
    LLM: PAnsiChar;
    Embedding: PAnsiChar;
    Tokenizer: PAnsiChar;
    SystemPrompt: PAnsiChar;
    UserPrompt: PAnsiChar;
    MaxNewTokens: cint32;
    Temperature: cfloat;
    TopP: cfloat;
    Seed: cint32;
    Language: PAnsiChar;
    UseItn: cint32;
    Hotwords: PAnsiChar;
  end;
  EdgevoxOnnxOfflineFireRedAsrCtcModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineQwen3ASRModelConfig = record
    ConvFrontend: PAnsiChar;
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Tokenizer: PAnsiChar;
    MaxTotalLen: cint32;
    MaxNewTokens: cint32;
    Temperature: cfloat;
    TopP: cfloat;
    Seed: cint32;
    Hotwords: PAnsiChar;
  end;
  EdgevoxOnnxOfflineWhisperModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Language: PAnsiChar;
    Task: PAnsiChar;
    TailPaddings: cint32;
    EnableTokenTimestamps: cint32;
    EnableSegmentTimestamps: cint32;
  end;
  EdgevoxOnnxOfflineCanaryModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    SrcLang: PAnsiChar;
    TgtLang: PAnsiChar;
    UsePnc: cint32;
  end;
  EdgevoxOnnxOfflineCohereTranscribeModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Language: PAnsiChar;
    UsePunct: cint32;
    UseItn: cint32;
  end;
  EdgevoxOnnxOfflineFireRedAsrModelConfig = record
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
  end;
  EdgevoxOnnxOfflineMoonshineModelConfig = record
    Preprocessor: PAnsiChar;
    Encoder: PAnsiChar;
    UncachedDecoder: PAnsiChar;
    CachedDecoder: PAnsiChar;
    MergedDecoder: PAnsiChar;
  end;
  EdgevoxOnnxOfflineTdnnModelConfig = record
    Model: PAnsiChar;
  end;
  EdgevoxOnnxOfflineLMConfig = record
    Model: PAnsiChar;
    Scale: cfloat;
  end;
  EdgevoxOnnxOfflineSenseVoiceModelConfig = record
    Model: PAnsiChar;
    Language: PAnsiChar;
    UseItn: cint32;
  end;
  EdgevoxOnnxOfflineModelConfig = record
    Transducer: EdgevoxOnnxOfflineTransducerModelConfig;
    Paraformer: EdgevoxOnnxOfflineParaformerModelConfig;
    NeMoCtc: EdgevoxOnnxOfflineNemoEncDecCtcModelConfig;
    Whisper: EdgevoxOnnxOfflineWhisperModelConfig;
    Tdnn: EdgevoxOnnxOfflineTdnnModelConfig;
    Tokens: PAnsiChar;
    NumThreads: cint32;
    Debug: cint32;
    Provider: PAnsiChar;
    ModelType: PAnsiChar;
    ModelingUnit: PAnsiChar;
    BpeVocab: PAnsiChar;
    TeleSpeechCtc: PAnsiChar;
    SenseVoice:  EdgevoxOnnxOfflineSenseVoiceModelConfig;
    Moonshine: EdgevoxOnnxOfflineMoonshineModelConfig;
    FireRedAsr: EdgevoxOnnxOfflineFireRedAsrModelConfig;
    Dolphin: EdgevoxOnnxOfflineDolphinModelConfig;
    ZipformerCtc: EdgevoxOnnxOfflineZipformerCtcModelConfig;
    Canary: EdgevoxOnnxOfflineCanaryModelConfig;
    WenetCtc: EdgevoxOnnxOfflineWenetCtcModelConfig;
    Omnilingual: EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig;
    MedAsr: EdgevoxOnnxOfflineMedAsrCtcModelConfig;
    FunAsrNano: EdgevoxOnnxOfflineFunAsrNanoModelConfig;
    FireRedAsrCtc: EdgevoxOnnxOfflineFireRedAsrCtcModelConfig;
    Qwen3Asr: EdgevoxOnnxOfflineQwen3ASRModelConfig;
    CohereTranscribe: EdgevoxOnnxOfflineCohereTranscribeModelConfig;
  end;

  EdgevoxOnnxOfflineRecognizerConfig = record
    FeatConfig: EdgevoxOnnxFeatureConfig;
    ModelConfig: EdgevoxOnnxOfflineModelConfig;
    LMConfig: EdgevoxOnnxOfflineLMConfig;
    DecodingMethod: PAnsiChar;
    MaxActivePaths: cint32;
    HotwordsFile: PAnsiChar;
    HotwordsScore: cfloat;
    RuleFsts: PAnsiChar;
    RuleFars: PAnsiChar;
    BlankPenalty: cfloat;
    Hr: EdgevoxOnnxHomophoneReplacerConfig;
  end;

  PEdgevoxOnnxOfflineRecognizerConfig = ^EdgevoxOnnxOfflineRecognizerConfig;

  EdgevoxOnnxSileroVadModelConfig = record
    Model: PAnsiChar;
    Threshold: cfloat;
    MinSilenceDuration: cfloat;
    MinSpeechDuration: cfloat;
    WindowSize: cint32;
    MaxSpeechDuration: cfloat;
  end;

  EdgevoxOnnxTenVadModelConfig = record
    Model: PAnsiChar;
    Threshold: cfloat;
    MinSilenceDuration: cfloat;
    MinSpeechDuration: cfloat;
    WindowSize: cint32;
    MaxSpeechDuration: cfloat;
  end;

  EdgevoxOnnxVadModelConfig = record
    SileroVad: EdgevoxOnnxSileroVadModelConfig;
    SampleRate: cint32;
    NumThreads: cint32;
    Provider: PAnsiChar;
    Debug: cint32;
    TenVad: EdgevoxOnnxTenVadModelConfig;
  end;
  PEdgevoxOnnxVadModelConfig = ^EdgevoxOnnxVadModelConfig;

  EdgevoxOnnxSpeechSegment = record
    Start: cint32;
    Samples: pcfloat;
    N: cint32;
  end;

  PEdgevoxOnnxSpeechSegment = ^EdgevoxOnnxSpeechSegment;

  EdgevoxOnnxOfflineTtsVitsModelConfig = record
    Model: PAnsiChar;
    Lexicon: PAnsiChar;
    Tokens: PAnsiChar;
    DataDir: PAnsiChar;
    NoiseScale: cfloat;
    NoiseScaleW: cfloat;
    LengthScale: cfloat;
    DictDir: PAnsiChar;
  end;

  PEdgevoxOnnxGenerationConfig = ^EdgevoxOnnxGenerationConfig;

  EdgevoxOnnxGenerationConfig = record
    SilenceScale: cfloat;
    Speed: cfloat;
    Sid: cint32;
    ReferenceAudio: pcfloat;
    ReferenceAudioLen: cint32;
    ReferenceSampleRate: cint32;
    ReferenceText: PAnsiChar;
    NumSteps: cint32;
    Extra: PAnsiChar;
  end;

  EdgevoxOnnxOfflineTtsMatchaModelConfig = record
    AcousticModel: PAnsiChar;
    Vocoder: PAnsiChar;
    Lexicon: PAnsiChar;
    Tokens: PAnsiChar;
    DataDir: PAnsiChar;
    NoiseScale: cfloat;
    LengthScale: cfloat;
    DictDir: PAnsiChar;
  end;

  EdgevoxOnnxOfflineTtsKokoroModelConfig = record
    Model: PAnsiChar;
    Voices: PAnsiChar;
    Tokens: PAnsiChar;
    DataDir: PAnsiChar;
    LengthScale: cfloat;
    DictDir: PAnsiChar;
    Lexicon: PAnsiChar;
    Lang: PAnsiChar;
  end;

  EdgevoxOnnxOfflineTtsKittenModelConfig = record
    Model: PAnsiChar;
    Voices: PAnsiChar;
    Tokens: PAnsiChar;
    DataDir: PAnsiChar;
    LengthScale: cfloat;
  end;

  EdgevoxOnnxOfflineTtsZipVoiceModelConfig = record
    Tokens: PAnsiChar;
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    Vocoder: PAnsiChar;
    DataDir: PAnsiChar;
    Lexicon: PAnsiChar;
    FeatScale: cfloat;
    Tshift: cfloat;
    TargetRms: cfloat;
    GuidanceScale: cfloat;
  end;

  EdgevoxOnnxOfflineTtsPocketModelConfig = record
    LmFlow: PAnsiChar;
    LmMain: PAnsiChar;
    Encoder: PAnsiChar;
    Decoder: PAnsiChar;
    TextConditioner: PAnsiChar;
    VocabJson: PAnsiChar;
    TokenScoresJson: PAnsiChar;
    VoiceEmbeddingCacheCapacity: cint32;
  end;

  EdgevoxOnnxOfflineTtsSupertonicModelConfig = record
    DurationPredictor: PAnsiChar;
    TextEncoder: PAnsiChar;
    VectorEstimator: PAnsiChar;
    Vocoder: PAnsiChar;
    TtsJson: PAnsiChar;
    UnicodeIndexer: PAnsiChar;
    VoiceStyle: PAnsiChar;
  end;

  EdgevoxOnnxOfflineTtsModelConfig = record
    Vits: EdgevoxOnnxOfflineTtsVitsModelConfig;
    NumThreads: cint32;
    Debug: cint32;
    Provider: PAnsiChar;
    Matcha: EdgevoxOnnxOfflineTtsMatchaModelConfig;
    Kokoro: EdgevoxOnnxOfflineTtsKokoroModelConfig;
    Kitten: EdgevoxOnnxOfflineTtsKittenModelConfig;
    ZipVoice: EdgevoxOnnxOfflineTtsZipVoiceModelConfig;
    Pocket: EdgevoxOnnxOfflineTtsPocketModelConfig;
    Supertonic: EdgevoxOnnxOfflineTtsSupertonicModelConfig;
  end;

  EdgevoxOnnxOfflineTtsConfig = record
    Model: EdgevoxOnnxOfflineTtsModelConfig;
    RuleFsts: PAnsiChar;
    MaxNumSentences: cint32;
    RuleFars: PAnsiChar;
    SilenceScale: cfloat;
  end;

  PEdgevoxOnnxOfflineTtsConfig = ^EdgevoxOnnxOfflineTtsConfig;

  EdgevoxOnnxGeneratedAudio = record
    Samples: pcfloat;
    N: cint32;
    SampleRate: cint32;
  end;

  PEdgevoxOnnxGeneratedAudio = ^EdgevoxOnnxGeneratedAudio;

  EdgevoxOnnxResampleOut = record
    Samples: pcfloat;
    N: cint32;
  end;

  PEdgevoxOnnxResampleOut = ^EdgevoxOnnxResampleOut;

  EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOfflineSpeakerSegmentationModelConfig = record
    Pyannote: EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig;
    NumThreads: cint32;
    Debug: cint32;
    Provider: PAnsiChar;
  end;

  EdgevoxOnnxFastClusteringConfig = record
    NumClusters: cint32;
    Threshold: cfloat;
  end;

  EdgevoxOnnxSpeakerEmbeddingExtractorConfig = record
    Model: PAnsiChar;
    NumThreads: cint32;
    Debug: cint32;
    Provider: PAnsiChar;
  end;

  EdgevoxOnnxOfflineSpeakerDiarizationConfig = record
    Segmentation: EdgevoxOnnxOfflineSpeakerSegmentationModelConfig;
    Embedding: EdgevoxOnnxSpeakerEmbeddingExtractorConfig;
    Clustering: EdgevoxOnnxFastClusteringConfig;
    MinDurationOn: cfloat;
    MinDurationOff: cfloat;
  end;

  EdgevoxOnnxOfflineSpeakerDiarizationSegment = record
    Start: cfloat;
    Stop: cfloat;
    Speaker: cint32;
  end;

  PEdgevoxOnnxOfflineSpeakerDiarizationSegment = ^EdgevoxOnnxOfflineSpeakerDiarizationSegment;

  PEdgevoxOnnxOfflineSpeakerDiarizationConfig = ^EdgevoxOnnxOfflineSpeakerDiarizationConfig;

  EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig = record
    Model: PAnsiChar;
  end;

  EdgevoxOnnxOfflineSpeechDenoiserModelConfig = record
    Gtcrn: EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig;
    NumThreads: cint32;
    Debug: cint32;
    Provider: PAnsiChar;
    DpdfNet: EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig;
  end;

  EdgevoxOnnxOfflineSpeechDenoiserConfig = record
    Model: EdgevoxOnnxOfflineSpeechDenoiserModelConfig;
  end;

  PEdgevoxOnnxOfflineSpeechDenoiserConfig = ^EdgevoxOnnxOfflineSpeechDenoiserConfig;

  EdgevoxOnnxOnlineSpeechDenoiserConfig = record
    Model: EdgevoxOnnxOfflineSpeechDenoiserModelConfig;
  end;

  PEdgevoxOnnxOnlineSpeechDenoiserConfig = ^EdgevoxOnnxOnlineSpeechDenoiserConfig;

  EdgevoxOnnxDenoisedAudio = record
    Samples: pcfloat;
    N: cint32;
    SampleRate: cint32;
  end;

  PEdgevoxOnnxDenoisedAudio = ^EdgevoxOnnxDenoisedAudio;

function EdgevoxOnnxCreateLinearResampler(SampleRateInHz: cint32;
  SampleRateOutHz: cint32;
  FilterCutoffHz: cfloat;
  NumZeros: cint32): Pointer; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxGetVersionStrWrapper(): PAnsiChar; cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxGetVersionStr';

function EdgevoxOnnxGetGitSha1Wrapper(): PAnsiChar; cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxGetGitSha1';

function EdgevoxOnnxGetGitDateWrapper(): PAnsiChar; cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxGetGitDate';

function EdgevoxOnnxGetVersionStr(): AnsiString;
begin
  Result := EdgevoxOnnxGetVersionStrWrapper();
end;

function EdgevoxOnnxGetGitSha1(): AnsiString;
begin
  Result := EdgevoxOnnxGetGitSha1Wrapper();
end;

function EdgevoxOnnxGetGitDate(): AnsiString;
begin
  Result := EdgevoxOnnxGetGitDateWrapper();
end;

procedure EdgevoxOnnxDestroyLinearResampler(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxLinearResamplerResample(P: Pointer;
  Samples: pcfloat;
  N: Integer;
  Flush: Integer): PEdgevoxOnnxResampleOut; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxLinearResamplerResampleFree(P: PEdgevoxOnnxResampleOut); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxLinearResamplerReset(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOfflineSpeechDenoiser(Config: PEdgevoxOnnxOfflineSpeechDenoiserConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineSpeechDenoiser(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(P: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeechDenoiserRun(P: Pointer;
  Samples: pcfloat; N: cint32;SampleRate: cint32):PEdgevoxOnnxDenoisedAudio; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOnlineSpeechDenoiser(Config: PEdgevoxOnnxOnlineSpeechDenoiserConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOnlineSpeechDenoiser(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(P: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(P: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOnlineSpeechDenoiserRun(P: Pointer;
  Samples: pcfloat; N: cint32; SampleRate: cint32): PEdgevoxOnnxDenoisedAudio; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOnlineSpeechDenoiserFlush(P: Pointer): PEdgevoxOnnxDenoisedAudio; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOnlineSpeechDenoiserReset(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyDenoisedAudio(Audio: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOfflineSpeakerDiarization(Config: PEdgevoxOnnxOfflineSpeakerDiarizationConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineSpeakerDiarization(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(P: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(P: Pointer; Config: PEdgevoxOnnxOfflineSpeakerDiarizationConfig); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(P: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(P: Pointer): PEdgevoxOnnxOfflineSpeakerDiarizationSegment; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeakerDiarizationProcess(P: Pointer; Samples: pcfloat; N: cint32): Pointer; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg(P: Pointer;
  Samples: pcfloat; N: cint32;  Callback: PEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(P: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOfflineTts(Config: PEdgevoxOnnxOfflineTtsConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineTts(Tts: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineTtsSampleRate(Tts: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineTtsNumSpeakers(Tts: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineTtsGenerate(Tts: Pointer;
  Text: PAnsiChar; Sid: cint32; Speed: cfloat): PEdgevoxOnnxGeneratedAudio; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineTtsGenerateWithCallbackWithArg(Tts: Pointer;
  Text: PAnsiChar; Sid: cint32; Speed: cfloat;
  Callback: TEdgevoxOnnxGeneratedAudioCallbackWithArg;
  Arg: Pointer): PEdgevoxOnnxGeneratedAudio; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOfflineTtsGenerateWithConfig(Tts: Pointer;
  Text: PAnsiChar; config: PEdgevoxOnnxGenerationConfig;
  Callback: TEdgevoxOnnxGeneratedAudioProgressCallbackWithArg;
  Arg: Pointer): PEdgevoxOnnxGeneratedAudio; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(Audio: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateVoiceActivityDetector(Config: PEdgevoxOnnxVadModelConfig;
  BufferSizeInSeconds: cfloat): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyVoiceActivityDetector(Vad: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(Vad: Pointer;
  Samples: pcfloat; N: cint32); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxVoiceActivityDetectorEmpty(Vad: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxVoiceActivityDetectorDetected(Vad: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxVoiceActivityDetectorPop(Vad: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxVoiceActivityDetectorClear(Vad: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxVoiceActivityDetectorFront(Vad: Pointer): PEdgevoxOnnxSpeechSegment; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroySpeechSegment(P: PEdgevoxOnnxSpeechSegment); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxVoiceActivityDetectorReset(P: PEdgevoxOnnxSpeechSegment); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxVoiceActivityDetectorFlush(P: PEdgevoxOnnxSpeechSegment); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateCircularBuffer(Capacity: cint32): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyCircularBuffer(Buffer: Pointer) ; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxCircularBufferPush(Buffer: Pointer; Samples: pcfloat; N: cint32); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCircularBufferGet(Buffer: Pointer; StartIndex: cint32; N: cint32): pcfloat ; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxCircularBufferFree(P: pcfloat); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxCircularBufferPop(Buffer: Pointer; N: cint32); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCircularBufferSize(Buffer: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCircularBufferHead(Buffer: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxCircularBufferReset(Buffer: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOnlineRecognizer(Config: PEdgevoxOnnxOnlineRecognizerConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOnlineRecognizer(Recognizer: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOnlineStream(Recognizer: Pointer): Pointer; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOnlineStreamWithHotwords(Recognizer: Pointer; Hotwords: PAnsiChar): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOnlineStream(Recognizer: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOnlineStreamAcceptWaveform(Stream: Pointer;
  SampleRate: cint32; Samples: pcfloat; N: cint32 ); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOnlineStreamInputFinished(Stream: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxIsOnlineStreamReady(Recognizer: Pointer; Stream: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDecodeOnlineStream(Recognizer: Pointer; Stream: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOnlineStreamReset(Recognizer: Pointer; Stream: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxOnlineStreamIsEndpoint(Recognizer: Pointer; Stream: Pointer): cint32; cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxGetOnlineStreamResultAsJson(Recognizer: Pointer; Stream: Pointer): PAnsiChar; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOnlineStreamResultJson(PJson: PAnsiChar); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOfflineRecognizer(Config: PEdgevoxOnnxOfflineRecognizerConfig): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineRecognizer(Recognizer: Pointer); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxCreateOfflineStream(Recognizer: Pointer): Pointer; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineStream(Stream: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxAcceptWaveformOffline(Stream: Pointer;
  SampleRate: cint32; Samples: pcfloat; N: cint32); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOfflineStreamSetOption(Stream: Pointer; Key: PAnsiChar;
  Value: PAnsiChar); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDecodeOfflineStream(Recognizer: Pointer; Stream: Pointer); cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxOfflineRecognizerSetConfig(Recognizer: Pointer; Config: PEdgevoxOnnxOfflineRecognizerConfig); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxGetOfflineStreamResultAsJson(Stream: Pointer): PAnsiChar; cdecl;
  external EdgevoxOnnxLibName;

procedure EdgevoxOnnxDestroyOfflineStreamResultJson(Json: PAnsiChar); cdecl;
  external EdgevoxOnnxLibName;

function EdgevoxOnnxReadWaveWrapper(Filename: PAnsiChar): PEdgevoxOnnxWave; cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxReadWave';

function EdgevoxOnnxWriteWaveWrapper(Samples: pcfloat; N: cint32;
  SampleRate: cint32; Filename: PAnsiChar): cint32; cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxWriteWave';

procedure EdgevoxOnnxFreeWaveWrapper(P: PEdgevoxOnnxWave); cdecl;
  external EdgevoxOnnxLibName name 'EdgevoxOnnxFreeWave';

function EdgevoxOnnxWriteWave(Filename: AnsiString;
    const Samples: array of Single; SampleRate: Integer): Boolean;
begin
  Result := EdgevoxOnnxWriteWaveWrapper(pcfloat(Samples), Length(Samples),
    SampleRate, PAnsiChar(Filename)) = 1;
end;

function EdgevoxOnnxReadWave(Filename: AnsiString): TEdgevoxOnnxWave;
var
  PWave: PEdgevoxOnnxWave;
begin
  Result.Samples := nil;
  Result.SampleRate := 0;

  PWave := EdgevoxOnnxReadWaveWrapper(PAnsiChar(Filename));

  if PWave = nil then
    Exit;

  Result.SampleRate := PWave^.SampleRate;
  SetLength(Result.Samples, PWave^.NumSamples);

  if PWave^.NumSamples > 0 then
    Move(PWave^.Samples[0], Result.Samples[0], PWave^.NumSamples * SizeOf(Single));

  EdgevoxOnnxFreeWaveWrapper(PWave);
end;

function TEdgevoxOnnxOnlineTransducerModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineTransducerModelConfig(Encoder := %s, Decoder := %s, Joiner := %s)',
  [Self.Encoder, Self.Decoder, Self.Joiner]);
end;

function TEdgevoxOnnxOnlineParaformerModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineParaformerModelConfig(Encoder := %s, Decoder := %s)',
  [Self.Encoder, Self.Decoder]);
end;

function TEdgevoxOnnxOnlineZipformer2CtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineZipformer2CtcModelConfig(Model := %s)',
  [Self.Model]);
end;

function TEdgevoxOnnxOnlineNemoCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineNemoCtcModelConfig(Model := %s)',
  [Self.Model]);
end;

function TEdgevoxOnnxOnlineToneCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineToneCtcModelConfig(Model := %s)',
  [Self.Model]);
end;

function TEdgevoxOnnxOnlineModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineModelConfig(Transducer := %s, ' +
    'Paraformer := %s,' +
    'Zipformer2Ctc := %s, ' +
    'Tokens := %s, ' +
    'NumThreads := %d, ' +
    'Provider := %s, ' +
    'Debug := %s, ' +
    'ModelType := %s, ' +
    'ModelingUnit := %s, ' +
    'BpeVocab := %s, ' +
    'NemoCtc := %s, ' +
    'ToneCtc := %s)',
  [Self.Transducer.ToString, Self.Paraformer.ToString,
   Self.Zipformer2Ctc.ToString, Self.Tokens,
   Self.NumThreads, Self.Provider, Self.Debug.ToString,
   Self.ModelType, Self.ModelingUnit, Self.BpeVocab,
   Self.NemoCtc.ToString, Self.ToneCtc.ToString
  ]);
end;

function TEdgevoxOnnxFeatureConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxFeatureConfig(SampleRate := %d, FeatureDim := %d)',
    [Self.SampleRate, Self.FeatureDim]);
end;

function TEdgevoxOnnxOnlineCtcFstDecoderConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineCtcFstDecoderConfig(Graph := %s, MaxActive := %d)',
  [Self.Graph, Self.MaxActive]);
end;

function TEdgevoxOnnxHomophoneReplacerConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxHomophoneReplacerConfig(Lexicon := %s, RuleFsts := %s)',
  [Self.Lexicon, Self.RuleFsts]);
end;

function TEdgevoxOnnxOnlineRecognizerConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineRecognizerConfig(FeatConfig := %s, ' +
    'ModelConfig := %s, ' +
    'DecodingMethod := %s, ' +
    'MaxActivePaths := %d, ' +
    'EnableEndpoint := %s, ' +
    'Rule1MinTrailingSilence := %.1f, ' +
    'Rule2MinTrailingSilence := %.1f, ' +
    'Rule3MinUtteranceLength := %.1f, ' +
    'HotwordsFile := %s, ' +
    'HotwordsScore := %.1f, ' +
    'CtcFstDecoderConfig := %s, ' +
    'RuleFsts := %s, ' +
    'RuleFars := %s, ' +
    'BlankPenalty := %.1f, ' +
    'Hr := %s' +
    ')'
    ,
    [Self.FeatConfig.ToString, Self.ModelConfig.ToString,
     Self.DecodingMethod, Self.MaxActivePaths, Self.EnableEndpoint.ToString,
     Self.Rule1MinTrailingSilence, Self.Rule2MinTrailingSilence,
     Self.Rule3MinUtteranceLength, Self.HotwordsFile, Self.HotwordsScore,
     Self.CtcFstDecoderConfig.ToString, Self.RuleFsts, Self.RuleFars,
     Self.BlankPenalty, Self.Hr.ToString
    ]);
end;

function TEdgevoxOnnxOnlineRecognizerResult.ToString: AnsiString;
var
  TokensStr: AnsiString;
  S: AnsiString;
  TimestampStr: AnsiString;
  T: Single;
  Sep: AnsiString;
begin
  TokensStr := '[';
  Sep := '';
  for S in Self.Tokens do
  begin
    TokensStr := TokensStr + Sep + S;
    Sep := ', ';
  end;
  TokensStr := TokensStr + ']';

  TimestampStr := '[';
  Sep := '';
  for T in Self.Timestamps do
  begin
    TimestampStr := TimestampStr + Sep + Format('%.2f', [T]);
    Sep := ', ';
  end;
  TimestampStr := TimestampStr + ']';

  Result := Format('TEdgevoxOnnxOnlineRecognizerResult(Text := %s, ' +
    'Tokens := %s, ' +
    'Timestamps := %s' +
    ')',
    [Self.Text, TokensStr, TimestampStr]);
end;

constructor TEdgevoxOnnxOnlineRecognizer.Create(Config: TEdgevoxOnnxOnlineRecognizerConfig);
var
  C: EdgevoxOnnxOnlineRecognizerConfig;
begin
  C := Default(EdgevoxOnnxOnlineRecognizerConfig);
  C.FeatConfig.SampleRate := Config.FeatConfig.SampleRate;
  C.FeatConfig.FeatureDim := Config.FeatConfig.FeatureDim;

  C.ModelConfig.Transducer.Encoder := PAnsiChar(Config.ModelConfig.Transducer.Encoder);
  C.ModelConfig.Transducer.Decoder := PAnsiChar(Config.ModelConfig.Transducer.Decoder);
  C.ModelConfig.Transducer.Joiner := PAnsiChar(Config.ModelConfig.Transducer.Joiner);

  C.ModelConfig.Paraformer.Encoder := PAnsiChar(Config.ModelConfig.Paraformer.Encoder);
  C.ModelConfig.Paraformer.Decoder := PAnsiChar(Config.ModelConfig.Paraformer.Decoder);

  C.ModelConfig.Zipformer2Ctc.Model := PAnsiChar(Config.ModelConfig.Zipformer2Ctc.Model);
  C.ModelConfig.NemoCtc.Model := PAnsiChar(Config.ModelConfig.NemoCtc.Model);
  C.ModelConfig.ToneCtc.Model := PAnsiChar(Config.ModelConfig.ToneCtc.Model);

  C.ModelConfig.Tokens := PAnsiChar(Config.ModelConfig.Tokens);
  C.ModelConfig.NumThreads := Config.ModelConfig.NumThreads;
  C.ModelConfig.Provider := PAnsiChar(Config.ModelConfig.Provider);
  C.ModelConfig.Debug := Ord(Config.ModelConfig.Debug);
  C.ModelConfig.ModelType := PAnsiChar(Config.ModelConfig.ModelType);
  C.ModelConfig.ModelingUnit := PAnsiChar(Config.ModelConfig.ModelingUnit);
  C.ModelConfig.BpeVocab := PAnsiChar(Config.ModelConfig.BpeVocab);

  C.DecodingMethod := PAnsiChar(Config.DecodingMethod);
  C.MaxActivePaths := Config.MaxActivePaths;
  C.EnableEndpoint := Ord(Config.EnableEndpoint);
  C.Rule1MinTrailingSilence := Config.Rule1MinTrailingSilence;
  C.Rule2MinTrailingSilence := Config.Rule2MinTrailingSilence;
  C.Rule3MinUtteranceLength := Config.Rule3MinUtteranceLength;
  C.HotwordsFile := PAnsiChar(Config.HotwordsFile);
  C.HotwordsScore := Config.HotwordsScore;
  C.CtcFstDecoderConfig.Graph := PAnsiChar(Config.CtcFstDecoderConfig.Graph);
  C.CtcFstDecoderConfig.MaxActive := Config.CtcFstDecoderConfig.MaxActive;
  C.RuleFsts := PAnsiChar(Config.RuleFsts);
  C.RuleFars := PAnsiChar(Config.RuleFars);
  C.BlankPenalty := Config.BlankPenalty;
  C.Hr.Lexicon := PAnsiChar(Config.Hr.Lexicon);
  C.Hr.RuleFsts := PAnsiChar(Config.Hr.RuleFsts);

  Self.Handle := EdgevoxOnnxCreateOnlineRecognizer(@C);
  Self._Config := Config;
end;

destructor TEdgevoxOnnxOnlineRecognizer.Destroy;
begin
  EdgevoxOnnxDestroyOnlineRecognizer(Self.Handle);
  Self.Handle := nil;
end;

function TEdgevoxOnnxOnlineRecognizer.CreateStream: TEdgevoxOnnxOnlineStream;
var
  Stream: Pointer;
begin
  Stream := EdgevoxOnnxCreateOnlineStream(Self.Handle);
  Result := TEdgevoxOnnxOnlineStream.Create(Stream);
end;

function TEdgevoxOnnxOnlineRecognizer.CreateStream(Hotwords: AnsiString): TEdgevoxOnnxOnlineStream;
var
  Stream: Pointer;
begin
  Stream := EdgevoxOnnxCreateOnlineStreamWithHotwords(Self.Handle, PAnsiChar(Hotwords));
  Result := TEdgevoxOnnxOnlineStream.Create(Stream);
end;

function TEdgevoxOnnxOnlineRecognizer.IsReady(Stream: TEdgevoxOnnxOnlineStream): Boolean;
begin
  Result := EdgevoxOnnxIsOnlineStreamReady(Self.Handle, Stream.Handle) = 1;
end;

procedure TEdgevoxOnnxOnlineRecognizer.Decode(Stream: TEdgevoxOnnxOnlineStream);
begin
  EdgevoxOnnxDecodeOnlineStream(Self.Handle, Stream.Handle);
end;

procedure TEdgevoxOnnxOnlineRecognizer.Reset(Stream: TEdgevoxOnnxOnlineStream);
begin
  EdgevoxOnnxOnlineStreamReset(Self.Handle, Stream.Handle);
end;

function TEdgevoxOnnxOnlineRecognizer.IsEndpoint(Stream: TEdgevoxOnnxOnlineStream): Boolean;
begin
  Result := EdgevoxOnnxOnlineStreamIsEndpoint(Self.Handle, Stream.Handle) = 1;
end;

function TEdgevoxOnnxOnlineRecognizer.GetResult(Stream: TEdgevoxOnnxOnlineStream): TEdgevoxOnnxOnlineRecognizerResult;
var
  pJson: PAnsiChar;
  JsonData: TJSONData;
  JsonObject : TJSONObject;
  JsonEnum: TJSONEnum;
  I: Integer;
begin
  pJson := EdgevoxOnnxGetOnlineStreamResultAsJson(Self.Handle, Stream.Handle);

  {
   - https://www.freepascal.org/daily/doc/fcl/fpjson/getjson.html
   - https://www.freepascal.org/daily/doc/fcl/fpjson/tjsondata.html
   - https://www.freepascal.org/daily/doc/fcl/fpjson/tjsonobject.html
   - https://www.freepascal.org/daily/doc/fcl/fpjson/tjsonenum.html
  }

  JsonData := GetJSON(AnsiString(pJson), False);

  JsonObject := JsonData as TJSONObject;

  Result.Text := JsonObject.Strings['text'];

  SetLength(Result.Tokens, JsonObject.Arrays['tokens'].Count);

  I := 0;
  for JsonEnum in JsonObject.Arrays['tokens'] do
  begin
    Result.Tokens[I] := JsonEnum.Value.AsString;
    Inc(I);
  end;

  SetLength(Result.Timestamps, JsonObject.Arrays['timestamps'].Count);
  I := 0;
  for JsonEnum in JsonObject.Arrays['timestamps'] do
  begin
    Result.Timestamps[I] := JsonEnum.Value.AsFloat;
    Inc(I);
  end;

  EdgevoxOnnxDestroyOnlineStreamResultJson(pJson);
end;


constructor TEdgevoxOnnxOnlineStream.Create(P: Pointer);
begin
  Self.Handle := P;
end;

destructor TEdgevoxOnnxOnlineStream.Destroy;
begin
  EdgevoxOnnxDestroyOnlineStream(Self.Handle);
  Self.Handle := nil;
end;

procedure TEdgevoxOnnxOnlineStream.AcceptWaveform(const Samples: array of Single; SampleRate: Integer);
begin
  EdgevoxOnnxOnlineStreamAcceptWaveform(Self.Handle, SampleRate,
    pcfloat(Samples), Length(Samples));
end;

procedure TEdgevoxOnnxOnlineStream.InputFinished;
begin
  EdgevoxOnnxOnlineStreamInputFinished(Self.Handle);
end;

function TEdgevoxOnnxOfflineTransducerModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTransducerModelConfig(' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'Joiner := %s' +
    ')',
    [Self.Encoder, Self.Decoder, Self.Joiner]);
end;

function TEdgevoxOnnxOfflineParaformerModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineParaformerModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineNemoEncDecCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineNemoEncDecCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineDolphinModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineDolphinModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineZipformerCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineZipformerCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineWenetCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineWenetCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineMedAsrCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineMedAsrCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineFireRedAsrCtcModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineFireRedAsrCtcModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineQwen3ASRModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineQwen3ASRModelConfig(' +
    'ConvFrontend := %s' +
    ', Encoder := %s' +
    ', Decoder := %s' +
    ', Tokenizer := %s' +
    ', MaxTotalLen := %d' +
    ', MaxNewTokens := %d' +
    ', Temperature := %.3f' +
    ', TopP := %.3f' +
    ', Seed := %d' +
    ', Hotwords := %s' +
    ')',
    [Self.ConvFrontend, Self.Encoder, Self.Decoder, Self.Tokenizer,
     Self.MaxTotalLen, Self.MaxNewTokens, Self.Temperature,
     Self.TopP, Self.Seed, Self.Hotwords]);
end;

function TEdgevoxOnnxOfflineFunAsrNanoModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineFunAsrNanoModelConfig(' +
    'EncoderAdaptor := %s' +
    ', LLM := %s' +
    ', Embedding := %s' +
    ', Tokenizer := %s' +
    ', SystemPrompt := %s' +
    ', UserPrompt := %s' +
    ', MaxNewTokens := %d' +
    ', Temperature := %.3f' +
    ', TopP := %.3f' +
    ', Seed := %d' +
    ', Language := %s' +
    ', UseItn := %s' +
    ', Hotwords := %s' +
    ')',
    [Self.EncoderAdaptor, Self.LLM, Self.Embedding, Self.Tokenizer,
     Self.SystemPrompt, Self.UserPrompt, Self.MaxNewTokens, Self.Temperature,
     Self.TopP, Self.Seed, Self.Language, Self.UseItn.ToString, Self.Hotwords]);
end;

function TEdgevoxOnnxOfflineWhisperModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineWhisperModelConfig(' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'Language := %s, ' +
    'Task := %s, ' +
    'TailPaddings := %d, ' +
    'EnableTokenTimestamps := %s, ' +
    'EnableSegmentTimestamps := %s' +
    ')',
    [Self.Encoder, Self.Decoder, Self.Language, Self.Task, Self.TailPaddings,
     Self.EnableTokenTimestamps.ToString,
     Self.EnableSegmentTimestamps.ToString]);
end;

function TEdgevoxOnnxOfflineCanaryModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineCanaryModelConfig(' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'SrcLang := %s, ' +
    'TgtLang := %s, ' +
    'UsePnc := %s' +
    ')',
    [Self.Encoder, Self.Decoder, Self.SrcLang,
     Self.TgtLang, Self.UsePnc.ToString]);
end;

function TEdgevoxOnnxOfflineCohereTranscribeModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineCohereTranscribeModelConfig(' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'Language := %s, ' +
    'UsePunct := %s, ' +
    'UseItn := %s' +
    ')',
    [Self.Encoder, Self.Decoder, Self.Language, Self.UsePunct.ToString,
     Self.UseItn.ToString]);
end;

function TEdgevoxOnnxOfflineFireRedAsrModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineFireRedAsrModelConfig(' +
    'Encoder := %s, ' +
    'Decoder := %s)',
    [Self.Encoder, Self.Decoder]);
end;

function TEdgevoxOnnxOfflineMoonshineModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineMoonshineModelConfig(' +
    'Preprocessor := %s, ' +
    'Encoder := %s, ' +
    'UncachedDecoder := %s, ' +
    'CachedDecoder := %s, ' +
    'MergedDecoder := %s)',
    [Self.Preprocessor, Self.Encoder, Self.UncachedDecoder, Self.CachedDecoder,
     Self.MergedDecoder]);
end;

function TEdgevoxOnnxOfflineTdnnModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTdnnModelConfig(Model := %s)',
    [Self.Model]);
end;

function TEdgevoxOnnxOfflineLMConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineLMConfig(' +
    'Model := %s, ' +
    'Scale := %.1f' +
    ')',
    [Self.Model, Self.Scale]);
end;

function TEdgevoxOnnxOfflineSenseVoiceModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSenseVoiceModelConfig(' +
    'Model := %s, ' +
    'Language := %s, ' +
    'UseItn := %s' +
    ')',
    [Self.Model, Self.Language, Self.UseItn.ToString]);
end;

function TEdgevoxOnnxOfflineModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineModelConfig(' +
    'Transducer := %s, ' +
    'Paraformer := %s, ' +
    'NeMoCtc := %s, ' +
    'Whisper := %s, ' +
    'Tdnn := %s, ' +
    'Tokens := %s, ' +
    'NumThreads := %d, ' +
    'Debug := %s, ' +
    'Provider := %s, ' +
    'ModelType := %s, ' +
    'ModelingUnit := %s, ' +
    'BpeVocab := %s, ' +
    'TeleSpeechCtc := %s, ' +
    'SenseVoice := %s, ' +
    'Moonshine := %s, ' +
    'FireRedAsr := %s, ' +
    'Dolphin := %s, ' +
    'ZipformerCtc := %s, ' +
    'Canary := %s, ' +
    'WenetCtc := %s, ' +
     'Omnilingual := %s' +
     ', MedAsr := %s' +
     ', FunAsrNano := %s' +
     ', FireRedAsrCtc := %s' +
     ', Qwen3Asr := %s' +
     ', CohereTranscribe := %s' +
     ')',
    [Self.Transducer.ToString, Self.Paraformer.ToString,
     Self.NeMoCtc.ToString, Self.Whisper.ToString, Self.Tdnn.ToString,
     Self.Tokens, Self.NumThreads, Self.Debug.ToString, Self.Provider,
     Self.ModelType, Self.ModelingUnit, Self.BpeVocab,
     Self.TeleSpeechCtc, Self.SenseVoice.ToString, Self.Moonshine.ToString,
     Self.FireRedAsr.ToString, Self.Dolphin.ToString,
     Self.ZipformerCtc.ToString, Self.Canary.ToString, Self.WenetCtc.ToString,
     Self.Omnilingual.ToString, Self.MedAsr.ToString,
     Self.FunAsrNano.ToString, Self.FireRedAsrCtc.ToString,
     Self.Qwen3Asr.ToString, Self.CohereTranscribe.ToString
      ]);
end;

function TEdgevoxOnnxOfflineRecognizerConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineRecognizerConfig(' +
    'FeatConfig := %s, ' +
    'ModelConfig := %s, ' +
    'LMConfig := %s, ' +
    'DecodingMethod := %s, ' +
    'MaxActivePaths := %d, ' +
    'HotwordsFile := %s, ' +
    'HotwordsScore := %.1f, ' +
    'RuleFsts := %s, ' +
    'RuleFars := %s, ' +
    'BlankPenalty := %1.f, ' +
    'Hr := %s' +
    ')',
    [Self.FeatConfig.ToString, Self.ModelConfig.ToString,
     Self.LMConfig.ToString, Self.DecodingMethod, Self.MaxActivePaths,
     Self.HotwordsFile, Self.HotwordsScore, Self.RuleFsts, Self.RuleFars,
     Self.BlankPenalty, Self.Hr.ToString
     ]);
end;

function ConvertOfflineRecognizerConfig(Config: TEdgevoxOnnxOfflineRecognizerConfig): EdgevoxOnnxOfflineRecognizerConfig;
var
  C: EdgevoxOnnxOfflineRecognizerConfig;
begin
  C := Default(EdgevoxOnnxOfflineRecognizerConfig);
  C.FeatConfig.SampleRate := Config.FeatConfig.SampleRate;
  C.FeatConfig.FeatureDim := Config.FeatConfig.FeatureDim;

  C.ModelConfig.Transducer.Encoder := PAnsiChar(Config.ModelConfig.Transducer.Encoder);
  C.ModelConfig.Transducer.Decoder := PAnsiChar(Config.ModelConfig.Transducer.Decoder);
  C.ModelConfig.Transducer.Joiner := PAnsiChar(Config.ModelConfig.Transducer.Joiner);

  C.ModelConfig.Paraformer.Model := PAnsiChar(Config.ModelConfig.Paraformer.Model);
  C.ModelConfig.NeMoCtc.Model := PAnsiChar(Config.ModelConfig.NeMoCtc.Model);

  C.ModelConfig.Whisper.Encoder := PAnsiChar(Config.ModelConfig.Whisper.Encoder);
  C.ModelConfig.Whisper.Decoder := PAnsiChar(Config.ModelConfig.Whisper.Decoder);
  C.ModelConfig.Whisper.Language := PAnsiChar(Config.ModelConfig.Whisper.Language);
  C.ModelConfig.Whisper.Task := PAnsiChar(Config.ModelConfig.Whisper.Task);
  C.ModelConfig.Whisper.TailPaddings := Config.ModelConfig.Whisper.TailPaddings;
  C.ModelConfig.Whisper.EnableTokenTimestamps := Ord(Config.ModelConfig.Whisper.EnableTokenTimestamps);
  C.ModelConfig.Whisper.EnableSegmentTimestamps := Ord(Config.ModelConfig.Whisper.EnableSegmentTimestamps);

  C.ModelConfig.Tdnn.Model := PAnsiChar(Config.ModelConfig.Tdnn.Model);

  C.ModelConfig.Tokens := PAnsiChar(Config.ModelConfig.Tokens);
  C.ModelConfig.NumThreads := Config.ModelConfig.NumThreads;
  C.ModelConfig.Debug := Ord(Config.ModelConfig.Debug);
  C.ModelConfig.Provider := PAnsiChar(Config.ModelConfig.Provider);
  C.ModelConfig.ModelType := PAnsiChar(Config.ModelConfig.ModelType);
  C.ModelConfig.ModelingUnit := PAnsiChar(Config.ModelConfig.ModelingUnit);
  C.ModelConfig.BpeVocab := PAnsiChar(Config.ModelConfig.BpeVocab);
  C.ModelConfig.TeleSpeechCtc := PAnsiChar(Config.ModelConfig.TeleSpeechCtc);

  C.ModelConfig.SenseVoice.Model := PAnsiChar(Config.ModelConfig.SenseVoice.Model);
  C.ModelConfig.SenseVoice.Language := PAnsiChar(Config.ModelConfig.SenseVoice.Language);
  C.ModelConfig.SenseVoice.UseItn := Ord(Config.ModelConfig.SenseVoice.UseItn);

  C.ModelConfig.Moonshine.Preprocessor := PAnsiChar(Config.ModelConfig.Moonshine.Preprocessor);
  C.ModelConfig.Moonshine.Encoder := PAnsiChar(Config.ModelConfig.Moonshine.Encoder);
  C.ModelConfig.Moonshine.UncachedDecoder := PAnsiChar(Config.ModelConfig.Moonshine.UncachedDecoder);
  C.ModelConfig.Moonshine.CachedDecoder := PAnsiChar(Config.ModelConfig.Moonshine.CachedDecoder);
  C.ModelConfig.Moonshine.MergedDecoder := PAnsiChar(Config.ModelConfig.Moonshine.MergedDecoder);

  C.ModelConfig.FireRedAsr.Encoder := PAnsiChar(Config.ModelConfig.FireRedAsr.Encoder);
  C.ModelConfig.FireRedAsr.Decoder := PAnsiChar(Config.ModelConfig.FireRedAsr.Decoder);

  C.ModelConfig.Dolphin.Model := PAnsiChar(Config.ModelConfig.Dolphin.Model);
  C.ModelConfig.ZipformerCtc.Model := PAnsiChar(Config.ModelConfig.ZipformerCtc.Model);

  C.ModelConfig.Canary.Encoder := PAnsiChar(Config.ModelConfig.Canary.Encoder);
  C.ModelConfig.Canary.Decoder := PAnsiChar(Config.ModelConfig.Canary.Decoder);
  C.ModelConfig.Canary.SrcLang := PAnsiChar(Config.ModelConfig.Canary.SrcLang);
  C.ModelConfig.Canary.TgtLang := PAnsiChar(Config.ModelConfig.Canary.TgtLang);
  C.ModelConfig.Canary.UsePnc := Ord(Config.ModelConfig.Canary.UsePnc);

  C.ModelConfig.WenetCtc.Model := PAnsiChar(Config.ModelConfig.WenetCtc.Model);
  C.ModelConfig.Omnilingual.Model := PAnsiChar(Config.ModelConfig.Omnilingual.Model);
  C.ModelConfig.MedAsr.Model := PAnsiChar(Config.ModelConfig.MedAsr.Model);

  C.ModelConfig.FunAsrNano.EncoderAdaptor := PAnsiChar(Config.ModelConfig.FunAsrNano.EncoderAdaptor);
  C.ModelConfig.FunAsrNano.LLM := PAnsiChar(Config.ModelConfig.FunAsrNano.LLM);
  C.ModelConfig.FunAsrNano.Embedding := PAnsiChar(Config.ModelConfig.FunAsrNano.Embedding);
  C.ModelConfig.FunAsrNano.Tokenizer := PAnsiChar(Config.ModelConfig.FunAsrNano.Tokenizer);
  C.ModelConfig.FunAsrNano.SystemPrompt := PAnsiChar(Config.ModelConfig.FunAsrNano.SystemPrompt);
  C.ModelConfig.FunAsrNano.UserPrompt := PAnsiChar(Config.ModelConfig.FunAsrNano.UserPrompt);
  C.ModelConfig.FunAsrNano.MaxNewTokens := Config.ModelConfig.FunAsrNano.MaxNewTokens;
  C.ModelConfig.FunAsrNano.Temperature := Config.ModelConfig.FunAsrNano.Temperature;
  C.ModelConfig.FunAsrNano.TopP := Config.ModelConfig.FunAsrNano.TopP;
  C.ModelConfig.FunAsrNano.Seed := Config.ModelConfig.FunAsrNano.Seed;
  C.ModelConfig.FunAsrNano.Language := PAnsiChar(Config.ModelConfig.FunAsrNano.Language);
  C.ModelConfig.FunAsrNano.UseItn := Ord(Config.ModelConfig.FunAsrNano.UseItn);
  C.ModelConfig.FunAsrNano.Hotwords := PAnsiChar(Config.ModelConfig.FunAsrNano.Hotwords);

  C.ModelConfig.FireRedAsrCtc.Model := PAnsiChar(Config.ModelConfig.FireRedAsrCtc.Model);

  C.ModelConfig.Qwen3Asr.ConvFrontend := PAnsiChar(Config.ModelConfig.Qwen3Asr.ConvFrontend);
  C.ModelConfig.Qwen3Asr.Encoder := PAnsiChar(Config.ModelConfig.Qwen3Asr.Encoder);
  C.ModelConfig.Qwen3Asr.Decoder := PAnsiChar(Config.ModelConfig.Qwen3Asr.Decoder);
  C.ModelConfig.Qwen3Asr.Tokenizer := PAnsiChar(Config.ModelConfig.Qwen3Asr.Tokenizer);
  C.ModelConfig.Qwen3Asr.MaxTotalLen := Config.ModelConfig.Qwen3Asr.MaxTotalLen;
  C.ModelConfig.Qwen3Asr.MaxNewTokens := Config.ModelConfig.Qwen3Asr.MaxNewTokens;
  C.ModelConfig.Qwen3Asr.Temperature := Config.ModelConfig.Qwen3Asr.Temperature;
  C.ModelConfig.Qwen3Asr.TopP := Config.ModelConfig.Qwen3Asr.TopP;
  C.ModelConfig.Qwen3Asr.Seed := Config.ModelConfig.Qwen3Asr.Seed;
  C.ModelConfig.Qwen3Asr.Hotwords := PAnsiChar(Config.ModelConfig.Qwen3Asr.Hotwords);

  C.ModelConfig.CohereTranscribe.Encoder := PAnsiChar(Config.ModelConfig.CohereTranscribe.Encoder);
  C.ModelConfig.CohereTranscribe.Decoder := PAnsiChar(Config.ModelConfig.CohereTranscribe.Decoder);
  C.ModelConfig.CohereTranscribe.Language := PAnsiChar(Config.ModelConfig.CohereTranscribe.Language);
  C.ModelConfig.CohereTranscribe.UsePunct := Ord(Config.ModelConfig.CohereTranscribe.UsePunct);
  C.ModelConfig.CohereTranscribe.UseItn := Ord(Config.ModelConfig.CohereTranscribe.UseItn);

  C.LMConfig.Model := PAnsiChar(Config.LMConfig.Model);
  C.LMConfig.Scale := Config.LMConfig.Scale;

  C.DecodingMethod := PAnsiChar(Config.DecodingMethod);
  C.MaxActivePaths := Config.MaxActivePaths;
  C.HotwordsFile := PAnsiChar(Config.HotwordsFile);
  C.HotwordsScore := Config.HotwordsScore;
  C.RuleFsts := PAnsiChar(Config.RuleFsts);
  C.RuleFars := PAnsiChar(Config.RuleFars);
  C.BlankPenalty := Config.BlankPenalty;

  C.Hr.Lexicon := PAnsiChar(Config.Hr.Lexicon);
  C.Hr.RuleFsts := PAnsiChar(Config.Hr.RuleFsts);

  Result := C;
end;

constructor TEdgevoxOnnxOfflineRecognizer.Create(Config: TEdgevoxOnnxOfflineRecognizerConfig);
var
  C: EdgevoxOnnxOfflineRecognizerConfig;
begin
  C := ConvertOfflineRecognizerConfig(Config);
  Self.Handle := EdgevoxOnnxCreateOfflineRecognizer(@C);
  Self._Config := Config;
end;

procedure TEdgevoxOnnxOfflineRecognizer.SetConfig(Config: TEdgevoxOnnxOfflineRecognizerConfig);
var
  C: EdgevoxOnnxOfflineRecognizerConfig;
begin
  C := ConvertOfflineRecognizerConfig(Config);
  EdgevoxOnnxOfflineRecognizerSetConfig(Self.Handle, @C);
  { We don't update Self._Config }
end;

destructor TEdgevoxOnnxOfflineRecognizer.Destroy;
begin
  EdgevoxOnnxDestroyOfflineRecognizer(Self.Handle);
  Self.Handle := nil;
end;

function TEdgevoxOnnxOfflineRecognizer.CreateStream: TEdgevoxOnnxOfflineStream;
var
  Stream: Pointer;
begin
  Stream := EdgevoxOnnxCreateOfflineStream(Self.Handle);
  Result := TEdgevoxOnnxOfflineStream.Create(Stream);
end;

procedure TEdgevoxOnnxOfflineRecognizer.Decode(Stream: TEdgevoxOnnxOfflineStream);
begin
  EdgevoxOnnxDecodeOfflineStream(Self.Handle, Stream.Handle);
end;

function TEdgevoxOnnxOfflineRecognizer.GetResult(Stream: TEdgevoxOnnxOfflineStream): TEdgevoxOnnxOfflineRecognizerResult;
var
  pJson: PAnsiChar;
  JsonData: TJSONData;
  JsonObject : TJSONObject;
  JsonEnum: TJSONEnum;
  I: Integer;
begin
  pJson := EdgevoxOnnxGetOfflineStreamResultAsJson(Stream.Handle);

  JsonData := GetJSON(AnsiString(pJson), False);

  JsonObject := JsonData as TJSONObject;

  Result.Text := JsonObject.Strings['text'];

  SetLength(Result.Tokens, JsonObject.Arrays['tokens'].Count);

  I := 0;
  for JsonEnum in JsonObject.Arrays['tokens'] do
  begin
    Result.Tokens[I] := JsonEnum.Value.AsString;
    Inc(I);
  end;

  SetLength(Result.Timestamps, JsonObject.Arrays['timestamps'].Count);
  I := 0;
  for JsonEnum in JsonObject.Arrays['timestamps'] do
  begin
    Result.Timestamps[I] := JsonEnum.Value.AsFloat;
    Inc(I);
  end;

  EdgevoxOnnxDestroyOfflineStreamResultJson(pJson);
end;

constructor TEdgevoxOnnxOfflineStream.Create(P: Pointer);
begin
  Self.Handle := P;
end;

destructor TEdgevoxOnnxOfflineStream.Destroy;
begin
  EdgevoxOnnxDestroyOfflineStream(Self.Handle);
  Self.Handle := nil;
end;

procedure TEdgevoxOnnxOfflineStream.AcceptWaveform(const Samples: array of Single; SampleRate: Integer);
begin
  EdgevoxOnnxAcceptWaveformOffline(Self.Handle, SampleRate, pcfloat(Samples),
    Length(Samples));
end;

procedure TEdgevoxOnnxOfflineStream.SetOption(const Key: AnsiString;
  const Value: AnsiString);
begin
  EdgevoxOnnxOfflineStreamSetOption(Self.Handle, PAnsiChar(Key),
    PAnsiChar(Value));
end;

function TEdgevoxOnnxOfflineRecognizerResult.ToString: AnsiString;
var
  TokensStr: AnsiString;
  S: AnsiString;
  TimestampStr: AnsiString;
  T: Single;
  Sep: AnsiString;
begin
  TokensStr := '[';
  Sep := '';
  for S in Self.Tokens do
  begin
    TokensStr := TokensStr + Sep + S;
    Sep := ', ';
  end;
  TokensStr := TokensStr + ']';

  TimestampStr := '[';
  Sep := '';
  for T in Self.Timestamps do
  begin
    TimestampStr := TimestampStr + Sep + Format('%.2f', [T]);
    Sep := ', ';
  end;
  TimestampStr := TimestampStr + ']';

  Result := Format('TEdgevoxOnnxOfflineRecognizerResult(Text := %s, ' +
    'Tokens := %s, ' +
    'Timestamps := %s' +
    ')',
    [Self.Text, TokensStr, TimestampStr]);
end;

function TEdgevoxOnnxSileroVadModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxSileroVadModelConfig(' +
    'Model := %s, ' +
    'Threshold := %.2f, ' +
    'MinSilenceDuration := %.2f, ' +
    'MinSpeechDuration := %.2f, ' +
    'WindowSize := %d, ' +
    'MaxSpeechDuration := %.2f' +
    ')',
    [Self.Model, Self.Threshold, Self.MinSilenceDuration,
     Self.MinSpeechDuration, Self.WindowSize, Self.MaxSpeechDuration
    ]);
end;

function TEdgevoxOnnxTenVadModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxTenVadModelConfig(' +
    'Model := %s, ' +
    'Threshold := %.2f, ' +
    'MinSilenceDuration := %.2f, ' +
    'MinSpeechDuration := %.2f, ' +
    'WindowSize := %d, ' +
    'MaxSpeechDuration := %.2f' +
    ')',
    [Self.Model, Self.Threshold, Self.MinSilenceDuration,
     Self.MinSpeechDuration, Self.WindowSize, Self.MaxSpeechDuration
    ]);
end;

class operator TEdgevoxOnnxOfflineFunAsrNanoModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineFunAsrNanoModelConfig);
begin
  Dest.MaxNewTokens := 512;
  Dest.Temperature := 1e-6;
  Dest.TopP := 0.8;
  Dest.Seed := 42;
  Dest.UseItn := False;
end;

class operator TEdgevoxOnnxOfflineQwen3ASRModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineQwen3ASRModelConfig);
begin
  Dest.MaxTotalLen := 512;
  Dest.MaxNewTokens := 128;
  Dest.Temperature := 1e-6;
  Dest.TopP := 0.8;
  Dest.Seed := 42;
end;

class operator TEdgevoxOnnxSileroVadModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxSileroVadModelConfig);
begin
  Dest.Threshold := 0.5;
  Dest.MinSilenceDuration := 0.5;
  Dest.MinSpeechDuration := 0.25;
  Dest.WindowSize := 512;
  Dest.MaxSpeechDuration := 5.0;
end;

class operator TEdgevoxOnnxTenVadModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxTenVadModelConfig);
begin
  Dest.Threshold := 0.5;
  Dest.MinSilenceDuration := 0.5;
  Dest.MinSpeechDuration := 0.25;
  Dest.WindowSize := 256;
  Dest.MaxSpeechDuration := 5.0;
end;

function TEdgevoxOnnxVadModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxVadModelConfig(' +
    'SileroVad := %s, ' +
    'SampleRate := %d, ' +
    'NumThreads := %d, ' +
    'Provider := %s, ' +
    'Debug := %s, ' +
    'TenVad := %s' +
    ')',
    [Self.SileroVad.ToString, Self.SampleRate, Self.NumThreads, Self.Provider,
     Self.Debug.ToString, Self.TenVad.ToString
    ]);
end;

class operator TEdgevoxOnnxVadModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxVadModelConfig);
begin
  Dest.SampleRate := 16000;
  Dest.NumThreads := 1;
  Dest.Provider := 'cpu';
  Dest.Debug := False;
end;

class operator TEdgevoxOnnxFeatureConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxFeatureConfig);
begin
  Dest.SampleRate := 16000;
  Dest.FeatureDim := 80;
end;

class operator TEdgevoxOnnxOnlineCtcFstDecoderConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineCtcFstDecoderConfig);
begin
  Dest.MaxActive := 3000;
end;

class operator TEdgevoxOnnxOnlineRecognizerConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineRecognizerConfig);
begin
  Dest.DecodingMethod := 'greedy_search';
  Dest.EnableEndpoint := False;
  Dest.Rule1MinTrailingSilence := 2.4;
  Dest.Rule2MinTrailingSilence := 1.2;
  Dest.Rule3MinUtteranceLength := 20;
  Dest.HotwordsScore := 1.5;
  Dest.BlankPenalty := 0;
end;

class operator TEdgevoxOnnxOnlineModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOnlineModelConfig);
begin
  Dest.NumThreads := 1;
  Dest.Provider := 'cpu';
  Dest.Debug := False;
end;

class operator TEdgevoxOnnxOfflineWhisperModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineWhisperModelConfig);
begin
  Dest.Task := 'transcribe';
  Dest.TailPaddings := -1;
  Dest.EnableTokenTimestamps := False;
  Dest.EnableSegmentTimestamps := False;
end;

class operator TEdgevoxOnnxOfflineCanaryModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineCanaryModelConfig);
begin
  Dest.SrcLang := 'en';
  Dest.TgtLang := 'en';
  Dest.UsePnc := True;
end;

class operator TEdgevoxOnnxOfflineCohereTranscribeModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineCohereTranscribeModelConfig);
begin
  Dest.Language := '';
  Dest.UsePunct := True;
  Dest.UseItn := True;
end;

class operator TEdgevoxOnnxOfflineLMConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineLMConfig);
begin
  Dest.Scale := 1.0;
end;

class operator TEdgevoxOnnxOfflineSenseVoiceModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSenseVoiceModelConfig);
begin
  Dest.UseItn := True;
end;

class operator TEdgevoxOnnxOfflineModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineModelConfig);
begin
  Dest.NumThreads := 1;
  Dest.Debug := False;
  Dest.Provider := 'cpu';
end;

class operator TEdgevoxOnnxOfflineRecognizerConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineRecognizerConfig);
begin
  Dest.DecodingMethod := 'greedy_search';
  Dest.MaxActivePaths := 4;
  Dest.HotwordsScore := 1.5;
  Dest.BlankPenalty := 0;
end;

constructor TEdgevoxOnnxCircularBuffer.Create(Capacity: Integer);
begin
  Self.Handle := EdgevoxOnnxCreateCircularBuffer(Capacity);
end;

destructor TEdgevoxOnnxCircularBuffer.Destroy;
begin
  EdgevoxOnnxDestroyCircularBuffer(Self.Handle);
  Self.Handle := nil;
end;

procedure TEdgevoxOnnxCircularBuffer.Push(Samples: array of Single);
begin
  EdgevoxOnnxCircularBufferPush(Self.Handle, pcfloat(Samples), Length(Samples));
end;

procedure TEdgevoxOnnxCircularBuffer.Push(Samples: pcfloat; N: Integer);
begin
  EdgevoxOnnxCircularBufferPush(Self.Handle, Samples, N);
end;

function TEdgevoxOnnxCircularBuffer.Get(StartIndex: Integer; N: Integer): TEdgevoxOnnxSamplesArray;
var
  P: pcfloat;
begin
  Result := nil;

  if N <= 0 then
    Exit;

  P := EdgevoxOnnxCircularBufferGet(Self.Handle, StartIndex, N);
  if P = nil then
    Exit;

  SetLength(Result, N);

  Move(P[0], Result[0], N * SizeOf(Single));

  EdgevoxOnnxCircularBufferFree(P);
end;

procedure TEdgevoxOnnxCircularBuffer.Pop(N: Integer);
begin
  EdgevoxOnnxCircularBufferPop(Self.Handle, N);
end;

procedure TEdgevoxOnnxCircularBuffer.Reset;
begin
  EdgevoxOnnxCircularBufferReset(Self.Handle);
end;

function TEdgevoxOnnxCircularBuffer.Size: Integer;
begin
  Result := EdgevoxOnnxCircularBufferSize(Self.Handle);
end;

function TEdgevoxOnnxCircularBuffer.Head: Integer;
begin
  Result := EdgevoxOnnxCircularBufferHead(Self.Handle);
end;

constructor TEdgevoxOnnxVoiceActivityDetector.Create(Config: TEdgevoxOnnxVadModelConfig; BufferSizeInSeconds: Single);
var
  C: EdgevoxOnnxVadModelConfig ;
begin
  C := Default(EdgevoxOnnxVadModelConfig);
  Self._Config := Config;

  C.SileroVad.Model := PAnsiChar(Config.SileroVad.Model);
  C.SileroVad.Threshold := Config.SileroVad.Threshold;
  C.SileroVad.MinSilenceDuration := Config.SileroVad.MinSilenceDuration;
  C.SileroVad.MinSpeechDuration := Config.SileroVad.MinSpeechDuration;
  C.SileroVad.WindowSize := Config.SileroVad.WindowSize;
  C.SileroVad.MaxSpeechDuration := Config.SileroVad.MaxSpeechDuration;

  C.TenVad.Model := PAnsiChar(Config.TenVad.Model);
  C.TenVad.Threshold := Config.TenVad.Threshold;
  C.TenVad.MinSilenceDuration := Config.TenVad.MinSilenceDuration;
  C.TenVad.MinSpeechDuration := Config.TenVad.MinSpeechDuration;
  C.TenVad.WindowSize := Config.TenVad.WindowSize;
  C.TenVad.MaxSpeechDuration := Config.TenVad.MaxSpeechDuration;

  C.SampleRate := Config.SampleRate;
  C.NumThreads := Config.NumThreads;
  C.Provider := PAnsiChar(Config.Provider);
  C.Debug := Ord(Config.Debug);

  Self.Handle := EdgevoxOnnxCreateVoiceActivityDetector(@C, BufferSizeInSeconds);
end;

destructor TEdgevoxOnnxVoiceActivityDetector.Destroy;
begin
  EdgevoxOnnxDestroyVoiceActivityDetector(Self.Handle);
  Self.Handle := nil;
end;

procedure TEdgevoxOnnxVoiceActivityDetector.AcceptWaveform(const Samples: array of Single);
begin
  EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(Self.Handle, pcfloat(Samples), Length(Samples));
end;

procedure TEdgevoxOnnxVoiceActivityDetector.AcceptWaveform(const Samples: array of Single; Offset: Integer; N: Integer);
begin
  if Offset + N > Length(Samples) then
    begin
      WriteLn(Format('Invalid arguments!. Array length: %d, Offset: %d, N: %d',
        [Length(Samples), Offset, N]
      ));
      Exit;
    end;

  EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(Self.Handle,
    pcfloat(Samples) + Offset, N);
end;

function TEdgevoxOnnxVoiceActivityDetector.IsEmpty: Boolean;
begin
  Result := EdgevoxOnnxVoiceActivityDetectorEmpty(Self.Handle) = 1;
end;

function TEdgevoxOnnxVoiceActivityDetector.IsDetected: Boolean;
begin
  Result := EdgevoxOnnxVoiceActivityDetectorDetected(Self.Handle) = 1;
end;

procedure TEdgevoxOnnxVoiceActivityDetector.Pop;
begin
  EdgevoxOnnxVoiceActivityDetectorPop(Self.Handle);
end;

procedure TEdgevoxOnnxVoiceActivityDetector.Clear;
begin
  EdgevoxOnnxVoiceActivityDetectorClear(Self.Handle);
end;

function TEdgevoxOnnxVoiceActivityDetector.Front: TEdgevoxOnnxSpeechSegment;
var
  P: PEdgevoxOnnxSpeechSegment;
begin
  Result := Default(TEdgevoxOnnxSpeechSegment);

  P := EdgevoxOnnxVoiceActivityDetectorFront(Self.Handle);
  if P = nil then
    Exit;

  Result.Start := P^.Start;
  Result.Samples := nil;
  SetLength(Result.Samples, P^.N);

  if P^.N > 0 then
    Move(P^.Samples[0], Result.Samples[0], P^.N * SizeOf(Single));

  EdgevoxOnnxDestroySpeechSegment(P);
end;

procedure TEdgevoxOnnxVoiceActivityDetector.Reset;
begin
  EdgevoxOnnxVoiceActivityDetectorReset(Self.Handle);
end;

procedure TEdgevoxOnnxVoiceActivityDetector.Flush;
begin
  EdgevoxOnnxVoiceActivityDetectorFlush(Self.Handle);
end;

function TEdgevoxOnnxOfflineTtsVitsModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsVitsModelConfig(' +
    'Model := %s, ' +
    'Lexicon := %s, ' +
    'Tokens := %s, ' +
    'DataDir := %s, ' +
    'NoiseScale := %.2f, ' +
    'NoiseScaleW := %.2f, ' +
    'LengthScale := %.2f' +
    ')',
    [Self.Model, Self.Lexicon, Self.Tokens, Self.DataDir, Self.NoiseScale,
     Self.NoiseScaleW, Self.LengthScale
    ]);
end;

class operator TEdgevoxOnnxOfflineTtsVitsModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsVitsModelConfig);
begin
  Dest.NoiseScale := 0.667;
  Dest.NoiseScaleW := 0.8;
  Dest.LengthScale := 1.0;
end;

class operator TEdgevoxOnnxGenerationConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxGenerationConfig);
begin
  Dest.SilenceScale := 0.2;
  Dest.Speed := 1.0;
  Dest.Sid := 0;
  Dest.ReferenceAudioLen := 0;
  Dest.ReferenceSampleRate := 0;
  Dest.NumSteps := 5;
end;

function TEdgevoxOnnxOfflineTtsMatchaModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsMatchaModelConfig(' +
    'AcousticModel := %s, ' +
    'Vocoder := %s, ' +
    'Lexicon := %s, ' +
    'Tokens := %s, ' +
    'DataDir := %s, ' +
    'NoiseScale := %.2f, ' +
    'LengthScale := %.2f' +
    ')',
    [Self.AcousticModel, Self.Vocoder, Self.Lexicon, Self.Tokens,
     Self.DataDir, Self.NoiseScale, Self.LengthScale
    ]);
end;

class operator TEdgevoxOnnxOfflineTtsMatchaModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsMatchaModelConfig);
begin
  Dest.NoiseScale := 0.667;
  Dest.LengthScale := 1.0;
end;

function TEdgevoxOnnxOfflineTtsKokoroModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsKokoroModelConfig(' +
    'Model := %s, ' +
    'Voices := %s, ' +
    'Tokens := %s, ' +
    'DataDir := %s, ' +
    'LengthScale := %.2f, ' +
    'Lexicon := %s, ' +
    'Lang := %s' +
    ')',
    [Self.Model, Self.Voices, Self.Tokens, Self.DataDir, Self.LengthScale,
     Self.Lexicon, Self.Lang]);
end;

class operator TEdgevoxOnnxOfflineTtsKokoroModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsKokoroModelConfig);
begin
  Dest.LengthScale := 1.0;
end;

function TEdgevoxOnnxOfflineTtsKittenModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsKittenModelConfig(' +
    'Model := %s, ' +
    'Voices := %s, ' +
    'Tokens := %s, ' +
    'DataDir := %s, ' +
    'LengthScale := %.2f' +
    ')',
    [Self.Model, Self.Voices, Self.Tokens, Self.DataDir, Self.LengthScale]);
end;

class operator TEdgevoxOnnxOfflineTtsKittenModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsKittenModelConfig);
begin
  Dest.LengthScale := 1.0;
end;

function TEdgevoxOnnxOfflineTtsZipVoiceModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsZipVoiceModelConfig(' +
    'Tokens := %s, ' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'Vocoder := %s, ' +
    'DataDir := %s, ' +
    'Lexicon := %s, ' +
    'FeatScale := %.2f, ' +
    'Tshift := %.2f, ' +
    'TargetRms := %.2f, ' +
    'GuidanceScale := %.2f' +
    ')',
    [Self.Tokens, Self.Encoder, Self.Decoder, Self.Vocoder,
     Self.DataDir, Self.Lexicon, Self.FeatScale, Self.Tshift,
     Self.TargetRms, Self.GuidanceScale]);
end;

class operator TEdgevoxOnnxOfflineTtsZipVoiceModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsZipVoiceModelConfig);
begin
  Dest.FeatScale := 0.1;
  Dest.Tshift := 0.5;
  Dest.TargetRms := 0.1;
  Dest.GuidanceScale := 1.0;
end;

class operator TEdgevoxOnnxOfflineTtsPocketModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsPocketModelConfig);
begin
  Dest.VoiceEmbeddingCacheCapacity := 50;
end;

function TEdgevoxOnnxOfflineTtsPocketModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsPocketModelConfig(' +
    'LmFlow := %s, ' +
    'LmMain := %s, ' +
    'Encoder := %s, ' +
    'Decoder := %s, ' +
    'TextConditioner := %s, ' +
    'VocabJson := %s, ' +
    'TokenScoresJson := %s, ' +
    'VoiceEmbeddingCacheCapacity := %d' +
    ')',
    [Self.LmFlow, Self.LmMain, Self.Encoder, Self.Decoder, Self.TextConditioner,
     Self.VocabJson, Self.TokenScoresJson, Self.VoiceEmbeddingCacheCapacity]);
end;

function TEdgevoxOnnxOfflineTtsSupertonicModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsSupertonicModelConfig(' +
    'DurationPredictor := %s, ' +
    'TextEncoder := %s, ' +
    'VectorEstimator := %s, ' +
    'Vocoder := %s, ' +
    'TtsJson := %s, ' +
    'UnicodeIndexer := %s, ' +
    'VoiceStyle := %s' +
    ')',
    [Self.DurationPredictor, Self.TextEncoder, Self.VectorEstimator, Self.Vocoder,
     Self.TtsJson, Self.UnicodeIndexer, Self.VoiceStyle]);
end;

function TEdgevoxOnnxOfflineTtsModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsModelConfig(' +
    'Vits := %s, ' +
    'NumThreads := %d, ' +
    'Debug := %s, ' +
    'Provider := %s, ' +
    'Matcha := %s, ' +
    'Kokoro := %s, ' +
    'Kitten := %s, ' +
    'ZipVoice := %s, ' +
    'Pocket := %s, ' +
    'Supertonic := %s' +
    ')',
    [Self.Vits.ToString, Self.NumThreads, Self.Debug.ToString, Self.Provider,
     Self.Matcha.ToString, Self.Kokoro.ToString, Self.Kitten.ToString,
     Self.ZipVoice.ToString, Self.Pocket.ToString, Self.Supertonic.ToString
    ]);
end;

class operator TEdgevoxOnnxOfflineTtsModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsModelConfig);
begin
  Dest.NumThreads := 1;
  Dest.Debug := False;
  Dest.Provider := 'cpu';
end;

function TEdgevoxOnnxOfflineTtsConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineTtsConfig(' +
    'Model := %s, ' +
    'RuleFsts := %s, ' +
    'MaxNumSentences := %d, ' +
    'RuleFars := %s, ' +
    'SilenceScale := %f' +
    ')',
    [Self.Model.ToString, Self.RuleFsts, Self.MaxNumSentences, Self.RuleFars,
     Self.SilenceScale]);
end;

class operator TEdgevoxOnnxOfflineTtsConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineTtsConfig);
begin
  Dest.MaxNumSentences := 1;
  Dest.SilenceScale := 0.2;
end;

constructor TEdgevoxOnnxOfflineTts.Create(Config: TEdgevoxOnnxOfflineTtsConfig);
var
  C: EdgevoxOnnxOfflineTtsConfig;
begin
  C := Default(EdgevoxOnnxOfflineTtsConfig);
  Self._Config := Config;

  C.Model.Vits.Model := PAnsiChar(Config.Model.Vits.Model);
  C.Model.Vits.Lexicon := PAnsiChar(Config.Model.Vits.Lexicon);
  C.Model.Vits.Tokens := PAnsiChar(Config.Model.Vits.Tokens);
  C.Model.Vits.DataDir := PAnsiChar(Config.Model.Vits.DataDir);
  C.Model.Vits.NoiseScale := Config.Model.Vits.NoiseScale;
  C.Model.Vits.NoiseScaleW := Config.Model.Vits.NoiseScaleW;
  C.Model.Vits.LengthScale := Config.Model.Vits.LengthScale;

  C.Model.Matcha.AcousticModel := PAnsiChar(Config.Model.Matcha.AcousticModel);
  C.Model.Matcha.Vocoder := PAnsiChar(Config.Model.Matcha.Vocoder);
  C.Model.Matcha.Lexicon := PAnsiChar(Config.Model.Matcha.Lexicon);
  C.Model.Matcha.Tokens := PAnsiChar(Config.Model.Matcha.Tokens);
  C.Model.Matcha.DataDir := PAnsiChar(Config.Model.Matcha.DataDir);
  C.Model.Matcha.NoiseScale := Config.Model.Matcha.NoiseScale;
  C.Model.Matcha.LengthScale := Config.Model.Matcha.LengthScale;

  C.Model.Kokoro.Model := PAnsiChar(Config.Model.Kokoro.Model);
  C.Model.Kokoro.Voices := PAnsiChar(Config.Model.Kokoro.Voices);
  C.Model.Kokoro.Tokens := PAnsiChar(Config.Model.Kokoro.Tokens);
  C.Model.Kokoro.DataDir := PAnsiChar(Config.Model.Kokoro.DataDir);
  C.Model.Kokoro.LengthScale := Config.Model.Kokoro.LengthScale;
  C.Model.Kokoro.Lexicon := PAnsiChar(Config.Model.Kokoro.Lexicon);
  C.Model.Kokoro.Lang := PAnsiChar(Config.Model.Kokoro.Lang);

  C.Model.Kitten.Model := PAnsiChar(Config.Model.Kitten.Model);
  C.Model.Kitten.Voices := PAnsiChar(Config.Model.Kitten.Voices);
  C.Model.Kitten.Tokens := PAnsiChar(Config.Model.Kitten.Tokens);
  C.Model.Kitten.DataDir := PAnsiChar(Config.Model.Kitten.DataDir);
  C.Model.Kitten.LengthScale := Config.Model.Kitten.LengthScale;

  C.Model.ZipVoice.Tokens := PAnsiChar(Config.Model.ZipVoice.Tokens);
  C.Model.ZipVoice.Encoder := PAnsiChar(Config.Model.ZipVoice.Encoder);
  C.Model.ZipVoice.Decoder := PAnsiChar(Config.Model.ZipVoice.Decoder);
  C.Model.ZipVoice.Vocoder := PAnsiChar(Config.Model.ZipVoice.Vocoder);
  C.Model.ZipVoice.DataDir := PAnsiChar(Config.Model.ZipVoice.DataDir);
  C.Model.ZipVoice.Lexicon := PAnsiChar(Config.Model.ZipVoice.Lexicon);
  C.Model.ZipVoice.FeatScale := Config.Model.ZipVoice.FeatScale;
  C.Model.ZipVoice.Tshift := Config.Model.ZipVoice.Tshift;
  C.Model.ZipVoice.TargetRms := Config.Model.ZipVoice.TargetRms;
  C.Model.ZipVoice.GuidanceScale := Config.Model.ZipVoice.GuidanceScale;

  C.Model.Pocket.LmFlow := PAnsiChar(Config.Model.Pocket.LmFlow);
  C.Model.Pocket.LmMain := PAnsiChar(Config.Model.Pocket.LmMain);
  C.Model.Pocket.Encoder := PAnsiChar(Config.Model.Pocket.Encoder);
  C.Model.Pocket.Decoder := PAnsiChar(Config.Model.Pocket.Decoder);
  C.Model.Pocket.TextConditioner := PAnsiChar(Config.Model.Pocket.TextConditioner);
  C.Model.Pocket.VocabJson := PAnsiChar(Config.Model.Pocket.VocabJson);
  C.Model.Pocket.TokenScoresJson := PAnsiChar(Config.Model.Pocket.TokenScoresJson);
  C.Model.Pocket.VoiceEmbeddingCacheCapacity := Config.Model.Pocket.VoiceEmbeddingCacheCapacity;

  C.Model.Supertonic.DurationPredictor := PAnsiChar(Config.Model.Supertonic.DurationPredictor);
  C.Model.Supertonic.TextEncoder := PAnsiChar(Config.Model.Supertonic.TextEncoder);
  C.Model.Supertonic.VectorEstimator := PAnsiChar(Config.Model.Supertonic.VectorEstimator);
  C.Model.Supertonic.Vocoder := PAnsiChar(Config.Model.Supertonic.Vocoder);
  C.Model.Supertonic.TtsJson := PAnsiChar(Config.Model.Supertonic.TtsJson);
  C.Model.Supertonic.UnicodeIndexer := PAnsiChar(Config.Model.Supertonic.UnicodeIndexer);
  C.Model.Supertonic.VoiceStyle := PAnsiChar(Config.Model.Supertonic.VoiceStyle);

  C.Model.NumThreads := Config.Model.NumThreads;
  C.Model.Provider := PAnsiChar(Config.Model.Provider);
  C.Model.Debug := Ord(Config.Model.Debug);

  C.RuleFsts := PAnsiChar(Config.RuleFsts);
  C.MaxNumSentences := Config.MaxNumSentences;
  C.RuleFars := PAnsiChar(Config.RuleFars);
  C.SilenceScale := Config.SilenceScale;

  Self.Handle := EdgevoxOnnxCreateOfflineTts(@C);

  Self.SampleRate := EdgevoxOnnxOfflineTtsSampleRate(Self.Handle);
  Self.NumSpeakers := EdgevoxOnnxOfflineTtsNumSpeakers(Self.Handle);
end;

destructor TEdgevoxOnnxOfflineTts.Destroy;
begin
  EdgevoxOnnxDestroyOfflineTts(Self.Handle);
  Self.Handle := nil;
end;

function ExtractGeneratedAudio(Audio: PEdgevoxOnnxGeneratedAudio): TEdgevoxOnnxGeneratedAudio;
begin
  Result := Default(TEdgevoxOnnxGeneratedAudio);

  if Audio = nil then
    Exit;

  SetLength(Result.Samples, Audio^.N);
  Result.SampleRate := Audio^.SampleRate;

  if Audio^.N > 0 then
    Move(Audio^.Samples[0], Result.Samples[0], Audio^.N * SizeOf(Single));

  EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(Audio);
end;

{ Adapts TEdgevoxOnnxGeneratedAudioCallbackWithArg (no progress) to
  TEdgevoxOnnxGeneratedAudioProgressCallbackWithArg so we can use the
  non-deprecated EdgevoxOnnxOfflineTtsGenerateWithConfig. }
var
  _OfflineTtsCallbackWithArg: TEdgevoxOnnxGeneratedAudioCallbackWithArg;
  _OfflineTtsCallbackWithArgUserArg: Pointer;

function _OfflineTtsCallbackWithArgWrapper(Samples: pcfloat; N: cint32;
  P: cfloat; Arg: Pointer): cint32; cdecl;
begin
  Result := _OfflineTtsCallbackWithArg(Samples, N, _OfflineTtsCallbackWithArgUserArg);
end;

function TEdgevoxOnnxOfflineTts.Generate(Text: AnsiString; SpeakerId: Integer;
  Speed: Single): TEdgevoxOnnxGeneratedAudio;
var
  Audio: PEdgevoxOnnxGeneratedAudio;
  Config: TEdgevoxOnnxGenerationConfig;
begin
  Config := Default(TEdgevoxOnnxGenerationConfig);
  Config.Sid := SpeakerId;
  Config.Speed := Speed;
  Audio := EdgevoxOnnxOfflineTtsGenerateWithConfig(Self.Handle, PAnsiChar(Text),
    @Config, nil, nil);
  Result := ExtractGeneratedAudio(Audio);
end;

function TEdgevoxOnnxOfflineTts.Generate(Text: AnsiString; SpeakerId: Integer;
  Speed: Single;
  Callback: TEdgevoxOnnxGeneratedAudioCallbackWithArg;
  Arg: Pointer
  ): TEdgevoxOnnxGeneratedAudio;
var
  Audio: PEdgevoxOnnxGeneratedAudio;
  Config: TEdgevoxOnnxGenerationConfig;
begin
  Config := Default(TEdgevoxOnnxGenerationConfig);
  Config.Sid := SpeakerId;
  Config.Speed := Speed;

  _OfflineTtsCallbackWithArg := Callback;
  _OfflineTtsCallbackWithArgUserArg := Arg;

  if Assigned(Callback) then
    Audio := EdgevoxOnnxOfflineTtsGenerateWithConfig(Self.Handle, PAnsiChar(Text),
      @Config, @_OfflineTtsCallbackWithArgWrapper, nil)
  else
    Audio := EdgevoxOnnxOfflineTtsGenerateWithConfig(Self.Handle, PAnsiChar(Text),
      @Config, nil, nil);

  Result := ExtractGeneratedAudio(Audio);
end;

function TEdgevoxOnnxOfflineTts.Generate(Text: AnsiString;
  GenerationConfig: TEdgevoxOnnxGenerationConfig;
  Callback: TEdgevoxOnnxGeneratedAudioProgressCallbackWithArg;
  Arg: Pointer
  ): TEdgevoxOnnxGeneratedAudio;
var
  Audio: PEdgevoxOnnxGeneratedAudio;
  C: EdgevoxOnnxGenerationConfig;
  ReferenceAudio: TEdgevoxOnnxSamplesArray;
  CReferenceAudio: pcfloat;
  ReferenceText: AnsiString;
  Extra: AnsiString;
begin
  C := Default(EdgevoxOnnxGenerationConfig);
  C.SilenceScale := GenerationConfig.SilenceScale;
  C.Speed := GenerationConfig.Speed;
  C.Sid := GenerationConfig.Sid;
  ReferenceAudio := GenerationConfig.ReferenceAudio;
  CReferenceAudio := nil;
  C.ReferenceAudio := nil;
  C.ReferenceAudioLen := Length(ReferenceAudio);
  if C.ReferenceAudioLen > 0 then
    begin
      GetMem(CReferenceAudio, C.ReferenceAudioLen * SizeOf(Single));
      Move(ReferenceAudio[0], CReferenceAudio[0], C.ReferenceAudioLen * SizeOf(Single));
      C.ReferenceAudio := CReferenceAudio;
    end;
  C.ReferenceSampleRate:= GenerationConfig.ReferenceSampleRate;
  ReferenceText := GenerationConfig.ReferenceText;
  C.ReferenceText := PAnsiChar(ReferenceText);
  C.NumSteps := GenerationConfig.NumSteps;
  Extra := GenerationConfig.Extra;
  C.Extra := PAnsiChar(Extra);

  Audio := nil;
  try
    Audio := EdgevoxOnnxOfflineTtsGenerateWithConfig(Self.Handle, PAnsiChar(Text),
      @C, Callback, Arg);
  finally
    if CReferenceAudio <> nil then
      FreeMem(CReferenceAudio);
  end;

  Result := ExtractGeneratedAudio(Audio);
end;

constructor TEdgevoxOnnxLinearResampler.Create(SampleRateIn: Integer; SampleRateOut: Integer);
var
  MinFreq: Single;
  LowpassCutoff: Single;
  LowpassFilterWidth: Integer = 6;
begin
  if SampleRateIn > SampleRateOut then
    MinFreq := SampleRateOut
  else
    MinFreq := SampleRateIn;

  LowpassCutoff := 0.99 * 0.5 * MinFreq;

  Self.Handle := EdgevoxOnnxCreateLinearResampler(SampleRateIn,
    SampleRateOut, LowpassCutoff, LowpassFilterWidth);
  Self.InputSampleRate := SampleRateIn;
  Self.OutputSampleRate := SampleRateOut;
end;

destructor TEdgevoxOnnxLinearResampler.Destroy;
begin
  EdgevoxOnnxDestroyLinearResampler(Self.Handle);
  Self.Handle := nil;
end;

function TEdgevoxOnnxLinearResampler.Resample(Samples: pcfloat;
  N: Integer; Flush: Boolean): TEdgevoxOnnxSamplesArray;
var
  P: PEdgevoxOnnxResampleOut;
begin
  Result := Default(TEdgevoxOnnxSamplesArray);
  P := EdgevoxOnnxLinearResamplerResample(Self.Handle, Samples, N, Ord(Flush));
  if P = nil then
    Exit;

  SetLength(Result, P^.N);

  if P^.N > 0 then
    Move(P^.Samples[0], Result[0], P^.N * SizeOf(Single));

  EdgevoxOnnxLinearResamplerResampleFree(P);
end;

function TEdgevoxOnnxLinearResampler.Resample(const Samples: array of Single; Flush: Boolean): TEdgevoxOnnxSamplesArray;
begin
  Result := Self.Resample(pcfloat(Samples), Length(Samples), Flush);
end;

procedure TEdgevoxOnnxLinearResampler.Reset;
begin
  EdgevoxOnnxLinearResamplerReset(Self.Handle);
end;

function TEdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig(' +
    'Model := %s)',[Self.Model]);
end;

function TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig(' +
    'Pyannote := %s, ' +
    'NumThreads := %d, ' +
    'Debug := %s, ' +
    'Provider := %s)',
    [Self.Pyannote.ToString, Self.NumThreads,
     Self.Debug.ToString, Self.Provider]);
end;

class operator TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeakerSegmentationModelConfig);
begin
  Dest.NumThreads := 1;
  Dest.Debug := False;
  Dest.Provider := 'cpu';
end;

function TEdgevoxOnnxFastClusteringConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxFastClusteringConfig(' +
    'NumClusters := %d, Threshold := %.3f)',
    [Self.NumClusters, Self.Threshold]);
end;

class operator TEdgevoxOnnxFastClusteringConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxFastClusteringConfig);
begin
  Dest.NumClusters := -1;
  Dest.Threshold := 0.5;
end;

function TEdgevoxOnnxSpeakerEmbeddingExtractorConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxSpeakerEmbeddingExtractorConfig(' +
    'Model := %s, '+
    'NumThreads := %d, '+
    'Debug := %s, '+
    'Provider := %s)',
    [Self.Model, Self.NumThreads, Self.Debug.ToString, Self.Provider]);
end;

class operator TEdgevoxOnnxSpeakerEmbeddingExtractorConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxSpeakerEmbeddingExtractorConfig);
begin
  Dest.NumThreads := 1;
  Dest.Debug := False;
  Dest.Provider := 'cpu';
end;

function TEdgevoxOnnxOfflineSpeakerDiarizationConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeakerDiarizationConfig(' +
    'Segmentation := %s, '+
    'Embedding := %s, '+
    'Clustering := %s, '+
    'MinDurationOn := %.3f, '+
    'MinDurationOff := %.3f)',
    [Self.Segmentation.ToString, Self.Embedding.ToString,
     Self.Clustering.ToString, Self.MinDurationOn, Self.MinDurationOff]);
end;

class operator TEdgevoxOnnxOfflineSpeakerDiarizationConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
begin
  Dest.MinDurationOn := 0.2;
  Dest.MinDurationOff := 0.5;
end;

function TEdgevoxOnnxOfflineSpeakerDiarizationSegment.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeakerDiarizationSegment(' +
    'Start := %.3f, '+
    'Stop := %.3f, '+
    'Speaker := %d)',
    [Self.Start, Self.Stop, Self.Speaker]);
end;

constructor TEdgevoxOnnxOfflineSpeakerDiarization.Create(Config: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
var
  C: EdgevoxOnnxOfflineSpeakerDiarizationConfig;
begin
  C := Default(EdgevoxOnnxOfflineSpeakerDiarizationConfig);
  C.Segmentation.Pyannote.Model := PAnsiChar(Config.Segmentation.Pyannote.Model);
  C.Segmentation.NumThreads := Config.Segmentation.NumThreads;
  C.Segmentation.Debug := Ord(Config.Segmentation.Debug);
  C.Segmentation.Provider := PAnsiChar(Config.Segmentation.Provider);

  C.Embedding.Model := PAnsiChar(Config.Embedding.Model);
  C.Embedding.NumThreads := Config.Embedding.NumThreads;
  C.Embedding.Debug := Ord(Config.Embedding.Debug);
  C.Embedding.Provider := PAnsiChar(Config.Embedding.Provider);

  C.Clustering.NumClusters := Config.Clustering.NumClusters;
  C.Clustering.Threshold := Config.Clustering.Threshold;

  C.MinDurationOn := Config.MinDurationOn;
  C.MinDurationOff := Config.MinDurationOff;

  Self.Handle := EdgevoxOnnxCreateOfflineSpeakerDiarization(@C);
  Self._Config := Config;
  Self.SampleRate :=  0;

  if Self.Handle <> nil then
    begin
      Self.SampleRate := EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(Self.Handle);
    end;
end;

destructor TEdgevoxOnnxOfflineSpeakerDiarization.Destroy;
begin
  EdgevoxOnnxDestroyOfflineSpeakerDiarization(Self.Handle);
  Self.Handle := nil;
end;

procedure TEdgevoxOnnxOfflineSpeakerDiarization.SetConfig(Config: TEdgevoxOnnxOfflineSpeakerDiarizationConfig);
var
  C: EdgevoxOnnxOfflineSpeakerDiarizationConfig;
begin
  C := Default(EdgevoxOnnxOfflineSpeakerDiarizationConfig);

  C.Clustering.NumClusters := Config.Clustering.NumClusters;
  C.Clustering.Threshold := Config.Clustering.Threshold;

  EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(Self.Handle, @C);
end;

function TEdgevoxOnnxOfflineSpeakerDiarization.Process(const Samples: array of Single): TEdgevoxOnnxOfflineSpeakerDiarizationSegmentArray;
var
  R: Pointer;
  NumSegments: Integer;
  I: Integer;
  Segments: PEdgevoxOnnxOfflineSpeakerDiarizationSegment;
begin
  Result := nil;

  R := EdgevoxOnnxOfflineSpeakerDiarizationProcess(Self.Handle, pcfloat(Samples), Length(Samples));
  if R = nil then
    begin
      Exit
    end;
  NumSegments := EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(R);

  Segments := EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(R);

  SetLength(Result, NumSegments);
  for I := Low(Result) to High(Result) do
    begin
      Result[I].Start := Segments[I].Start;
      Result[I].Stop := Segments[I].Stop;
      Result[I].Speaker := Segments[I].Speaker;
    end;

  EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(Segments);
  EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(R);
end;

function TEdgevoxOnnxOfflineSpeakerDiarization.Process(const Samples: array of Single;
  callback: PEdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg): TEdgevoxOnnxOfflineSpeakerDiarizationSegmentArray;
var
  R: Pointer;
  NumSegments: Integer;
  I: Integer;
  Segments: PEdgevoxOnnxOfflineSpeakerDiarizationSegment;
begin
  Result := nil;

  R := EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg(Self.Handle, pcfloat(Samples), Length(Samples), callback);
  if R = nil then
    begin
      Exit
    end;
  NumSegments := EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(R);

  Segments := EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(R);

  SetLength(Result, NumSegments);
  for I := Low(Result) to High(Result) do
    begin
      Result[I].Start := Segments[I].Start;
      Result[I].Stop := Segments[I].Stop;
      Result[I].Speaker := Segments[I].Speaker;
    end;

  EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(Segments);
  EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(R);
end;

function TEdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig(' +
    'Model := %s)', [Self.Model]);
end;

function TEdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig(' +
    'Model := %s)', [Self.Model]);
end;

function TEdgevoxOnnxOfflineSpeechDenoiserModelConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeechDenoiserModelConfig(' +
    'Gtcrn := %s, '+
    'DpdfNet := %s, '+
    'NumThreads := %d, '+
    'Debug := %s, '+
    'Provider := %s)',
    [Self.Gtcrn.ToString, Self.DpdfNet.ToString, Self.NumThreads, Self.Debug.ToString, Self.Provider]);
end;

class operator TEdgevoxOnnxOfflineSpeechDenoiserModelConfig.Initialize({$IFDEF FPC}var{$ELSE}out{$ENDIF} Dest: TEdgevoxOnnxOfflineSpeechDenoiserModelConfig);
begin
  Dest.NumThreads := 1;
  Dest.Debug := False;
  Dest.Provider := 'cpu';
end;

function TEdgevoxOnnxOfflineSpeechDenoiserConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOfflineSpeechDenoiserConfig(' +
    'Model := %s)', [Self.Model.ToString]);
end;

function TEdgevoxOnnxOnlineSpeechDenoiserConfig.ToString: AnsiString;
begin
  Result := Format('TEdgevoxOnnxOnlineSpeechDenoiserConfig(' +
    'Model := %s)', [Self.Model.ToString]);
end;

function ExtractDenoisedAudio(Audio: PEdgevoxOnnxDenoisedAudio): TEdgevoxOnnxDenoisedAudio;
begin
  Result := Default(TEdgevoxOnnxDenoisedAudio);

  if Audio = nil then
    Exit;

  SetLength(Result.Samples, Audio^.N);
  Result.SampleRate := Audio^.SampleRate;

  if Audio^.N > 0 then
    Move(Audio^.Samples[0], Result.Samples[0], Audio^.N * SizeOf(Single));

  EdgevoxOnnxDestroyDenoisedAudio(Audio);
end;

constructor TEdgevoxOnnxOfflineSpeechDenoiser.Create(Config: TEdgevoxOnnxOfflineSpeechDenoiserConfig);
var
  C: EdgevoxOnnxOfflineSpeechDenoiserConfig;
begin
  C := Default(EdgevoxOnnxOfflineSpeechDenoiserConfig);
  C.Model.Gtcrn.Model := PAnsiChar(Config.Model.Gtcrn.Model);
  C.Model.DpdfNet.Model := PAnsiChar(Config.Model.DpdfNet.Model);
  C.Model.NumThreads := Config.Model.NumThreads;
  C.Model.Debug := Ord(Config.Model.Debug);
  C.Model.Provider := PAnsiChar(Config.Model.Provider);

  Self.Handle := EdgevoxOnnxCreateOfflineSpeechDenoiser(@C);
  Self._Config := Config;
  Self.SampleRate :=  0;

  if Self.Handle <> nil then
    begin
      Self.SampleRate := EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(Self.Handle);
    end;
end;

destructor TEdgevoxOnnxOfflineSpeechDenoiser.Destroy;
begin
  EdgevoxOnnxDestroyOfflineSpeechDenoiser(Self.Handle);
  Self.Handle := nil;
end;

function TEdgevoxOnnxOfflineSpeechDenoiser.Run(const Samples: array of Single; InputSampleRate: Integer): TEdgevoxOnnxDenoisedAudio;
var
  Audio: PEdgevoxOnnxDenoisedAudio;
begin
  Audio := EdgevoxOnnxOfflineSpeechDenoiserRun(Self.Handle, pcfloat(Samples), Length(Samples), InputSampleRate);
  Result := ExtractDenoisedAudio(Audio);
end;

constructor TEdgevoxOnnxOnlineSpeechDenoiser.Create(Config: TEdgevoxOnnxOnlineSpeechDenoiserConfig);
var
  C: EdgevoxOnnxOnlineSpeechDenoiserConfig;
begin
  C := Default(EdgevoxOnnxOnlineSpeechDenoiserConfig);
  C.Model.Gtcrn.Model := PAnsiChar(Config.Model.Gtcrn.Model);
  C.Model.DpdfNet.Model := PAnsiChar(Config.Model.DpdfNet.Model);
  C.Model.NumThreads := Config.Model.NumThreads;
  C.Model.Debug := Ord(Config.Model.Debug);
  C.Model.Provider := PAnsiChar(Config.Model.Provider);

  Self.Handle := EdgevoxOnnxCreateOnlineSpeechDenoiser(@C);
  Self._Config := Config;
  Self.SampleRate := 0;
  Self.FrameShiftInSamples := 0;

  if Self.Handle <> nil then
    begin
      Self.SampleRate := EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(Self.Handle);
      Self.FrameShiftInSamples := EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(Self.Handle);
    end;
end;

destructor TEdgevoxOnnxOnlineSpeechDenoiser.Destroy;
begin
  EdgevoxOnnxDestroyOnlineSpeechDenoiser(Self.Handle);
  Self.Handle := nil;
end;

function TEdgevoxOnnxOnlineSpeechDenoiser.Run(const Samples: array of Single; InputSampleRate: Integer): TEdgevoxOnnxDenoisedAudio;
var
  Audio: PEdgevoxOnnxDenoisedAudio;
begin
  Audio := EdgevoxOnnxOnlineSpeechDenoiserRun(Self.Handle, pcfloat(Samples), Length(Samples), InputSampleRate);
  Result := ExtractDenoisedAudio(Audio);
end;

function TEdgevoxOnnxOnlineSpeechDenoiser.Flush: TEdgevoxOnnxDenoisedAudio;
var
  Audio: PEdgevoxOnnxDenoisedAudio;
begin
  Audio := EdgevoxOnnxOnlineSpeechDenoiserFlush(Self.Handle);
  Result := ExtractDenoisedAudio(Audio);
end;

procedure TEdgevoxOnnxOnlineSpeechDenoiser.Reset;
begin
  EdgevoxOnnxOnlineSpeechDenoiserReset(Self.Handle);
end;

initialization
  { Match the C API's default behavior. PocketTTS can raise FP overflow flags
    during native inference on some platforms, and Free Pascal would otherwise
    surface them as EOverflow.
    See also https://github.com/k2-fsa/edgevox-onnx/pull/3351
  }
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide, exOverflow,
    exUnderflow, exPrecision]);

end.
