// Copyright (c)  2026  Xiaomi Corporation
const edgevox_onnx = require('edgevox-onnx-node');

const config = {
  'featConfig': {
    'sampleRate': 16000,
    'featureDim': 80,
  },
  'modelConfig': {
    'qwen3Asr': {
      'convFrontend':
          './edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/conv_frontend.onnx',
      'encoder':
          './edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx',
      'decoder':
          './edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/decoder.int8.onnx',
      'tokenizer': './edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/tokenizer',
      'hotwords': '',
    },
    'tokens': '',
    'numThreads': 2,
    'provider': 'cpu',
    'debug': 1,
  }
};

const waveFilename =
    './edgevox-onnx-qwen3-asr-0.6B-int8-2026-03-25/test_wavs/raokouling.wav';

const recognizer = new edgevox_onnx.OfflineRecognizer(config);
const stream = recognizer.createStream();
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform({sampleRate: wave.sampleRate, samples: wave.samples});

recognizer.decode(stream);
const result = recognizer.getResult(stream);
console.log('result\n', result);
