{ Copyright (c)  2024  Xiaomi Corporation }

{
This file shows how to use a non-streaming Moonshine model
with silero VAD to decode files.

You can download the model files from
https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
}

program vad_with_moonshine;

{$mode objfpc}

uses
  edgevox_onnx,
  SysUtils;

function CreateVad(): TEdgevoxOnnxVoiceActivityDetector;
var
  Config: TEdgevoxOnnxVadModelConfig;

  SampleRate: Integer;
  WindowSize: Integer;
begin
  Initialize(Config);

  SampleRate := 16000; {Please don't change it unless you know the details}
  WindowSize := 512; {Please don't change it unless you know the details}

  Config.SileroVad.Model := './silero_vad.onnx';
  Config.SileroVad.MinSpeechDuration := 0.5;
  Config.SileroVad.MinSilenceDuration := 0.5;
  Config.SileroVad.Threshold := 0.5;
  Config.SileroVad.WindowSize := WindowSize;
  Config.NumThreads:= 1;
  Config.Debug:= True;
  Config.Provider:= 'cpu';
  Config.SampleRate := SampleRate;

  Result := TEdgevoxOnnxVoiceActivityDetector.Create(Config, 30);
end;

function CreateOfflineRecognizer(): TEdgevoxOnnxOfflineRecognizer;
var
  Config: TEdgevoxOnnxOfflineRecognizerConfig;
begin
  Initialize(Config);

  Config.ModelConfig.Moonshine.Preprocessor := './edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx';
  Config.ModelConfig.Moonshine.Encoder := './edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx';
  Config.ModelConfig.Moonshine.UncachedDecoder := './edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx';
  Config.ModelConfig.Moonshine.CachedDecoder := './edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx';

  Config.ModelConfig.Tokens := './edgevox-onnx-moonshine-tiny-en-int8/tokens.txt';
  Config.ModelConfig.Provider := 'cpu';
  Config.ModelConfig.NumThreads := 1;
  Config.ModelConfig.Debug := False;

  Result := TEdgevoxOnnxOfflineRecognizer.Create(Config);
end;

var
  Wave: TEdgevoxOnnxWave;

  Recognizer: TEdgevoxOnnxOfflineRecognizer;
  Vad: TEdgevoxOnnxVoiceActivityDetector;

  Offset: Integer;
  WindowSize: Integer;
  SpeechSegment: TEdgevoxOnnxSpeechSegment;

  Start: Single;
  Duration: Single;

  Stream: TEdgevoxOnnxOfflineStream;
  RecognitionResult: TEdgevoxOnnxOfflineRecognizerResult;
begin
  Vad := CreateVad();
  Recognizer := CreateOfflineRecognizer();

  Wave := EdgevoxOnnxReadWave('./Obama.wav');
  if Wave.SampleRate <> Vad.Config.SampleRate then
    begin
      WriteLn(Format('Expected sample rate: %d. Given: %d',
        [Vad.Config.SampleRate, Wave.SampleRate]));

      Exit;
    end;

  WindowSize := Vad.Config.SileroVad.WindowSize;
  Offset := 0;
  while Offset + WindowSize <= Length(Wave.Samples) do
    begin
      Vad.AcceptWaveform(Wave.Samples, Offset, WindowSize);
      Offset += WindowSize;

      while not Vad.IsEmpty do
        begin
          SpeechSegment := Vad.Front();
          Vad.Pop();
          Stream := Recognizer.CreateStream();

          Stream.AcceptWaveform(SpeechSegment.Samples, Wave.SampleRate);
          Recognizer.Decode(Stream);
          RecognitionResult := Recognizer.GetResult(Stream);

          Start := SpeechSegment.Start / Wave.SampleRate;
          Duration := Length(SpeechSegment.Samples) / Wave.SampleRate;
          WriteLn(Format('%.3f -- %.3f %s',
            [Start, Start + Duration, RecognitionResult.Text]));

          FreeAndNil(Stream);
        end;
    end;

  Vad.Flush;

  while not Vad.IsEmpty do
    begin
      SpeechSegment := Vad.Front();
      Vad.Pop();
      Stream := Recognizer.CreateStream();

      Stream.AcceptWaveform(SpeechSegment.Samples, Wave.SampleRate);
      Recognizer.Decode(Stream);
      RecognitionResult := Recognizer.GetResult(Stream);

      Start := SpeechSegment.Start / Wave.SampleRate;
      Duration := Length(SpeechSegment.Samples) / Wave.SampleRate;
      WriteLn(Format('%.3f -- %.3f %s',
        [Start, Start + Duration, RecognitionResult.Text]));

      FreeAndNil(Stream);
    end;

  FreeAndNil(Recognizer);
  FreeAndNil(Vad);
end.
