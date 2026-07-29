'use strict';

const path = require('node:path');
const edgevox = require('..');

function value(name, fallback = '') {
  const prefix = `--${name}=`;
  const arg = process.argv.find((item) => item.startsWith(prefix));
  return arg ? arg.slice(prefix.length) : fallback;
}

if (!value('model-dir')) {
  console.error(
      'Usage: npm run benchmark:wfloat -- --model-dir=DIR ' +
      '[--reference-wav=FILE] [--emotion=joy] [--intensity=0.7] ' +
      '[--speaker=10] [--threads=2] [--output=FILE]');
  process.exit(2);
}

const modelDir = path.resolve(value('model-dir'));
const referencePath = value('reference-wav');
const openvoiceDir = path.join(modelDir, 'openvoice_v2');
const vits = {
  model: path.join(modelDir, 'wfloat_emotional_vits.onnx'),
  tokens: path.join(modelDir, 'tokens.txt'),
  dataDir: path.join(modelDir, 'espeak-ng-data'),
  noiseScale: 0.667,
  noiseScaleW: 0.8,
  lengthScale: 1,
};
if (referencePath) {
  vits.openvoiceToneEncoder =
      path.join(openvoiceDir, 'tone_ref_encoder.onnx');
  vits.openvoiceToneConverter =
      path.join(openvoiceDir, 'tone_converter.onnx');
}

const initStarted = performance.now();
const tts = edgevox.createOfflineTts({
  offlineTtsModelConfig: {
    offlineTtsVitsModelConfig: vits,
    numThreads: Math.max(1, Number(value('threads', '2'))),
    provider: 'cpu',
  },
  maxNumSentences: 1,
});
const initializedMs = performance.now() - initStarted;

const generationConfig = {
  sid: Number(value('speaker', '10')),
  speed: 1,
  extra: {
    emotion: value('emotion', 'joy'),
    emotion_intensity: Number(value('intensity', '0.7')),
  },
};
if (referencePath) {
  const reference = edgevox.readWave(path.resolve(referencePath));
  generationConfig.referenceAudio = reference.samples;
  generationConfig.referenceSampleRate = reference.sampleRate;
}

const started = performance.now();
const audio = tts.generateWithConfig(
    value('text', 'This is a clear English emotional voice benchmark.'),
    generationConfig);
const generationMs = performance.now() - started;
const audioSeconds = audio.samples.length / audio.sampleRate;
const output = value('output');
if (output) {
  edgevox.writeWave(path.resolve(output), audio);
}
console.log(JSON.stringify({
  runtime: edgevox.getRuntimeInfo(),
  initializedMs,
  generationMs,
  audioSeconds,
  rtf: generationMs / 1000 / audioSeconds,
  sampleRate: audio.sampleRate,
  samples: audio.samples.length,
  cloned: Boolean(referencePath),
  output: output ? path.resolve(output) : null,
}, null, 2));
