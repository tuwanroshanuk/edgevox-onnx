// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
//
const edgevox_onnx = require('edgevox-onnx');
console.log(`version : ${edgevox_onnx.version}`);
console.log(`git sha1: ${edgevox_onnx.gitSha1}`);
console.log(`git date: ${edgevox_onnx.gitDate}`);

function createOfflineRecognizer() {
  let modelConfig = {
    whisper: {
      encoder: './edgevox-onnx-whisper-tiny.en/tiny.en-encoder.int8.onnx',
      decoder: './edgevox-onnx-whisper-tiny.en/tiny.en-decoder.int8.onnx',
      language: '',
      task: 'transcribe',
      tailPaddings: -1,
    },
    tokens: './edgevox-onnx-whisper-tiny.en/tiny.en-tokens.txt',
  };

  let config = {
    modelConfig: modelConfig,
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename = './edgevox-onnx-whisper-tiny.en/test_wavs/0.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
