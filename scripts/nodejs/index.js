// Copyright (c)  2023-2024  Xiaomi Corporation (authors: Fangjun Kuang)
'use strict'

// Emscripten >= 3.1.50 made MODULARIZE always return a Promise, even with
// WASM_ASYNC_COMPILATION=0. The runtime still attaches the exports onto the
// user-supplied moduleArg synchronously, so we pass our own object and ignore
// the (already-resolved) Promise — keeping this whole module synchronous.
let wasmModule = null;
function getWasmModule() {
  if (!wasmModule) {
    wasmModule = {};
    require('./edgevox-onnx-wasm-nodejs.js')(wasmModule);
  }
  return wasmModule;
}
const edgevox_onnx_asr = require('./edgevox-onnx-asr.js');
const edgevox_onnx_tts = require('./edgevox-onnx-tts.node.js');
const edgevox_onnx_kws = require('./edgevox-onnx-kws.js');
const edgevox_onnx_wave = require('./edgevox-onnx-wave.js');
const edgevox_onnx_vad = require('./edgevox-onnx-vad.js');
const edgevox_onnx_punctuation = require('./edgevox-onnx-punctuation.js');
const edgevox_onnx_speaker_diarization =
    require('./edgevox-onnx-speaker-diarization.js');
const edgevox_onnx_speech_enhancement =
    require('./edgevox-onnx-speech-enhancement.js');

let nativeAddon = null;
let nativeFallbackReason = '';
let activeRuntimeConfig = {provider: 'cpu', effectiveThreads: 1};

if (process.platform === 'win32' && process.arch === 'x64' &&
    process.env.EDGEVOX_ONNX_FORCE_WASM !== '1') {
  try {
    nativeAddon = require('./native/win32-x64/edgevox-onnx.node');
  } catch (error) {
    nativeFallbackReason =
        error && error.message ? String(error.message) : String(error);
  }
} else if (process.env.EDGEVOX_ONNX_FORCE_WASM === '1') {
  nativeFallbackReason = 'EDGEVOX_ONNX_FORCE_WASM=1';
} else {
  nativeFallbackReason = `No native artifact for ${process.platform}-${process.arch}`;
}

function nativeModelConfig(config) {
  const legacy = config.offlineTtsModelConfig || {};
  const zip = legacy.offlineTtsZipVoiceModelConfig || {};
  return {
    model: {
      vits: legacy.offlineTtsVitsModelConfig || {},
      matcha: legacy.offlineTtsMatchaModelConfig || {},
      kokoro: legacy.offlineTtsKokoroModelConfig || {},
      kitten: legacy.offlineTtsKittenModelConfig || {},
      zipvoice: {
        ...zip,
        targetRms: zip.targetRms ?? zip.targetRMS ?? 0.1,
      },
      pocket: legacy.offlineTtsPocketModelConfig || {},
      supertonic: legacy.offlineTtsSupertonicModelConfig || {},
      chatterbox: legacy.offlineTtsChatterboxModelConfig || {},
      numThreads: legacy.numThreads ?? 1,
      debug: legacy.debug ?? 0,
      // The packaged Windows ONNX Runtime currently ships CPUExecutionProvider
      // only. Normalize unsupported desktop preferences before core creation so
      // diagnostics report the effective provider and ORT does not warn/fallback.
      provider: 'cpu',
    },
    ruleFsts: config.ruleFsts || '',
    ruleFars: config.ruleFars || '',
    maxNumSentences: config.maxNumSentences ?? 1,
    silenceScale: config.silenceScale ?? 0.2,
  };
}

class NativeOfflineTts {
  constructor(config) {
    this.handle = nativeAddon.createOfflineTts(nativeModelConfig(config));
    this.sampleRate = nativeAddon.getOfflineTtsSampleRate(this.handle);
    this.numSpeakers = nativeAddon.getOfflineTtsNumSpeakers(this.handle);
  }

  free() {
    // The Node-API External owns the native finalizer. Dropping this reference
    // makes it collectible without risking a second native destruction.
    this.handle = null;
  }

  generate(config) {
    if (!this.handle) throw new Error('OfflineTts has been freed');
    return nativeAddon.offlineTtsGenerate(this.handle, {
      text: config.text,
      sid: config.sid ?? 0,
      speed: config.speed ?? 1.0,
      // Electron utility/worker processes reject Node-API external buffers.
      // Copy into a regular V8-owned ArrayBuffer for desktop compatibility.
      enableExternalBuffer: false,
    });
  }

  generateWithConfig(text, generationConfig) {
    if (!this.handle) throw new Error('OfflineTts has been freed');
    return nativeAddon.offlineTtsGenerateWithConfig(this.handle, {
      text,
      generationConfig,
      enableExternalBuffer: false,
    });
  }

  save(filename, audio) {
    return nativeAddon.writeWave(filename, audio);
  }
}



function createOnlineRecognizer(config) {
  return edgevox_onnx_asr.createOnlineRecognizer(getWasmModule(), config);
}

function createOfflineRecognizer(config) {
  return new edgevox_onnx_asr.OfflineRecognizer(config, getWasmModule());
}

function createOfflineTts(config) {
  const model = config && config.offlineTtsModelConfig || {};
  activeRuntimeConfig = {
    provider: nativeAddon ? 'cpu' : (model.provider || 'cpu'),
    effectiveThreads: model.numThreads ?? 1,
  };
  if (nativeAddon) {
    return new NativeOfflineTts(config);
  }
  return edgevox_onnx_tts.createOfflineTts(getWasmModule(), config);
}

function createKws(config) {
  return edgevox_onnx_kws.createKws(getWasmModule(), config);
}

function createCircularBuffer(capacity) {
  return new edgevox_onnx_vad.CircularBuffer(capacity, getWasmModule());
}

function createVad(config) {
  return edgevox_onnx_vad.createVad(getWasmModule(), config);
}

function createOfflinePunctuation(config) {
  return new edgevox_onnx_punctuation.OfflinePunctuation(
      config, getWasmModule());
}

function createOnlinePunctuation(config) {
  return new edgevox_onnx_punctuation.OnlinePunctuation(
      config, getWasmModule());
}

function createOfflineSpeakerDiarization(config) {
  return edgevox_onnx_speaker_diarization.createOfflineSpeakerDiarization(
      getWasmModule(), config);
}

function readWave(filename) {
  if (nativeAddon) return nativeAddon.readWave(filename, false);
  return edgevox_onnx_wave.readWave(filename, getWasmModule());
}

function writeWave(filename, data) {
  if (nativeAddon) return nativeAddon.writeWave(filename, data);
  edgevox_onnx_wave.writeWave(filename, data, getWasmModule());
}

function readWaveFromBinaryData(uint8Array) {
  if (nativeAddon) return nativeAddon.readWaveFromBinary(uint8Array, false);
  return edgevox_onnx_wave.readWaveFromBinaryData(
      uint8Array, getWasmModule());
}

function getRuntimeInfo() {
  return {
    backend: nativeAddon ? 'native' : 'wasm',
    platform: process.platform,
    arch: process.arch,
    nativeLoaded: !!nativeAddon,
    fallbackOccurred: !nativeAddon,
    fallbackReason: nativeAddon ? '' : nativeFallbackReason,
    provider: activeRuntimeConfig.provider,
    effectiveThreads: activeRuntimeConfig.effectiveThreads,
  };
}

function createOfflineSpeechDenoiser(config) {
  return edgevox_onnx_speech_enhancement.createOfflineSpeechDenoiser(
      getWasmModule(), config);
}

function createOnlineSpeechDenoiser(config) {
  return edgevox_onnx_speech_enhancement.createOnlineSpeechDenoiser(
      getWasmModule(), config);
}

function getVersion() {
  if (nativeAddon && nativeAddon.version != null) {
    return typeof nativeAddon.version === 'function' ?
        nativeAddon.version() : nativeAddon.version;
  }
  const wasmModule = getWasmModule();
  const v = wasmModule._EdgevoxOnnxGetVersionStr();
  return wasmModule.UTF8ToString(v);
}

function getGitSha1() {
  if (nativeAddon && nativeAddon.gitSha1 != null) {
    return typeof nativeAddon.gitSha1 === 'function' ?
        nativeAddon.gitSha1() : nativeAddon.gitSha1;
  }
  const wasmModule = getWasmModule();
  const v = wasmModule._EdgevoxOnnxGetGitSha1();
  return wasmModule.UTF8ToString(v);
}

function getGitDate() {
  if (nativeAddon && nativeAddon.gitDate != null) {
    return typeof nativeAddon.gitDate === 'function' ?
        nativeAddon.gitDate() : nativeAddon.gitDate;
  }
  const wasmModule = getWasmModule();
  const v = wasmModule._EdgevoxOnnxGetGitDate();
  return wasmModule.UTF8ToString(v);
}

// Note: online means streaming and offline means non-streaming here.
// Both of them don't require internet connection.
module.exports = {
  createOnlineRecognizer,
  createOfflineRecognizer,
  createOfflineTts,
  createKws,
  readWave,
  readWaveFromBinaryData,
  writeWave,
  createCircularBuffer,
  createVad,
  createOfflinePunctuation,
  createOnlinePunctuation,
  createOfflineSpeakerDiarization,
  createOfflineSpeechDenoiser,
  createOnlineSpeechDenoiser,
  getRuntimeInfo,
  version: getVersion(),
  gitSha1: getGitSha1(),
  gitDate: getGitDate(),
};
