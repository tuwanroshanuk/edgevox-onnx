// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)
//
const fs = require('fs');
const {Readable} = require('stream');

const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let config = {
    modelConfig: {
      medasr: {
        model: './edgevox-onnx-medasr-ctc-en-int8-2025-12-25/model.int8.onnx',
      },
      tokens: './edgevox-onnx-medasr-ctc-en-int8-2025-12-25/tokens.txt',
    }
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './edgevox-onnx-medasr-ctc-en-int8-2025-12-25/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
