// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
//
const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let modelConfig = {
    moonshine: {
      preprocessor: './edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx',
      encoder: './edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx',
      uncachedDecoder:
          './edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx',
      cachedDecoder:
          './edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx',
    },
    tokens: './edgevox-onnx-moonshine-tiny-en-int8/tokens.txt',
  };

  let config = {
    modelConfig: modelConfig,
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename = './edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
