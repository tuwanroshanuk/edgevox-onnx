// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)
//
const edgevox_onnx = require('edgevox-onnx');

function createOfflineRecognizer() {
  const config = {
    modelConfig: {
      qwen3Asr: {
        convFrontend:
            './sherpa-onnx-qwen3-asr-0.6B-int8-2026-03-25/conv_frontend.onnx',
        encoder:
            './sherpa-onnx-qwen3-asr-0.6B-int8-2026-03-25/encoder.int8.onnx',
        decoder:
            './sherpa-onnx-qwen3-asr-0.6B-int8-2026-03-25/decoder.int8.onnx',
        tokenizer: './sherpa-onnx-qwen3-asr-0.6B-int8-2026-03-25/tokenizer',
        hotwords: '',
      },
      tokens: '',
    }
  };

  return edgevox_onnx.createOfflineRecognizer(config);
}

const recognizer = createOfflineRecognizer();
const stream = recognizer.createStream();

const waveFilename =
    './sherpa-onnx-qwen3-asr-0.6B-int8-2026-03-25/test_wavs/raokouling.wav';
const wave = edgevox_onnx.readWave(waveFilename);
stream.acceptWaveform(wave.sampleRate, wave.samples);

recognizer.decode(stream);
const text = recognizer.getResult(stream).text;
console.log(text);

stream.free();
recognizer.free();
