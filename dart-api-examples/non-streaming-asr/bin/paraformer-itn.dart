// Copyright (c)  2024  Xiaomi Corporation
import 'dart:io';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;

import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('model', help: 'Path to the paraformer model')
    ..addOption('tokens', help: 'Path to tokens.txt')
    ..addOption('rule-fsts',
        help: 'Path to rule fsts for inverse text normalization')
    ..addOption('input-wav', help: 'Path to input.wav to transcribe');

  final res = parser.parse(arguments);
  if (res['model'] == null ||
      res['tokens'] == null ||
      res['rule-fsts'] == null ||
      res['input-wav'] == null) {
    print(parser.usage);
    exit(1);
  }

  final model = res['model'] as String;
  final tokens = res['tokens'] as String;
  final ruleFsts = res['rule-fsts'] as String;
  final inputWav = res['input-wav'] as String;

  final paraformer = edgevox_onnx.OfflineParaformerModelConfig(
    model: model,
  );

  final modelConfig = edgevox_onnx.OfflineModelConfig(
    paraformer: paraformer,
    tokens: tokens,
    debug: true,
    numThreads: 1,
    modelType: 'paraformer',
  );
  final config = edgevox_onnx.OfflineRecognizerConfig(
    model: modelConfig,
    ruleFsts: ruleFsts,
  );
  final recognizer = edgevox_onnx.OfflineRecognizer(config);

  final waveData = edgevox_onnx.readWave(inputWav);
  final stream = recognizer.createStream();

  stream.acceptWaveform(
      samples: waveData.samples, sampleRate: waveData.sampleRate);
  recognizer.decode(stream);

  final result = recognizer.getResult(stream);
  print(result.text);

  stream.free();
  recognizer.free();
}
