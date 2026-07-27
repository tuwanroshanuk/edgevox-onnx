// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let modelConfig = {
    paraformer: {
      model: './edgevox-onnx-paraformer-zh-2023-09-14/model.int8.onnx',
    },
    tokens: './edgevox-onnx-paraformer-zh-2023-09-14/tokens.txt',
  };

  let config = {
    modelConfig: modelConfig,
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename = './edgevox-onnx-paraformer-zh-2023-09-14/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
