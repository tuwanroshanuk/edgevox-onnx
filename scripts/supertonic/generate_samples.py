#!/usr/bin/env python3
# Copyright    2025  Xiaomi Corp.        (authors: Fangjun Kuang)
"""
Generate samples for
https://k2-fsa.github.io/sherpa/onnx/tts/all/
"""

import os
from pathlib import Path

import edgevox_onnx
import soundfile as sf

from gen_calib_configs import SENTENCES

config = edgevox_onnx.OfflineTtsConfig(
    model=edgevox_onnx.OfflineTtsModelConfig(
        supertonic=edgevox_onnx.OfflineTtsSupertonicModelConfig(
            duration_predictor="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/duration_predictor.int8.onnx",
            text_encoder="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/text_encoder.int8.onnx",
            vector_estimator="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/vector_estimator.int8.onnx",
            vocoder="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/vocoder.int8.onnx",
            tts_json="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/tts.json",
            unicode_indexer="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/unicode_indexer.bin",
            voice_style="./edgevox-onnx-supertonic-3-tts-int8-2026-05-11/voice.bin",
        ),
        debug=False,
        num_threads=2,
        provider="cpu",
    )
)

if not config.validate():
    raise ValueError("Please check your config")

tts = edgevox_onnx.OfflineTts(config)

for lang in SENTENCES:
    print("lang", lang)

    os.system(f"mkdir -p ./hf/supertonic/v3/mp3/{lang}")

    for sid in range(tts.num_speakers):
        print("sid", sid)

        sentence_list = SENTENCES[lang]
        for i, text in enumerate(sentence_list):

            filename = f"./hf/supertonic/v3/mp3/{lang}/sid-{sid}-{lang}-{i}.mp3"
            if Path(filename).is_file():
                continue

            print(i, text)

            gen_config = edgevox_onnx.GenerationConfig()

            # This model has 10 speakers. Valid sid: 0-9
            gen_config.sid = sid
            gen_config.num_steps = 8
            gen_config.speed = 1  # larger -> faster
            gen_config.extra["lang"] = lang

            audio = tts.generate(text, gen_config)

            sf.write(
                filename,
                audio.samples,
                samplerate=audio.sample_rate,
            )

        os.system(f"ls -lh hf/supertonic/v3/mp3/{lang}")
