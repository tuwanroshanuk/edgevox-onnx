{ Copyright (c)  2026  Xiaomi Corporation }
program zipvoice_zh_en;
{
This file shows how to use the text to speech API of edgevox-onnx
with ZipVoice TTS models.

It generates speech from text and saves it to a wave file.

Please visit
https://k2-fsa.github.io/sherpa/onnx/tts/zipvoice.html
to download the model.
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
  Config := Default(TEdgevoxOnnxOfflineTtsConfig);
  Config.Model.ZipVoice.Tokens := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/tokens.txt';
  Config.Model.ZipVoice.Encoder := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx';
  Config.Model.ZipVoice.Decoder := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/decoder.int8.onnx';
  Config.Model.ZipVoice.Vocoder := './vocos_24khz.onnx';
  Config.Model.ZipVoice.DataDir := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/espeak-ng-data';
  Config.Model.ZipVoice.Lexicon := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/lexicon.txt';
  Config.Model.ZipVoice.EspeakVoice := 'en-us';
  Config.Model.ZipVoice.FeatScale := 0.1;
  Config.Model.ZipVoice.Tshift := 0.5;
  Config.Model.ZipVoice.TargetRms := 0.1;
  Config.Model.ZipVoice.GuidanceScale := 1.0;
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
  ReferenceText: AnsiString;

begin
  Tts := GetOfflineTts;

  WriteLn('There are ', Tts.GetNumSpeakers, ' speakers');

  Text := '小米的价值观是真诚, 热爱. 真诚，就是不欺人也不自欺. 热爱, 就是全心投入并享受其中.';
  ReferenceText := '那还是三十六年前, 一九八七年. 我呢考上了武汉大学的计算机系.';

  WaveFilename := './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/test_wavs/leijun-1.wav';
  Wave := EdgevoxOnnxReadWave(WaveFilename);
  GenerationConfig := Default(TEdgevoxOnnxGenerationConfig);
  GenerationConfig.SilenceScale := 0.2;
  GenerationConfig.Speed := 1.0;
  GenerationConfig.Sid := 0;
  GenerationConfig.ReferenceAudio := Wave.Samples;
  GenerationConfig.ReferenceAudioLen := Length(Wave.Samples);
  GenerationConfig.ReferenceSampleRate := Wave.SampleRate;
  GenerationConfig.ReferenceText := ReferenceText;
  GenerationConfig.NumSteps := 4;
  GenerationConfig.Extra := '{"min_char_in_sentence": "10"}';

  Audio := Tts.Generate(Text, GenerationConfig, @ProgressCallback, NIL);
  EdgevoxOnnxWriteWave('./zipvoice-zh-en.wav', Audio.Samples, Audio.SampleRate);
  WriteLn('Saved to ./zipvoice-zh-en.wav');

  FreeAndNil(Tts);
end.
