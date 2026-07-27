{ Copyright (c)  2024  Xiaomi Corporation }
program main;

{$mode objfpc}

uses
  edgevox_onnx;

var
  Wave: TEdgevoxOnnxWave;
  S: Single;
  I: Integer;
begin
  Wave := EdgevoxOnnxReadWave('./lei-jun-test.wav');
  WriteLn('info ', Wave.SampleRate, ' ', Length(Wave.Samples));
  S := 0;
  for i := Low(Wave.Samples) to High(Wave.Samples) do
    S += Wave.Samples[i];

  WriteLn('sum is ', S);
end.
