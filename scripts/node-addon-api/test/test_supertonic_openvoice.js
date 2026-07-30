'use strict';

const fs = require('fs');
const path = require('path');
const edgevox = require('../lib/edgevox-onnx.js');

const modelDir = process.env.SUPERTONIC_MODEL_DIR;
const openvoiceDir = process.env.OPENVOICE_MODEL_DIR;
const referencePath = process.env.OPENVOICE_REFERENCE_WAV;
if (!modelDir || !openvoiceDir || !referencePath) {
  console.error(
      'Set SUPERTONIC_MODEL_DIR, OPENVOICE_MODEL_DIR, and ' +
      'OPENVOICE_REFERENCE_WAV to run this test.');
  process.exit(2);
}

for (const filename of ['tone_ref_encoder.onnx', 'tone_converter.onnx']) {
  if (!fs.existsSync(path.join(openvoiceDir, filename))) {
    throw new Error(`Missing OpenVoice model: ${path.join(openvoiceDir, filename)}`);
  }
}

const tts = new edgevox.OfflineTts({
  model: {
    vits: {
      openvoiceToneEncoder: path.join(openvoiceDir, 'tone_ref_encoder.onnx'),
      openvoiceToneConverter: path.join(openvoiceDir, 'tone_converter.onnx'),
    },
    supertonic: {
      durationPredictor: path.join(modelDir, 'duration_predictor.int8.onnx'),
      textEncoder: path.join(modelDir, 'text_encoder.int8.onnx'),
      vectorEstimator: path.join(modelDir, 'vector_estimator.int8.onnx'),
      vocoder: path.join(modelDir, 'vocoder.int8.onnx'),
      ttsJson: path.join(modelDir, 'tts.json'),
      unicodeIndexer: path.join(modelDir, 'unicode_indexer.bin'),
      voiceStyle: path.join(modelDir, 'voice.bin'),
    },
    numThreads: Number(process.env.SUPERTONIC_THREADS || 2),
    debug: process.env.EDGEVOX_DEBUG === '1',
    provider: 'cpu',
  },
  maxNumSentences: 1,
});

const reference = edgevox.readWave(referencePath, false);
const started = performance.now();
const audio = tts.generate({
  text: process.env.SUPERTONIC_TEXT ||
      'This is a clear English Supertonic voice cloning test.',
  generationConfig: {
    sid: Number(process.env.SUPERTONIC_SPEAKER_ID || 0),
    speed: 1,
    referenceAudio: reference.samples,
    referenceSampleRate: reference.sampleRate,
    extra: {lang: process.env.SUPERTONIC_LANGUAGE || 'en'},
  },
  enableExternalBuffer: false,
});
const elapsedMs = performance.now() - started;

let peak = 0;
let energy = 0;
for (const sample of audio.samples) {
  peak = Math.max(peak, Math.abs(sample));
  energy += sample * sample;
}
const rms = Math.sqrt(energy / Math.max(1, audio.samples.length));
if (!audio.samples.length || audio.sampleRate !== 22050 ||
    !Number.isFinite(peak) || !Number.isFinite(rms) ||
    peak < 0.01 || rms < 0.001) {
  throw new Error(`Supertonic OpenVoice produced invalid audio: ${JSON.stringify({
    samples: audio.samples.length,
    sampleRate: audio.sampleRate,
    peak,
    rms,
  })}`);
}

const output = process.env.SUPERTONIC_OUTPUT_WAV ||
    path.join(modelDir, 'smoke-test-openvoice-clone.wav');
edgevox.writeWave(output, audio);
console.log(JSON.stringify({
  elapsedMs,
  audioSeconds: audio.samples.length / audio.sampleRate,
  sampleRate: audio.sampleRate,
  samples: audio.samples.length,
  peak,
  rms,
  output,
}, null, 2));
