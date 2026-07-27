{ Copyright (c)  2025  Xiaomi Corporation }
{
This file shows how to use the speech enhancement API from edgevox-onnx

Please first download files used in this script before you run it.

wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/gtcrn_simple.onnx
wget https://github.com/k2-fsa/sherpa-onnx/releases/download/speech-enhancement-models/inp_16k.wav
}
program main;

{$mode delphi}

uses
  edgevox_onnx,
  SysUtils;

var
  Wave: TEdgevoxOnnxWave;
  Model: AnsiString;

  Config: TEdgevoxOnnxOfflineSpeechDenoiserConfig;
  Sd: TEdgevoxOnnxOfflineSpeechDenoiser;
  Audio: TEdgevoxOnnxDenoisedAudio;
begin
  Wave := EdgevoxOnnxReadWave('./inp_16k.wav');
  Model := './gtcrn_simple.onnx';

  Initialize(Config);
  Config.Model.Gtcrn.Model := Model;
  Config.Model.NumThreads:= 1;
  Config.Model.Debug:= True;
  Config.Model.Provider:= 'cpu';

  Sd := TEdgevoxOnnxOfflineSpeechDenoiser.Create(Config);

  Audio := Sd.Run(Wave.Samples, Wave.SampleRate);

  EdgevoxOnnxWriteWave('./enhanced.wav', Audio.Samples, Audio.SampleRate);
  WriteLn('Saved to ./enhanced.wav');

  FreeAndNil(Sd);
end.
