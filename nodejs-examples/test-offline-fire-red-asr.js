// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)
//
const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let modelConfig = {
    fireRedAsr: {
      encoder:
          './edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/encoder.int8.onnx',
      decoder:
          './edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/decoder.int8.onnx',
    },
    tokens: './edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/tokens.txt',
    debug: 1,
  };

  let config = {
    modelConfig: modelConfig,
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './edgevox-onnx-fire-red-asr-large-zh_en-2025-02-16/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
