{ Copyright (c)  2026  Xiaomi Corporation }
{
This file shows how to use the streaming speech enhancement API from edgevox-onnx
with a DPDFNet model.

Please first download files used in this script before you run it.

wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/dpdfnet_baseline.onnx
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/inp_16k.wav
}
program main;

{$mode delphi}

uses
  edgevox_onnx,
  SysUtils;

var
  Wave: TEdgevoxOnnxWave;
  Config: TEdgevoxOnnxOnlineSpeechDenoiserConfig;
  Sd: TEdgevoxOnnxOnlineSpeechDenoiser;
  Audio: TEdgevoxOnnxDenoisedAudio;
  Chunk: array of Single;
  Enhanced: array of Single;
  StartIndex: Integer;
  N: Integer;
  NewLength: Integer;
begin
  Wave := EdgevoxOnnxReadWave('./inp_16k.wav');

  Initialize(Config);
  Config.Model.DpdfNet.Model := './dpdfnet_baseline.onnx';
  Config.Model.NumThreads:= 1;
  Config.Model.Debug:= True;
  Config.Model.Provider:= 'cpu';

  Sd := TEdgevoxOnnxOnlineSpeechDenoiser.Create(Config);

  SetLength(Enhanced, 0);
  StartIndex := 0;
  while StartIndex < Length(Wave.Samples) do
    begin
      N := Sd.GetFrameShiftInSamples;
      if StartIndex + N > Length(Wave.Samples) then
        N := Length(Wave.Samples) - StartIndex;

      Chunk := Copy(Wave.Samples, StartIndex, N);
      Audio := Sd.Run(Chunk, Wave.SampleRate);
      NewLength := Length(Enhanced) + Length(Audio.Samples);
      SetLength(Enhanced, NewLength);
      if Length(Audio.Samples) > 0 then
        Move(Audio.Samples[0], Enhanced[NewLength - Length(Audio.Samples)],
          Length(Audio.Samples) * SizeOf(Single));
      Inc(StartIndex, N);
    end;

  Audio := Sd.Flush;
  NewLength := Length(Enhanced) + Length(Audio.Samples);
  SetLength(Enhanced, NewLength);
  if Length(Audio.Samples) > 0 then
    Move(Audio.Samples[0], Enhanced[NewLength - Length(Audio.Samples)],
      Length(Audio.Samples) * SizeOf(Single));

  EdgevoxOnnxWriteWave('./enhanced-online-dpdfnet.wav', Enhanced, Sd.GetSampleRate);
  WriteLn('Saved to ./enhanced-online-dpdfnet.wav');

  FreeAndNil(Sd);
end.
