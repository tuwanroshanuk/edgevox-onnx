// Copyright (c)  2023-2024  Xiaomi Corporation (authors: Fangjun Kuang)
//
const cpal = require('node-cpal');


const edgevox_onnx = require('edgevox-onnx-node');

function createKeywordSpotter() {
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
      'numThreads': 2,
      'provider': 'cpu',
      'debug': 1,
    },
    'keywordsFile':
        './edgevox-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01/keywords.txt',
  };

  return new edgevox_onnx.KeywordSpotter(config);
}

const kws = createKeywordSpotter();
const stream = kws.createStream();

let lastText = '';
let segmentIndex = 0;

const inputDevice = cpal.getDefaultInputDevice();
const deviceConfig = cpal.getDefaultInputConfig(inputDevice.deviceId);
const nativeSampleRate = deviceConfig.sampleRate;
const targetSampleRate = kws.config.featConfig.sampleRate;

const resampler = new edgevox_onnx.LinearResampler(nativeSampleRate, targetSampleRate);
const display = new edgevox_onnx.Display(50);

const inputStream = cpal.createStream(
    inputDevice.deviceId,
    true,
    {
      sampleRate: nativeSampleRate,
      channels: 1,
      format: 'f32',
    },
    (data) => {
      const resampled = resampler.resample(data);
      stream.acceptWaveform(
          {sampleRate: targetSampleRate, samples: resampled});

      while (kws.isReady(stream)) {
        kws.decode(stream);
      }

      const keyword = kws.getResult(stream).keyword;
      if (keyword != '') {
        display.print(segmentIndex, keyword);
        segmentIndex += 1;
      }
    });

console.log('Started! Please speak.');
console.log(`Only words from ${kws.config.keywordsFile} can be recognized`);
