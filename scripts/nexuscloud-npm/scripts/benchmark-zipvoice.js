'use strict';

const {performance} = require('node:perf_hooks');
const {createHash} = require('node:crypto');
const path = require('node:path');
const edgevox = require('..');

function value(name, fallback) {
  const prefix = `--${name}=`;
  const arg = process.argv.find((item) => item.startsWith(prefix));
  return arg ? arg.slice(prefix.length) : fallback;
}

const modelDir = path.resolve(value('model-dir', ''));
const referencePath = path.resolve(value('reference-wav', ''));
const referenceText = value('reference-text', '');
const text = value('text', 'This is a ZipVoice benchmark.');
const threads = Math.max(1, Number(value('threads', '2')));
const steps = Math.max(1, Number(value('steps', '4')));
const seed = Number(value('seed', '1234'));
const runCount = Math.max(1, Number(value('runs', '2')));

if (!value('model-dir', '') || !value('reference-wav', '') || !referenceText) {
  console.error(
      'Usage: npm run benchmark:zipvoice -- ' +
      '--model-dir=... --reference-wav=... --reference-text=... ' +
      '[--text=...] [--threads=2] [--steps=4] [--seed=1234] [--runs=2]');
  process.exit(2);
}

const modelFile = (name) => path.join(modelDir, name);
const config = {
  offlineTtsModelConfig: {
    offlineTtsVitsModelConfig: {},
    offlineTtsMatchaModelConfig: {},
    offlineTtsKokoroModelConfig: {},
    offlineTtsKittenModelConfig: {},
    offlineTtsPocketModelConfig: {},
    offlineTtsSupertonicModelConfig: {},
    offlineTtsZipVoiceModelConfig: {
      tokens: modelFile(value('tokens', 'tokens.txt')),
      encoder: modelFile(value('encoder', 'encoder.int8.onnx')),
      decoder: modelFile(value('decoder', 'decoder.int8.onnx')),
      vocoder: modelFile(value('vocoder', 'vocos_24khz.onnx')),
      dataDir: modelFile(value('data-dir', 'espeak-ng-data')),
      lexicon: modelFile(value('lexicon', 'lexicon.txt')),
      espeakVoice: value('espeak-voice', 'en-us'),
      featScale: 0.1,
      tShift: 0.5,
      targetRMS: 0.1,
      guidanceScale: 1.0,
    },
    numThreads: threads,
    debug: 0,
    provider: 'cpu',
  },
  ruleFsts: '',
  ruleFars: '',
  maxNumSentences: 1,
  silenceScale: 0.2,
};

const initStart = performance.now();
const tts = edgevox.createOfflineTts(config);
const initMs = performance.now() - initStart;
const reference = edgevox.readWave(referencePath);

function generate(label) {
  const start = performance.now();
  const audio = tts.generateWithConfig(text, {
    speed: 1,
    numSteps: steps,
    silenceScale: 0.2,
    referenceAudio: reference.samples,
    referenceSampleRate: reference.sampleRate,
    referenceText,
    extra: {seed, min_char_in_sentence: 10},
  });
  const elapsedMs = performance.now() - start;
  const audioSeconds = audio.samples.length / audio.sampleRate;
  const sampleBytes = Buffer.from(
      audio.samples.buffer, audio.samples.byteOffset, audio.samples.byteLength);
  return {
    label,
    elapsedMs: Number(elapsedMs.toFixed(1)),
    audioSeconds: Number(audioSeconds.toFixed(3)),
    rtf: Number((elapsedMs / 1000 / audioSeconds).toFixed(3)),
    sha256: createHash('sha256').update(sampleBytes).digest('hex'),
  };
}

const runs = Array.from(
    {length: runCount},
    (_, index) => generate(index === 0 ? 'cold' : `warm-${index}`));
const currentRssMiB = process.memoryUsage().rss / 1024 / 1024;
const resourceMaxRssMiB = process.resourceUsage().maxRSS / 1024;
console.log(JSON.stringify({
  runtime: edgevox.getRuntimeInfo(),
  modelDir,
  precision: value('decoder', 'decoder.int8.onnx').includes('int8') ?
      'int8' : 'float',
  threads,
  steps,
  seed,
  initMs: Number(initMs.toFixed(1)),
  memoryRssMiB: Number(currentRssMiB.toFixed(1)),
  peakMemoryRssMiB: Number(Math.max(currentRssMiB, resourceMaxRssMiB).toFixed(1)),
  runs,
}, null, 2));
tts.free();
