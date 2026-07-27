// Copyright (c)  2024  Xiaomi Corporation
const edgevox_onnx = require('edgevox-onnx-node');

// Please download test files from
// https://github.com/k2-fsa/edgevox-onnx/releases/tag/kws-models
const config = {
  'featConfig': {
    'sampleRate': 16000,
    'featureDim': 80,
  },
  'modelConfig': {
    'transducer': {
      'encoder':
          './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/encoder-epoch-12-avg-2-chunk-16-left-64.onnx',
      'decoder':
          './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/decoder-epoch-12-avg-2-chunk-16-left-64.onnx',
      'joiner':
          './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/joiner-epoch-12-avg-2-chunk-16-left-64.onnx',
    },
    'tokens':
        './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/tokens.txt',
    'numThreads': 1,
    'provider': 'cpu',
    'debug': 1,
  },
  'keywordsFile':
      './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/test_wavs/test_keywords.txt',
};

const waveFilename =
    './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/test_wavs/3.wav';

const kws = new edgevox_onnx.KeywordSpotter(config);
console.log('Started');
let start = Date.now();
const stream = kws.createStream();
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform({sampleRate: wave.sampleRate, samples: wave.samples});

const tailPadding = new Float32Array(wave.sampleRate * 0.4);
stream.acceptWaveform({samples: tailPadding, sampleRate: wave.sampleRate});

const detectedKeywords = [];
while (kws.isReady(stream)) {
  kws.decode(stream);
  const keyword = kws.getResult(stream).keyword;
  if (keyword != '') {
    detectedKeywords.push(keyword);
  }
}
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
console.log('result\n', detectedKeywords);
