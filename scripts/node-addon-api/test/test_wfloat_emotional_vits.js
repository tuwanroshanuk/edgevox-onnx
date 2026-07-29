'use strict';

const fs = require('fs');
const path = require('path');
const edgevox = require('../lib/edgevox-onnx.js');

const modelDir = process.env.WFLOAT_MODEL_DIR;
if (!modelDir) {
  console.error('Set WFLOAT_MODEL_DIR to run this test.');
  process.exit(2);
}

const referencePath = process.env.WFLOAT_REFERENCE_WAV || '';
const openvoiceDir = path.join(modelDir, 'openvoice_v2');
const outputPrefix = process.env.WFLOAT_OUTPUT_PREFIX ||
    path.join(modelDir, 'smoke-test-node');
const model = {
  model: path.join(modelDir, 'wfloat_emotional_vits.onnx'),
  tokens: path.join(modelDir, 'tokens.txt'),
  dataDir: path.join(modelDir, 'espeak-ng-data'),
  noiseScale: 0.667,
  noiseScaleW: 0.8,
  lengthScale: 1.0,
};
if (process.env.WFLOAT_DISABLE_OPENVOICE !== '1' &&
    fs.existsSync(path.join(openvoiceDir, 'tone_ref_encoder.onnx')) &&
    fs.existsSync(path.join(openvoiceDir, 'tone_converter.onnx'))) {
  model.openvoiceToneEncoder =
      path.join(openvoiceDir, 'tone_ref_encoder.onnx');
  model.openvoiceToneConverter =
      path.join(openvoiceDir, 'tone_converter.onnx');
}

const started = performance.now();
const tts = new edgevox.OfflineTts({
  model: {
    vits: model,
    numThreads: Number(process.env.WFLOAT_THREADS || 2),
    provider: 'cpu',
  },
  maxNumSentences: 1,
});
const initializedMs = performance.now() - started;

function validateAndWrite(label, audio, elapsedMs) {
  const audioSeconds = audio.samples.length / audio.sampleRate;
  let peak = 0;
  let energy = 0;
  for (const sample of audio.samples) {
    peak = Math.max(peak, Math.abs(sample));
    energy += sample * sample;
  }
  const rms = Math.sqrt(energy / audio.samples.length);
  if (!audio.samples.length || audio.sampleRate !== 22050 ||
      !Number.isFinite(peak) || !Number.isFinite(rms) ||
      peak < 0.05 || rms < 0.005) {
    throw new Error(`${label} produced invalid audio: ${JSON.stringify({
      samples: audio.samples.length,
      sampleRate: audio.sampleRate,
      peak,
      rms,
    })}`);
  }
  const output = `${outputPrefix}-${label}.wav`;
  edgevox.writeWave(output, audio);
  return {
    label,
    elapsedMs,
    audioSeconds,
    rtf: elapsedMs / 1000 / audioSeconds,
    sampleRate: audio.sampleRate,
    samples: audio.samples.length,
    peak,
    rms,
    output,
  };
}

function generate(label, generationConfig) {
  const generationStarted = performance.now();
  const audio = tts.generate({
    text: process.env.WFLOAT_TEXT ||
        'This is a clear English emotional voice test.',
    sid: Number(process.env.WFLOAT_SPEAKER_ID || 10),
    speed: 1,
    generationConfig,
    enableExternalBuffer: false,
  });
  return validateAndWrite(
      label, audio, performance.now() - generationStarted);
}

const results = [
  generate('plain', {
    extra: {
      emotion: process.env.WFLOAT_EMOTION || 'joy',
      emotion_intensity:
          Number(process.env.WFLOAT_EMOTION_INTENSITY || 0.7),
    },
  }),
];

if (referencePath) {
  if (!model.openvoiceToneEncoder || !model.openvoiceToneConverter) {
    throw new Error('Reference audio was supplied but OpenVoice models are missing.');
  }
  const reference = edgevox.readWave(referencePath, false);
  const cloneConfig = {
    referenceAudio: reference.samples,
    referenceSampleRate: reference.sampleRate,
    extra: {
      emotion: process.env.WFLOAT_EMOTION || 'joy',
      emotion_intensity:
          Number(process.env.WFLOAT_EMOTION_INTENSITY || 0.7),
    },
  };
  results.push(generate('clone-cold', cloneConfig));
  results.push(generate('clone-cached', cloneConfig));
}

console.log(JSON.stringify({
  initializedMs,
  openvoiceConfigured: Boolean(model.openvoiceToneEncoder),
  results,
}, null, 2));
