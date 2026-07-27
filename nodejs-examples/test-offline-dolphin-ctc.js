// Copyright (c)  2025  Xiaomi Corporation (authors: Fangjun Kuang)
//
const fs = require('fs');
const {Readable} = require('stream');
const wav = require('wav');

const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let config = {
    modelConfig: {
      dolphin: {
        model:
            './sherpa-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/model.int8.onnx',
      },
      tokens:
          './sherpa-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/tokens.txt',
    }
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './sherpa-onnx-dolphin-base-ctc-multi-lang-int8-2025-04-02/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
