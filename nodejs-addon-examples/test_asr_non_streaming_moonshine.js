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
    'moonshine': {
      'preprocessor': './edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx',
      'encoder': './edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx',
      'uncachedDecoder':
          './edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx',
      'cachedDecoder':
          './edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx',
    },
    'tokens': './edgevox-onnx-moonshine-tiny-en-int8/tokens.txt',
    'numThreads': 2,
    'provider': 'cpu',
    'debug': 1,
  }
};

const waveFilename = './edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav';

const recognizer = new edgevox_onnx.OfflineRecognizer(config);
console.log('Started');
let start = Date.now();
const stream = recognizer.createStream();
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform({sampleRate: wave.sampleRate, samples: wave.samples});

recognizer.decode(stream);
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
