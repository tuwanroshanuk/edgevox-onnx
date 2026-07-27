{ Copyright (c)  2026  Xiaomi Corporation }
program supertonic_en;
{
This file shows how to use the text to speech API of edgevox-onnx
with Supertonic TTS models.

It generates speech from text and saves it to a wave file.

Please visit
https://k2-fsa.github.io/sherpa/onnx/tts/supertonic.html
to download the model.
}

{$mode objfpc}

uses
  SysUtils,
  edgevox_onnx;

function GetOfflineTts: TEdgevoxOnnxOfflineTts;
var
  Config: TEdgevoxOnnxOfflineTtsConfig;
begin
  Config.Model.Supertonic.DurationPredictor := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/duration_predictor.int8.onnx';
  Config.Model.Supertonic.TextEncoder := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/text_encoder.int8.onnx';
  Config.Model.Supertonic.VectorEstimator := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/vector_estimator.int8.onnx';
  Config.Model.Supertonic.Vocoder := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/vocoder.int8.onnx';
  Config.Model.Supertonic.TtsJson := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/tts.json';
  Config.Model.Supertonic.UnicodeIndexer := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/unicode_indexer.bin';
  Config.Model.Supertonic.VoiceStyle := './edgevox-onnx-supertonic-3-tts-int8-2026-05-11/voice.bin';
  Config.Model.NumThreads := 2;
  Config.Model.Debug := True;
  Config.MaxNumSentences := 1;

  Result := TEdgevoxOnnxOfflineTts.Create(Config);
end;

var
  Tts: TEdgevoxOnnxOfflineTts;
  GenerationConfig: TEdgevoxOnnxGenerationConfig;
  Audio: TEdgevoxOnnxGeneratedAudio;
  Text: AnsiString;

begin
  Tts := GetOfflineTts;

  WriteLn('There are ', Tts.GetNumSpeakers, ' speakers');

  Text := 'Today as always, men fall into two groups: slaves and free men. Whoever ' +
    'does not have two-thirds of his day for himself, is a slave, whatever ' +
    'he may be: a statesman, a businessman, an official, or a scholar.';

  GenerationConfig.Sid := 6;
  GenerationConfig.NumSteps := 8;
  GenerationConfig.Speed := 1.25;
  GenerationConfig.Extra := '{"lang": "en"}';

  Audio := Tts.Generate(Text, GenerationConfig, NIL, NIL);
  EdgevoxOnnxWriteWave('./supertonic-tts-en.wav', Audio.Samples, Audio.SampleRate);
  WriteLn('Saved to ./supertonic-tts-en.wav');

  FreeAndNil(Tts);
end.
