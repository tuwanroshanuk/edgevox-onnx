#!/usr/bin/env python3
#
# Copyright (c)  2026  Xiaomi Corporation

"""
This file demonstrates how to use edgevox-onnx Python API
for Chinese/English zero-shot TTS with ZipVoice.

Different from ./zipvoice-tts.py, this file plays back the generated audio
while the model is still generating.

Usage:

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
tar xvf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
rm edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos_24khz.onnx

python3 ./python-api-examples/zipvoice-tts-play.py

You can find more models at
https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models

Please see
https://k2-fsa.github.io/sherpa/onnx/tts/zipvoice.html
for details.
"""

import logging
import queue
import sys
import threading
import time
from pathlib import Path

import librosa
import numpy as np
import edgevox_onnx
import soundfile as sf

try:
    import sounddevice as sd
except ImportError:
    print("Please install sounddevice first. You can use")
    print()
    print("  pip install sounddevice")
    print()
    print("to install it")
    sys.exit(-1)


def create_tts():
    tts_config = edgevox_onnx.OfflineTtsConfig(
        model=edgevox_onnx.OfflineTtsModelConfig(
            zipvoice=edgevox_onnx.OfflineTtsZipvoiceModelConfig(
                tokens="./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/tokens.txt",
                encoder="./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx",
                decoder="./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/decoder.int8.onnx",
                data_dir="./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/espeak-ng-data",
                lexicon="./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/lexicon.txt",
                vocoder="./vocos_24khz.onnx",
                espeak_voice="en-us",
            ),
            debug=False,
            num_threads=2,
            provider="cpu",
        )
    )
    if not tts_config.validate():
        raise ValueError(
            "Please read the previous error messages and re-check your config"
        )

    return edgevox_onnx.OfflineTts(tts_config)


buffer = queue.Queue()
started = False
stopped = False
killed = False
sample_rate = None
event = threading.Event()
first_message_time = None


def generated_audio_callback(samples: np.ndarray, progress: float):
    global first_message_time
    if first_message_time is None:
        first_message_time = time.time()

    buffer.put(samples)

    global started
    if started is False:
        logging.info("Start playing ...")
    started = True

    if killed:
        return 0

    return 1


def play_audio_callback(
    outdata: np.ndarray, frames: int, time, status: sd.CallbackFlags
):
    if killed or (started and buffer.empty() and stopped):
        event.set()

    if buffer.empty():
        outdata.fill(0)
        return

    n = 0
    while n < frames and not buffer.empty():
        remaining = frames - n
        k = buffer.queue[0].shape[0]

        if remaining <= k:
            outdata[n:, 0] = buffer.queue[0][:remaining]
            buffer.queue[0] = buffer.queue[0][remaining:]
            n = frames
            if buffer.queue[0].shape[0] == 0:
                buffer.get()

            break

        outdata[n : n + k, 0] = buffer.get()
        n += k

    if n < frames:
        outdata[n:, 0] = 0


def play_audio():
    if False:
        devices = sd.query_devices()
        print(devices)

        default_output_device_idx = sd.default.device[1]
        print(
            f'Use default output device: {devices[default_output_device_idx]["name"]}'
        )

    with sd.OutputStream(
        channels=1,
        callback=play_audio_callback,
        dtype="float32",
        samplerate=sample_rate,
        blocksize=1024,
    ):
        event.wait()

    logging.info("Exiting ...")


def main():
    reference_audio_file = (
        "./edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/test_wavs/leijun-1.wav"
    )
    if not Path(reference_audio_file).is_file():
        raise ValueError(f"Reference audio {reference_audio_file} does not exist")

    logging.info("Loading model ...")
    tts = create_tts()
    logging.info("Loading model done.")

    reference_audio, reference_sample_rate = librosa.load(reference_audio_file, sr=None)
    reference_text = "那还是三十六年前, 一九八七年. 我呢考上了武汉大学的计算机系."
    text = """
    小米的价值观是真诚, 热爱.
    真诚，就是不欺人也不自欺.
    热爱, 就是全心投入并享受其中.
    """

    global sample_rate
    sample_rate = tts.sample_rate

    gen_config = edgevox_onnx.GenerationConfig()
    gen_config.reference_audio = reference_audio
    gen_config.reference_sample_rate = reference_sample_rate
    gen_config.reference_text = reference_text
    gen_config.num_steps = 4
    gen_config.extra["min_char_in_sentence"] = "30"

    play_back_thread = threading.Thread(target=play_audio)
    play_back_thread.start()

    logging.info("Start generating ...")
    start_time = time.time()
    audio = tts.generate(
        text,
        gen_config,
        callback=generated_audio_callback,
    )
    end_time = time.time()
    logging.info("Finished generating!")

    global stopped
    stopped = True

    if len(audio.samples) == 0:
        print("Error in generating audios. Please read previous error messages.")
        global killed
        killed = True
        play_back_thread.join()
        return

    elapsed_seconds = end_time - start_time
    audio_duration = len(audio.samples) / audio.sample_rate
    real_time_factor = elapsed_seconds / audio_duration

    output_filename = "./generated-zipvoice-zh-en-play.wav"
    sf.write(
        output_filename,
        audio.samples,
        samplerate=audio.sample_rate,
        subtype="PCM_16",
    )
    logging.info(f"The text is '{text}'")
    logging.info(
        "Time in seconds to receive the first "
        f"message: {first_message_time-start_time:.3f}"
    )
    logging.info(f"Elapsed seconds: {elapsed_seconds:.3f}")
    logging.info(f"Audio duration in seconds: {audio_duration:.3f}")
    logging.info(
        f"RTF: {elapsed_seconds:.3f}/{audio_duration:.3f} = {real_time_factor:.3f}"
    )

    logging.info(f"***  Saved to {output_filename} ***")

    print("\n   >>>>>>>>> You can safely press ctrl + C to stop the play <<<<<<<<<<\n")

    play_back_thread.join()


if __name__ == "__main__":
    formatter = "%(asctime)s %(levelname)s [%(filename)s:%(lineno)d] %(message)s"

    logging.basicConfig(format=formatter, level=logging.INFO)
    try:
        main()
    except KeyboardInterrupt:
        print("\nCaught Ctrl + C. Exiting")
        killed = True
        sys.exit(0)
