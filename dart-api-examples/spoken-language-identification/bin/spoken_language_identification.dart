// Copyright (c)  2024  Xiaomi Corporation
import 'dart:io';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;
import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('encoder', help: 'Path to the whisper encoder model')
    ..addOption('decoder', help: 'Path to the whisper decoder model')
    ..addOption('tail-paddings', help: 'Tail paddings for the whisper model', defaultsTo: '0')
    ..addOption('wav', help: 'Path to test.wav for language identification')
    ..addFlag('help', abbr: 'h', help: 'Show this help message', negatable: false);

  final res = parser.parse(arguments);
  if (res['help'] as bool) {
    print(parser.usage);
    exit(0);
  }

  if (res['encoder'] == null || res['decoder'] == null || res['wav'] == null) {
    print(parser.usage);
    exit(1);
  }

  final encoder = res['encoder'] as String;
  final decoder = res['decoder'] as String;
  final tailPaddings = int.tryParse(res['tail-paddings'] as String) ?? 0;
  final wav = res['wav'] as String;

  final whisperConfig = edgevox_onnx.SpokenLanguageIdentificationWhisperConfig(
    encoder: encoder,
    decoder: decoder,
    tailPaddings: tailPaddings,
  );

  final config = edgevox_onnx.SpokenLanguageIdentificationConfig(
    whisper: whisperConfig,
    numThreads: 1,
    debug: true,
    provider: 'cpu',
  );

  final slid = edgevox_onnx.SpokenLanguageIdentification(config);

  final waveData = edgevox_onnx.readWave(wav);

  final stream = slid.createStream();
  stream.acceptWaveform(samples: waveData.samples, sampleRate: waveData.sampleRate);

  final result = slid.compute(stream);

  print('File: $wav');
  print('Detected language: ${result.lang}');

  stream.free();
  slid.free();
}
