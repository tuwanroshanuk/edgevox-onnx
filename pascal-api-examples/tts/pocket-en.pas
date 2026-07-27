{ Copyright (c)  2026  Xiaomi Corporation }
program pocket_en;
{
This file shows how to use the text to speech API of edgevox-onnx
with Pocket TTS models.

It generates speech from text and saves it to a wave file.
}

{$mode objfpc}

uses
  ctypes,
  SysUtils,
  edgevox_onnx;

function ProgressCallback(Samples: pcfloat; N: cint32; P: cfloat;
  Arg: Pointer): cint32; cdecl;
begin
  WriteLn(Format('Progress: %.2f%%, samples: %d', [P * 100.0, N]));
  Result := 1;
end;

function GetOfflineTts: TEdgevoxOnnxOfflineTts;
var
  Config: TEdgevoxOnnxOfflineTtsConfig;
begin
  Config.Model.Pocket.LmFlow := './edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx';
  Config.Model.Pocket.LmMain := './edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx';
  Config.Model.Pocket.Encoder := './edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx';
  Config.Model.Pocket.Decoder := './edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx';
  Config.Model.Pocket.TextConditioner := './edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx';
  Config.Model.Pocket.VocabJson := './edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json';
  Config.Model.Pocket.TokenScoresJson := './edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json';
  Config.Model.NumThreads := 2;
  Config.Model.Debug := True;
  Config.MaxNumSentences := 1;

  Result := TEdgevoxOnnxOfflineTts.Create(Config);
end;

var
  Tts: TEdgevoxOnnxOfflineTts;
  GenerationConfig: TEdgevoxOnnxGenerationConfig;
  Wave: TEdgevoxOnnxWave;
  WaveFilename: AnsiString;
  Audio: TEdgevoxOnnxGeneratedAudio;

  Text: AnsiString;

begin
  Tts := GetOfflineTts;

  WriteLn('There are ', Tts.GetNumSpeakers, ' speakers');

  Text := 'Friends fell out often because life was changing so fast. The easiest thing in the world was to lose touch with someone.';

  WaveFilename := './edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav';
  Wave := EdgevoxOnnxReadWave(WaveFilename);
  GenerationConfig.ReferenceAudio := Wave.Samples;
  GenerationConfig.ReferenceAudioLen := Length(Wave.Samples);
  GenerationConfig.ReferenceSampleRate := Wave.SampleRate;

  Audio := Tts.Generate(Text, GenerationConfig, @ProgressCallback, NIL);
  EdgevoxOnnxWriteWave('./pocket-tts-en.wav', Audio.Samples, Audio.SampleRate);
  WriteLn('Saved to ./pocket-tts-en.wav');

  FreeAndNil(Tts);
end.
