// Copyright (c)  2024  Xiaomi Corporation
import 'dart:io';
import 'dart:typed_data';

import 'package:args/args.dart';
import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;

import './init.dart';

void main(List<String> arguments) async {
  await initEdgevoxOnnx();

  final parser = ArgParser()
    ..addOption('model', help: 'Path to the ONNX model')
    ..addOption('tokens', help: 'Path to tokens.txt')
    ..addOption('data-dir', help: 'Path to espeak-ng-data directory')
    ..addOption('text', help: 'Text to generate TTS for')
    ..addOption('output-wav', help: 'Filename to save the generated audio')
    ..addOption('speed', help: 'Speech speed', defaultsTo: '1.0')
    ..addOption(
      'sid',
      help: 'Speaker ID to select. Used only for multi-speaker TTS',
      defaultsTo: '0',
    );
  final res = parser.parse(arguments);
  if (res['model'] == null ||
      res['tokens'] == null ||
      res['data-dir'] == null ||
      res['output-wav'] == null ||
      res['text'] == null) {
    print(parser.usage);
    exit(1);
  }
  final model = res['model'] as String;
  final tokens = res['tokens'] as String;
  final dataDir = res['data-dir'] as String;
  final text = res['text'] as String;
  final outputWav = res['output-wav'] as String;
  var speed = double.tryParse(res['speed'] as String) ?? 1.0;
  final sid = int.tryParse(res['sid'] as String) ?? 0;

  if (speed == 0) {
    speed = 1.0;
  }

  final vits = edgevox_onnx.OfflineTtsVitsModelConfig(
    model: model,
    tokens: tokens,
    dataDir: dataDir,
  );

  final modelConfig = edgevox_onnx.OfflineTtsModelConfig(
    vits: vits,
    numThreads: 1,
    debug: true,
  );
  final config = edgevox_onnx.OfflineTtsConfig(
    model: modelConfig,
    maxNumSenetences: 1,
  );

  final tts = edgevox_onnx.OfflineTts(config);
  final genConfig = edgevox_onnx.OfflineTtsGenerationConfig(
    sid: sid,
    speed: speed,
    silenceScale: 0.2,
  );
  final audio = tts.generateWithConfig(
      text: text,
      config: genConfig,
      onProgress: (Float32List samples, double progress) {
        print('${samples.length} samples received');
        // You can play samples in a separate thread/isolate

        // 1 means to continue
        // 0 means to stop
        return 1;
      });
  tts.free();

  edgevox_onnx.writeWave(
    filename: outputWav,
    samples: audio.samples,
    sampleRate: audio.sampleRate,
  );
  print('Saved to $outputWav');
}
