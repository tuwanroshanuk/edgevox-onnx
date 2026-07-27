// Copyright (c)  2023-2024  Xiaomi Corporation (authors: Fangjun Kuang)
//
const cpal = require('node-cpal');


const edgevox_onnx = require('edgevox-onnx-node');

function createOnlineRecognizer() {
  const config = {
    'featConfig': {
      'sampleRate': 16000,
      'featureDim': 80,
    },
    'modelConfig': {
      'transducer': {
        'encoder':
            './edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/encoder-epoch-99-avg-1.onnx',
        'decoder':
            './edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/decoder-epoch-99-avg-1.onnx',
        'joiner':
            './edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/joiner-epoch-99-avg-1.onnx',
      },
      'tokens':
          './edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt',
      'numThreads': 2,
      'provider': 'cpu',
      'debug': 1,
    },
    'decodingMethod': 'greedy_search',
    'maxActivePaths': 4,
    'enableEndpoint': true,
    'rule1MinTrailingSilence': 2.4,
    'rule2MinTrailingSilence': 1.2,
    'rule3MinUtteranceLength': 20
  };

  return new edgevox_onnx.OnlineRecognizer(config);
}

const recognizer = createOnlineRecognizer();
const stream = recognizer.createStream();

let lastText = '';
let segmentIndex = 0;

const inputDevice = cpal.getDefaultInputDevice();
const deviceConfig = cpal.getDefaultInputConfig(inputDevice.deviceId);
const nativeSampleRate = deviceConfig.sampleRate;
const targetSampleRate = recognizer.config.featConfig.sampleRate;

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

      while (recognizer.isReady(stream)) {
        recognizer.decode(stream);
      }

      const isEndpoint = recognizer.isEndpoint(stream);
      const text = recognizer.getResult(stream).text.toLowerCase();

      if (text.length > 0 && lastText != text) {
        lastText = text;
        display.print(segmentIndex, lastText);
      }
      if (isEndpoint) {
        if (text.length > 0) {
          lastText = text;
          segmentIndex += 1;
        }
        recognizer.reset(stream);
      }
    });

console.log('Started! Please speak');
