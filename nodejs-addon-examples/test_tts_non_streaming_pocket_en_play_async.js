// Copyright (c)  2026  Xiaomi Corporation
//
// npm install speaker
//
const Speaker = require('speaker');
const edgevox_onnx = require('edgevox-onnx-node');

async function createOfflineTts() {
  const config = {
    model: {
      pocket: {
        lmFlow: './edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx',
        lmMain: './edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx',
        encoder: './edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx',
        decoder: './edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx',
        textConditioner:
            './edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx',
        vocabJson: './edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json',
        tokenScoresJson:
            './edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json',
        voiceEmbeddingCacheCapacity: 50,
      },
      debug: false,  // set to true to see verbose logs
      numThreads: 2,
      provider: 'cpu',
    },
    maxNumSentences: 1,
  };

  return await edgevox_onnx.OfflineTts.createAsync(config);
}

function createSpeaker(sampleRate) {
  return new Speaker({
    channels: 1,
    bitDepth: 16,
    sampleRate: sampleRate,
    signed: true,
  });
}

function float32ToInt16Buffer(samples) {
  const buffer = Buffer.alloc(samples.length * 2);

  for (let i = 0; i < samples.length; ++i) {
    const s = Math.max(-1, Math.min(1, samples[i]));
    const v = s < 0 ? s * 0x8000 : s * 0x7fff;
    buffer.writeInt16LE(Math.round(v), i * 2);
  }

  return buffer;
}

function waitForEvent(emitter, eventName) {
  return new Promise((resolve, reject) => {
    emitter.once(eventName, resolve);
    emitter.once('error', reject);
  });
}

/**
 * @param {edgevox_onnx.OfflineTts} tts
 * @param {string} text
 */
async function generateAudioAsync(tts, text) {
  const referenceAudioFilename =
      './edgevox-onnx-pocket-tts-int8-2026-01-26/test_wavs/bria.wav';
  const referenceWave = edgevox_onnx.readWave(referenceAudioFilename);

  const generationConfig = new edgevox_onnx.GenerationConfig({
    speed: 1.0,
    referenceAudio: referenceWave.samples,
    referenceSampleRate: referenceWave.sampleRate,
    numSteps: 5,
    extra: {max_reference_audio_len: 12, seed: 42},
  });

  const speaker = createSpeaker(tts.sampleRate);
  const start = Date.now();

  console.log('Starting generation and playback...');

  const audio = await tts.generateAsync({
    text,
    enableExternalBuffer: true,
    generationConfig,
    onProgress: ({samples, progress}) => {
      process.stdout.write(
          `Progress: ${(progress * 100).toFixed(1)}%, ` +
          `Chunk samples: ${samples.length}\r`);
      speaker.write(float32ToInt16Buffer(samples));
      return 1;
    },
  });

  const generationStop = Date.now();
  speaker.end();
  await waitForEvent(speaker, 'close');
  const playbackStop = Date.now();

  console.log('\nGeneration and playback complete!');
  return {
    audio,
    generationElapsedSeconds: (generationStop - start) / 1000,
    playbackElapsedSeconds: (playbackStop - start) / 1000,
  };
}

async function main() {
  console.log('Creating OfflineTts...');
  const tts = await createOfflineTts();
  console.log('OfflineTts created!');

  const text =
      'Today as always, men fall into two groups: slaves and free men. Whoever does not have two-thirds of his day for himself, is a slave, whatever he may be: a statesman, a businessman, an official, or a scholar.';

  const {audio, generationElapsedSeconds, playbackElapsedSeconds} =
      await generateAudioAsync(tts, text);
  const duration = audio.samples.length / audio.sampleRate;
  const real_time_factor = generationElapsedSeconds / duration;

  console.log('Wave duration', duration.toFixed(3), 'seconds');
  console.log(
      'Generation elapsed', generationElapsedSeconds.toFixed(3), 'seconds');
  console.log(
      'Playback drained in', playbackElapsedSeconds.toFixed(3), 'seconds');
  console.log(
      `RTF = ${generationElapsedSeconds.toFixed(3)}/${duration.toFixed(3)} =`,
      real_time_factor.toFixed(3));

  const filename = 'test-pocket-bria-play-async.wav';
  edgevox_onnx.writeWave(filename, {
    samples: audio.samples,
    sampleRate: audio.sampleRate,
  });
  console.log(`Saved to ${filename}`);
}

main().catch((err) => {
  console.error('Error:', err);
  process.exitCode = 1;
});
