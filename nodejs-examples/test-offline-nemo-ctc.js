// Copyright (c)  2023-2024  Xiaomi Corporation (authors: Fangjun Kuang)
//
const fs = require('fs');
const {Readable} = require('stream');
const wav = require('wav');

const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let config = {
    modelConfig: {
      nemoCtc: {
        model: './edgevox-onnx-nemo-ctc-en-conformer-small/model.int8.onnx',
      },
      tokens: './edgevox-onnx-nemo-ctc-en-conformer-small/tokens.txt',
    }
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './edgevox-onnx-nemo-ctc-en-conformer-small/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
