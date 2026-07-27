// Copyright (c)  2024  Xiaomi Corporation
const edgevox_onnx = require('edgevox-onnx-node');

// Please download test files from
// https://github.com/k2-fsa/edgevox-onnx/releases/tag/asr-models
const config = {
  'featConfig': {
    'sampleRate': 16000,
    'featureDim': 80,
  },
  'modelConfig': {
    'paraformer': {
      'encoder':
          './edgevox-onnx-streaming-paraformer-bilingual-zh-en/encoder.int8.onnx',
      'decoder':
          './edgevox-onnx-streaming-paraformer-bilingual-zh-en/decoder.int8.onnx',
    },
    'tokens': './edgevox-onnx-streaming-paraformer-bilingual-zh-en/tokens.txt',
    'numThreads': 2,
    'provider': 'cpu',
    'debug': 1,
  }
};

const waveFilename =
    './edgevox-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/0.wav';

const recognizer = new edgevox_onnx.OnlineRecognizer(config);
console.log('Started');
let start = Date.now();
const stream = recognizer.createStream();
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform({sampleRate: wave.sampleRate, samples: wave.samples});

const tailPadding = new Float32Array(wave.sampleRate * 0.4);
stream.acceptWaveform({samples: tailPadding, sampleRate: wave.sampleRate});

while (recognizer.isReady(stream)) {
  recognizer.decode(stream);
}
const result = recognizer.getResult(stream);
let stop = Date.now();
console.log('Done');

const elapsed_seconds = (stop - start) / 1000;
const duration = wave.samples.length / wave.sampleRate;
const real_time_factor = elapsed_seconds / duration;
console.log('Wave duration', duration.toFixed(3), 'seconds');
console.log('Elapsed', elapsed_seconds.toFixed(3), 'seconds');
console.log(
    `RTF = ${elapsed_seconds.toFixed(3)}/${duration.toFixed(3)} =`,
    real_time_factor.toFixed(3));
console.log(waveFilename);
console.log('result\n', result);
