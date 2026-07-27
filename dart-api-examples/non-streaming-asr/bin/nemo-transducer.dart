// Copyright (c)  2024  Xiaomi Corporation
import 'dart:io';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;

import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('encoder', help: 'Path to the encoder model')
    ..addOption('decoder', help: 'Path to decoder model')
    ..addOption('joiner', help: 'Path to joiner model')
    ..addOption('tokens', help: 'Path to tokens.txt')
    ..addOption('input-wav', help: 'Path to input.wav to transcribe');

  final res = parser.parse(arguments);
  if (res['encoder'] == null ||
      res['decoder'] == null ||
      res['joiner'] == null ||
      res['tokens'] == null ||
      res['input-wav'] == null) {
    print(parser.usage);
    exit(1);
  }

  final encoder = res['encoder'] as String;
  final decoder = res['decoder'] as String;
  final joiner = res['joiner'] as String;
  final tokens = res['tokens'] as String;
  final inputWav = res['input-wav'] as String;

  final transducer = edgevox_onnx.OfflineTransducerModelConfig(
    encoder: encoder,
    decoder: decoder,
    joiner: joiner,
  );

  final modelConfig = edgevox_onnx.OfflineModelConfig(
    transducer: transducer,
    tokens: tokens,
    debug: true,
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
