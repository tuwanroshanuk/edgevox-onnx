#!/usr/bin/env python3
# Copyright    2025  Xiaomi Corp.        (authors: Fangjun Kuang)
"""
Generate samples for
https://k2-fsa.github.io/sherpa/onnx/tts/all/
"""


import edgevox_onnx
import soundfile as sf

config = edgevox_onnx.OfflineTtsConfig(
    model=edgevox_onnx.OfflineTtsModelConfig(
        matcha=edgevox_onnx.OfflineTtsMatchaModelConfig(
            acoustic_model="matcha-icefall-en_US-ljspeech/model-steps-3.onnx",
            vocoder="vocos-22khz-univ.onnx",
            tokens="matcha-icefall-en_US-ljspeech/tokens.txt",
            lexicon="",
            data_dir="matcha-icefall-en_US-ljspeech/espeak-ng-data",
        ),
        num_threads=2,
    ),
    max_num_sentences=1,
)

if not config.validate():
    raise ValueError("Please check your config")

tts = edgevox_onnx.OfflineTts(config)
text = "Friends fell out often because life was changing so fast. The easiest thing in the world was to lose touch with someone."

audio = tts.generate(text, sid=0, speed=1.0)

sf.write(
    "./hf/matcha/icefall-en-ljspeech/mp3/0.mp3",
    audio.samples,
    samplerate=audio.sample_rate,
)
