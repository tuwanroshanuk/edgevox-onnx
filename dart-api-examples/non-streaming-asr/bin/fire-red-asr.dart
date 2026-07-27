// Copyright (c)  2025  Xiaomi Corporation
import 'dart:io';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;

import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('encoder', help: 'Path to the FireRedAsr encoder model')
    ..addOption('decoder', help: 'Path to FireRedAsr decoder model')
    ..addOption('tokens', help: 'Path to tokens.txt')
    ..addOption('input-wav', help: 'Path to input.wav to transcribe');

  final res = parser.parse(arguments);
  if (res['encoder'] == null ||
      res['decoder'] == null ||
      res['tokens'] == null ||
      res['input-wav'] == null) {
    print(parser.usage);
    exit(1);
  }

  final encoder = res['encoder'] as String;
  final decoder = res['decoder'] as String;
  final tokens = res['tokens'] as String;
  final inputWav = res['input-wav'] as String;

  final fireRedAsr = edgevox_onnx.OfflineFireRedAsrModelConfig(
    encoder: encoder,
    decoder: decoder,
  );

  final modelConfig = edgevox_onnx.OfflineModelConfig(
    fireRedAsr: fireRedAsr,
    tokens: tokens,
    debug: false,
    numThreads: 1,
  );
  final config = edgevox_onnx.OfflineRecognizerConfig(model: modelConfig);
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
