{ Copyright (c)  2024  Xiaomi Corporation }

{
This file shows how to use a non-streaming NeMo transducer
to decode files.

You can download the model files from
https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
}

program nemo_transducer;

{$mode objfpc}

uses
  edgevox_onnx,
  DateUtils,
  SysUtils;

var
  Wave: TEdgevoxOnnxWave;
  WaveFilename: AnsiString;

  Config: TEdgevoxOnnxOfflineRecognizerConfig;
  Recognizer: TEdgevoxOnnxOfflineRecognizer;
  Stream: TEdgevoxOnnxOfflineStream;
  RecognitionResult: TEdgevoxOnnxOfflineRecognizerResult;

  Start: TDateTime;
  Stop: TDateTime;

  Elapsed: Single;
  Duration: Single;
  RealTimeFactor: Single;
begin
  Initialize(Config);

  Config.ModelConfig.Transducer.Encoder := './edgevox-onnx-nemo-fast-conformer-transducer-be-de-en-es-fr-hr-it-pl-ru-uk-20k/encoder.onnx';
  Config.ModelConfig.Transducer.Decoder := './edgevox-onnx-nemo-fast-conformer-transducer-be-de-en-es-fr-hr-it-pl-ru-uk-20k/decoder.onnx';
  Config.ModelConfig.Transducer.Joiner := './edgevox-onnx-nemo-fast-conformer-transducer-be-de-en-es-fr-hr-it-pl-ru-uk-20k/joiner.onnx';
  Config.ModelConfig.ModelType := 'nemo_transducer';
  Config.ModelConfig.Tokens := './edgevox-onnx-nemo-fast-conformer-transducer-be-de-en-es-fr-hr-it-pl-ru-uk-20k/tokens.txt';
  Config.ModelConfig.Provider := 'cpu';
  Config.ModelConfig.NumThreads := 1;
  Config.ModelConfig.Debug := False;

  WaveFilename := './edgevox-onnx-nemo-fast-conformer-transducer-be-de-en-es-fr-hr-it-pl-ru-uk-20k/test_wavs/de-german.wav';

  Wave := EdgevoxOnnxReadWave(WaveFilename);

  Recognizer := TEdgevoxOnnxOfflineRecognizer.Create(Config);
  Stream := Recognizer.CreateStream();
  Start := Now;

  Stream.AcceptWaveform(Wave.Samples, Wave.SampleRate);
  Recognizer.Decode(Stream);

  RecognitionResult := Recognizer.GetResult(Stream);

  Stop := Now;

  Elapsed := MilliSecondsBetween(Stop, Start) / 1000;
  Duration := Length(Wave.Samples) / Wave.SampleRate;
  RealTimeFactor := Elapsed / Duration;

  WriteLn(RecognitionResult.ToString);
  WriteLn(Format('NumThreads %d', [Config.ModelConfig.NumThreads]));
  WriteLn(Format('Elapsed %.3f s', [Elapsed]));
  WriteLn(Format('Wave duration %.3f s', [Duration]));
  WriteLn(Format('RTF = %.3f/%.3f = %.3f', [Elapsed, Duration, RealTimeFactor]));

  {Free resources to avoid memory leak.

  Note: You don't need to invoke them for this simple script.
  However, you have to invoke them in your own large/complex project.
  }
  FreeAndNil(Stream);
  FreeAndNil(Recognizer);
end.
