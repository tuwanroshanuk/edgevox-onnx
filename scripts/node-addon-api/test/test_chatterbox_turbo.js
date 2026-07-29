'use strict';

const path = require('path');
const edgevox = require('../lib/edgevox-onnx.js');

const modelDir = process.env.CHATTERBOX_MODEL_DIR;
const referencePath = process.env.CHATTERBOX_REFERENCE_WAV;
if (!modelDir || !referencePath) {
  console.error(
      'Set CHATTERBOX_MODEL_DIR and CHATTERBOX_REFERENCE_WAV to run this test.');
  process.exit(2);
}

const graph = (name) => path.join(modelDir, 'onnx', `${name}_q4.onnx`);
const decoder = path.join(
    modelDir, 'onnx', 'conditional_decoder_quantized.onnx');
const started = performance.now();
const tts = new edgevox.OfflineTts({
  model: {
    chatterbox: {
      speechEncoder: graph('speech_encoder'),
      embedTokens: graph('embed_tokens'),
      languageModel: graph('language_model'),
      conditionalDecoder: process.env.CHATTERBOX_DECODER ||
          decoder,
      tokenizer: modelDir,
    },
    numThreads: Number(process.env.CHATTERBOX_THREADS || 4),
    provider: 'cpu',
  },
});
const initializedMs = performance.now() - started;
const reference = edgevox.readWave(referencePath, false);
const generationStarted = performance.now();
const audio = tts.generate({
  text: process.env.CHATTERBOX_TEXT ||
      'Hello from Chatterbox Turbo. [chuckle] Voice cloning is running locally.',
  generationConfig: {
    referenceAudio: reference.samples,
    referenceSampleRate: reference.sampleRate,
    extra: {
      max_new_tokens: Number(process.env.CHATTERBOX_MAX_TOKENS || 256),
      repetition_penalty: 1.2,
    },
  },
  enableExternalBuffer: false,
});
const generationMs = performance.now() - generationStarted;
const audioSeconds = audio.samples.length / audio.sampleRate;
let peak = 0;
let energy = 0;
for (const sample of audio.samples) {
  peak = Math.max(peak, Math.abs(sample));
  energy += sample * sample;
}
const rms = Math.sqrt(energy / audio.samples.length);
if (!audio.samples.length || audio.sampleRate !== 24000) {
  throw new Error(`Invalid generated audio: ${JSON.stringify({
    samples: audio.samples.length,
    sampleRate: audio.sampleRate,
  })}`);
}
if (peak < 0.05 || rms < 0.01) {
  throw new Error(
      `Chatterbox produced an invalid low-level waveform: peak=${peak}, rms=${rms}`);
}
const output = process.env.CHATTERBOX_OUTPUT_WAV ||
    path.join(modelDir, 'smoke-test.wav');
edgevox.writeWave(output, audio);
console.log(JSON.stringify({
  initializedMs,
  generationMs,
  audioSeconds,
  rtf: generationMs / 1000 / audioSeconds,
  sampleRate: audio.sampleRate,
  samples: audio.samples.length,
  peak,
  rms,
  output,
}, null, 2));
