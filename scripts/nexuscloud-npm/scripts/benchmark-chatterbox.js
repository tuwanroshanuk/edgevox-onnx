'use strict';

const path = require('path');
const edgevox = require('..');

const args = Object.fromEntries(process.argv.slice(2).map((item) => {
  const [key, ...value] = item.replace(/^--/, '').split('=');
  return [key, value.join('=')];
}));

if (!args['model-dir'] || !args['reference-wav']) {
  console.error(
      'Usage: --model-dir=DIR --reference-wav=FILE [--text=TEXT] [--threads=4]');
  process.exit(2);
}

const dir = path.resolve(args['model-dir']);
const model = (name) => path.join(dir, 'onnx', `${name}_q4.onnx`);
const reference = edgevox.readWave(path.resolve(args['reference-wav']));
const t0 = performance.now();
const tts = edgevox.createOfflineTts({
  offlineTtsModelConfig: {
    offlineTtsChatterboxModelConfig: {
      speechEncoder: model('speech_encoder'),
      embedTokens: model('embed_tokens'),
      languageModel: model('language_model'),
      conditionalDecoder: model('conditional_decoder'),
      tokenizer: dir,
    },
    numThreads: Number(args.threads || 4),
    provider: 'cpu',
  },
});
const initializedMs = performance.now() - t0;
const text = args.text || 'Hello from Chatterbox Turbo. [chuckle] This is fast local voice cloning.';
const started = performance.now();
const audio = tts.generateWithConfig(text, {
  referenceAudio: reference.samples,
  referenceSampleRate: reference.sampleRate,
  extra: {
    max_new_tokens: Number(args['max-new-tokens'] || 1024),
    repetition_penalty: Number(args['repetition-penalty'] || 1.2),
    normalize_output: 1,
    target_rms: Number(args['target-rms'] || 0.08),
    max_peak: 0.95,
  },
});
const generationMs = performance.now() - started;
const duration = audio.samples.length / audio.sampleRate;
console.log({
  runtime: edgevox.getRuntimeInfo(),
  initializedMs,
  generationMs,
  audioSeconds: duration,
  rtf: generationMs / 1000 / duration,
  sampleRate: audio.sampleRate,
});
if (args.output) tts.save(path.resolve(args.output), audio);
