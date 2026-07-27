{ Copyright (c)  2025  Xiaomi Corporation }

{
This file shows how to use a streaming T-one CTC model
to decode files.

You can download the model files from
https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
}

program t_one_ctc;

{$mode objfpc}

uses
  edgevox_onnx,
  DateUtils,
  SysUtils;

var
  Config: TEdgevoxOnnxOnlineRecognizerConfig;
  Recognizer: TEdgevoxOnnxOnlineRecognizer;
  Stream: TEdgevoxOnnxOnlineStream;
  RecognitionResult: TEdgevoxOnnxOnlineRecognizerResult;
  Wave: TEdgevoxOnnxWave;
  WaveFilename: AnsiString;
  LeftPaddings: array of Single;
  TailPaddings: array of Single;

  Start: TDateTime;
  Stop: TDateTime;

  Elapsed: Single;
  Duration: Single;
  RealTimeFactor: Single;
begin
  Initialize(Config);

  {Please visit https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
  to download model files used in this file.}
  Config.ModelConfig.ToneCtc.Model := './edgevox-onnx-streaming-t-one-russian-2025-09-08/model.onnx';
  Config.ModelConfig.Tokens := './edgevox-onnx-streaming-t-one-russian-2025-09-08/tokens.txt';
  Config.ModelConfig.Provider := 'cpu';
  Config.ModelConfig.NumThreads := 1;
  Config.ModelConfig.Debug := False;

  WaveFilename := './edgevox-onnx-streaming-t-one-russian-2025-09-08/0.wav';

  Wave := EdgevoxOnnxReadWave(WaveFilename);

  Recognizer := TEdgevoxOnnxOnlineRecognizer.Create(Config);

  Start := Now;

  Stream := Recognizer.CreateStream();

  SetLength(LeftPaddings, Round(Wave.SampleRate * 0.3)); {0.3 seconds of padding}
  Stream.AcceptWaveform(LeftPaddings, Wave.SampleRate);

  Stream.AcceptWaveform(Wave.Samples, Wave.SampleRate);

  SetLength(TailPaddings, Round(Wave.SampleRate * 0.6)); {0.6 seconds of padding}
  Stream.AcceptWaveform(TailPaddings, Wave.SampleRate);

  Stream.InputFinished();

  while Recognizer.IsReady(Stream) do
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
