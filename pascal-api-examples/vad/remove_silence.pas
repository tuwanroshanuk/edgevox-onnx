{ Copyright (c)  2024  Xiaomi Corporation }
{
This file shows how to use the VAD API from edgevox-onnx
to remove silences from a wave file with silero-vad.
}
program main;

{$mode delphi}

uses
  edgevox_onnx,
  SysUtils;

var
  Wave: TEdgevoxOnnxWave;

  Config: TEdgevoxOnnxVadModelConfig;
  Vad: TEdgevoxOnnxVoiceActivityDetector;
  Offset: Integer;
  WindowSize: Integer;
  SpeechSegment: TEdgevoxOnnxSpeechSegment;

  Start: Single;
  Duration: Single;
  SampleRate: Integer;

  AllSpeechSegment: array of TEdgevoxOnnxSpeechSegment;
  AllSamples: array of Single;
  N: Integer;
  I: Integer;
begin
  SampleRate := 16000; {Please don't change it unless you know the details}

  Wave := EdgevoxOnnxReadWave('./lei-jun-test.wav');
  if Wave.SampleRate <> SampleRate then
    begin
      WriteLn(Format('Expected sample rate: %d. Given: %d',
        [SampleRate, Wave.SampleRate]));

      Exit;
    end;

  WindowSize := 512; {Please don't change it unless you know the details}
  Initialize(Config);

  Config.SileroVad.Model := './silero_vad.onnx';
  Config.SileroVad.MinSpeechDuration := 0.25;
  Config.SileroVad.MinSilenceDuration := 0.5;
  Config.SileroVad.Threshold := 0.5;
  Config.SileroVad.WindowSize := WindowSize;
  Config.NumThreads:= 1;
  Config.Debug:= True;
  Config.Provider:= 'cpu';
  Config.SampleRate := SampleRate;

  Vad := TEdgevoxOnnxVoiceActivityDetector.Create(Config, 20);

  AllSpeechSegment := nil;
  AllSamples := nil;
  Offset := 0;
  while Offset + WindowSize <= Length(Wave.Samples) do
    begin
      Vad.AcceptWaveform(Wave.Samples, Offset, WindowSize);
      Inc(Offset, WindowSize);

      while not Vad.IsEmpty do
        begin
          SetLength(AllSpeechSegment, Length(AllSpeechSegment) + 1);

          SpeechSegment := Vad.Front();
          Vad.Pop();
          AllSpeechSegment[Length(AllSpeechSegment)-1] := SpeechSegment;

          Start := SpeechSegment.Start / SampleRate;
          Duration := Length(SpeechSegment.Samples) / SampleRate;
          WriteLn(Format('%.3f -- %.3f', [Start, Start + Duration]));
        end;
    end;

  Vad.Flush;

  while not Vad.IsEmpty do
    begin
      SetLength(AllSpeechSegment, Length(AllSpeechSegment) + 1);

      SpeechSegment := Vad.Front();
      Vad.Pop();
      AllSpeechSegment[Length(AllSpeechSegment)-1] := SpeechSegment;

      Start := SpeechSegment.Start / SampleRate;
      Duration := Length(SpeechSegment.Samples) / SampleRate;
      WriteLn(Format('%.3f -- %.3f', [Start, Start + Duration]));
    end;

  N := 0;
  for SpeechSegment in AllSpeechSegment do
    Inc(N, Length(SpeechSegment.Samples));

  SetLength(AllSamples, N);

  N := 0;
  for SpeechSegment in AllSpeechSegment do
    begin
      for I := Low(SpeechSegment.Samples) to High(SpeechSegment.Samples) do
        begin
          AllSamples[N] := SpeechSegment.Samples[I];
          Inc(N);
        end;
    end;

  EdgevoxOnnxWriteWave('./lei-jun-test-no-silence.wav', AllSamples, SampleRate);
  WriteLn('Saved to ./lei-jun-test-no-silence.wav');

  FreeAndNil(Vad);
end.
