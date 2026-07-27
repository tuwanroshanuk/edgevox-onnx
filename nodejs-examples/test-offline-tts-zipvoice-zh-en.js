// Copyright (c)  2026  Xiaomi Corporation (authors: Fangjun Kuang)

const edgevox_onnx = require('edgevox-onnx');

function createOfflineTts() {
  const zipvoice = {
    encoder: './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx',
    decoder: './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/decoder.int8.onnx',
    tokens: './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/tokens.txt',
    lexicon: './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/lexicon.txt',
    dataDir: './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/espeak-ng-data',
    vocoder: './vocos_24khz.onnx',
    espeakVoice: 'en-us',
  };

  const offlineTtsModelConfig = {
    offlineTtsZipVoiceModelConfig: zipvoice,
    numThreads: 1,
    debug: 1,  // set it to 1 to see verbose logs; 0 to disable logs
    provider: 'cpu',
  };

  const offlineTtsConfig = {
    offlineTtsModelConfig: offlineTtsModelConfig,
    maxNumSentences: 1,
  };

  return edgevox_onnx.createOfflineTts(offlineTtsConfig);
}

const referenceWaveFilename =
    './edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/test_wavs/leijun-1.wav';
const wave = edgevox_onnx.readWave(referenceWaveFilename);

const referenceText =
    '那还是三十六年前, 一九八七年. 我呢考上了武汉大学的计算机系.';
const text =
    '小米的价值观是真诚, 热爱. 真诚，就是不欺人也不自欺. 热爱, 就是全心投入并享受其中.';

const generationConfig = {
  referenceAudio: wave.samples,
  referenceSampleRate: wave.sampleRate,
  // It must match the transcript of the reference audio above.
  referenceText: referenceText,
  numSteps: 4,
  extra: {min_char_in_sentence: 10},
};

const tts = createOfflineTts();
const audio = tts.generateWithConfig(text, generationConfig);
tts.save('./test-zipvoice-zh-en.wav', audio);
console.log('Saved to test-zipvoice-zh-en.wav successfully.');
tts.free();
