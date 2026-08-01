# @nexuscloud/edgevox-onnx

Fork build of [sherpa-onnx](https://github.com/k2-fsa/sherpa-onnx) with ZipVoice **`espeakVoice`** support — configurable eSpeak language for out-of-lexicon words (e.g. `"si"` for Sinhala).

Built from [tuwanroshanuk/edgevox-onnx](https://github.com/tuwanroshanuk/edgevox-onnx) via GitHub Actions (`.github/workflows/build.yaml`).

## Contents

| Path | Purpose |
|------|---------|
| `native/win32-x64/` | Multithreaded Windows x64 Node addon and runtime DLLs |
| `edgevox-onnx-wasm-nodejs.js` + `.wasm` | Portable Node fallback |
| `edgevox-onnx-*.js` | JS bindings (`espeakVoice` in ZipVoice config) |
| `android/jniLibs/{abi}/*.so` | Capacitor Android native engine |
| `android/kotlin-api/Tts.kt` | Kotlin OfflineTts API |
| `scripts/sync-android.mjs` | Sync Android bits into your Capacitor app |

## Install

```bash
npm install @nexuscloud/edgevox-onnx
```

## Sync Android (Capacitor)

From your app root:

```bash
node node_modules/@nexuscloud/edgevox-onnx/scripts/sync-android.mjs --force
```

## Windows runtime selection

Windows x64 uses the native multithreaded addon by default and automatically
falls back to WASM if the addon or one of its DLLs cannot load. To troubleshoot
with the portable backend:

```powershell
$env:EDGEVOX_ONNX_FORCE_WASM = '1'
```

Inspect the active runtime after creating a TTS instance:

```javascript
console.log(edgevox.getRuntimeInfo());
// { backend, provider, effectiveThreads, platform, fallbackOccurred, ... }
```

## ZipVoice espeakVoice (desktop)

```javascript
const edgevox = require('@nexuscloud/edgevox-onnx');

// Native Windows builds can construct TTS sessions from logical package
// resources without creating plaintext model files. `resources` maps the same
// relative names used in the model config to Node Buffers.
const memoryTts = edgevox.createOfflineTtsFromMemory(config, resources);
const tts = edgevox.createOfflineTts({
  offlineTtsModelConfig: {
    offlineTtsZipVoiceModelConfig: {
      // ...
      espeakVoice: 'si',  // Sinhala OOV phonemization
    },
    numThreads: 2,
    provider: 'cpu',
  },
});
```

ZipVoice generation accepts `extra.seed` for repeatable comparisons. Omitting
it retains random sampling. Preview generation is recommended at 4 flow steps;
use 8 steps for high-quality final output.

## ZipVoice benchmark

```bash
npm run benchmark:zipvoice -- \
  --model-dir=/path/to/voice \
  --reference-wav=/path/to/reference.wav \
  --reference-text="Reference transcript" \
  --threads=2 --steps=4 --seed=1234
```

The command reports the backend, provider, thread count, precision,
initialization time, cold/warm generation time, audio duration, RTF, and
process peak RSS.

## Chatterbox Turbo

Chatterbox Turbo uses the official four-graph ONNX export and GPT-2 tokenizer.
It provides local English zero-shot voice cloning and expressive tags such as
`[laugh]`, `[chuckle]`, and `[cough]`. For CPU use, Q4 is recommended for the
speech encoder, token embedding, and language model. Use the INT8
(`_quantized`) conditional decoder: the Q4 decoder can produce a corrupted,
extremely low-level waveform with ONNX Runtime on CPU.

```javascript
const reference = edgevox.readWave('/path/to/reference.wav');
const tts = edgevox.createOfflineTts({
  offlineTtsModelConfig: {
    offlineTtsChatterboxModelConfig: {
      speechEncoder: '/models/onnx/speech_encoder_q4.onnx',
      embedTokens: '/models/onnx/embed_tokens_q4.onnx',
      languageModel: '/models/onnx/language_model_q4.onnx',
      conditionalDecoder:
          '/models/onnx/conditional_decoder_quantized.onnx',
      tokenizer: '/models',
    },
    numThreads: 4,
    provider: 'cpu',
  },
});
const audio = tts.generateWithConfig(
    'That is wonderful! [chuckle] Thanks for calling.',
    {
      referenceAudio: reference.samples,
      referenceSampleRate: reference.sampleRate,
      extra: {max_new_tokens: 1024, repetition_penalty: 1.2},
    });
```

Benchmark it with:

```bash
npm run benchmark:chatterbox -- \
  --model-dir=/path/to/chatterbox_turbo \
  --reference-wav=/path/to/reference.wav \
  --output=chatterbox-test.wav
```

## Wfloat Emotional VITS

Wfloat provides fast **English-only** emotional TTS with 20 speakers, eight
emotions, and intensity control. It is not a multilingual model. The Edgevox
frontend preserves Wfloat's exact eSpeak token format instead of applying
generic Piper BOS/EOS and blank tokens.

```javascript
const edgevox = require('@nexuscloud/edgevox-onnx');
const modelDir = '/models/wfloat_emotional_vits';
const tts = edgevox.createOfflineTts({
  offlineTtsModelConfig: {
    offlineTtsVitsModelConfig: {
      model: `${modelDir}/wfloat_emotional_vits.onnx`,
      tokens: `${modelDir}/tokens.txt`,
      dataDir: `${modelDir}/espeak-ng-data`,
      noiseScale: 0.667,
      noiseScaleW: 0.8,
      lengthScale: 1,
      // Optional desktop voice conversion:
      openvoiceToneEncoder:
          `${modelDir}/openvoice_v2/tone_ref_encoder.onnx`,
      openvoiceToneConverter:
          `${modelDir}/openvoice_v2/tone_converter.onnx`,
    },
    numThreads: 2,
    provider: 'cpu',
  },
});
const audio = tts.generateWithConfig(
    'This is a clear English emotional voice.',
    {
      sid: 10,
      extra: {emotion: 'joy', emotion_intensity: 0.7},
    });
edgevox.writeWave('wfloat.wav', audio);
```

Supported emotion names are `neutral`, `joy`, `sadness`, `anger`, `fear`,
`surprise`, `dismissive`, and `confusion`. Intensity is clamped to 0–1.

OpenVoice V2 voice conversion is optional and currently supported on the
desktop filesystem path. Add `referenceAudio` and `referenceSampleRate` to the
generation config to convert Wfloat's output timbre. The reference embedding is
cached while the TTS instance remains alive. OpenVoice changes timbre; it does
not add languages to Wfloat.

Benchmark plain generation and optional cloning with:

```bash
npm run benchmark:wfloat -- \
  --model-dir=/path/to/wfloat_emotional_vits \
  --emotion=joy --intensity=0.7 --speaker=10 \
  --reference-wav=/path/to/reference.wav \
  --output=wfloat-test.wav
```

## Local validation (before publish)

```bash
# Keep CommonJS TTS binding in sync with browser source
python3 wasm/tts/sync-tts-node.py

# Fail on syntax errors (catches stray braces after hand-edits)
node --check wasm/tts/edgevox-onnx-tts.js
node --check wasm/tts/edgevox-onnx-tts.node.js
node --check scripts/nodejs/index.js
```

CI (`build.yaml`) runs the same checks before WASM/Android builds and again
on the assembled npm package so a bad JS file cannot be published.

## License

Apache-2.0 (same as upstream edgevox-onnx)
