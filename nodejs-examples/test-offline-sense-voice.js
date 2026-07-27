// Copyright (c)  2024  Xiaomi Corporation (authors: Fangjun Kuang)

const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  let modelConfig = {
    senseVoice: {
      model:
          './edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/model.int8.onnx',
      language: '',
      useInverseTextNormalization: 1,
    },
    tokens:
        './edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/tokens.txt',
  };

  let config = {
    modelConfig: modelConfig,
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './edgevox-onnx-sense-voice-zh-en-ja-ko-yue-int8-2024-07-17/test_wavs/zh.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
