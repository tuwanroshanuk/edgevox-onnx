// Copyright (c)  2025  Xiaomi Corporation
import 'dart:io';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;
import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('model', help: 'Path to a GTCRN onnx model')
    ..addOption('input-wav', help: 'Path to input.wav')
    ..addOption('output-wav', help: 'Path to output.wav');

  final res = parser.parse(arguments);
  if (res['model'] == null ||
      res['input-wav'] == null ||
      res['output-wav'] == null) {
    print(parser.usage);
    exit(1);
  }

  final model = res['model'] as String;
  final inputWav = res['input-wav'] as String;
  final outputWav = res['output-wav'] as String;

  final config = edgevox_onnx.OfflineSpeechDenoiserConfig(
      model: edgevox_onnx.OfflineSpeechDenoiserModelConfig(
    gtcrn: edgevox_onnx.OfflineSpeechDenoiserGtcrnModelConfig(model: model),
    dpdfnet: const edgevox_onnx.OfflineSpeechDenoiserDpdfNetModelConfig(),
    numThreads: 1,
    debug: true,
    provider: 'cpu',
  ));

  final sd = edgevox_onnx.OfflineSpeechDenoiser(config);

  final waveData = edgevox_onnx.readWave(inputWav);

  final denoised =
      sd.run(samples: waveData.samples, sampleRate: waveData.sampleRate);

  sd.free();

  edgevox_onnx.writeWave(
      filename: outputWav,
      samples: denoised.samples,
      sampleRate: denoised.sampleRate);

  print('Saved to $outputWav');
}
