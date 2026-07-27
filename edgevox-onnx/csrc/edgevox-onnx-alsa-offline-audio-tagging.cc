// edgevox-onnx/csrc/edgevox-onnx-alsa-offline-audio-tagging.cc
//
// Copyright (c)  2022-2024  Xiaomi Corporation

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <mutex>  // NOLINT
#include <string>
#include <thread>  // NOLINT
#include <utility>
#include <vector>

#include "edgevox-onnx/csrc/alsa.h"
#include "edgevox-onnx/csrc/audio-tagging.h"
#include "edgevox-onnx/csrc/macros.h"

enum class State {
  kIdle,
  kRecording,
  kDecoding,
};

State state = State::kIdle;

// true to stop the program and exit
bool stop = false;

std::vector<float> samples;
std::mutex samples_mutex;

static void DetectKeyPress() {
  EDGEVOX_ONNX_LOGE("Press Enter to start");
  int32_t key;
  while (!stop && (key = getchar())) {
    if (key != 0x0a) {
      continue;
    }

    switch (state) {
      case State::kIdle:
        EDGEVOX_ONNX_LOGE("Start recording. Press Enter to stop recording");
        state = State::kRecording;
        {
          std::lock_guard<std::mutex> lock(samples_mutex);
          samples.clear();
        }
        break;
      case State::kRecording:
        EDGEVOX_ONNX_LOGE("Stop recording. Decoding ...");
        state = State::kDecoding;
        break;
      case State::kDecoding:
        break;
    }
  }
}

static void Record(const char *device_name, int32_t expected_sample_rate) {
  edgevox_onnx::Alsa alsa(device_name);

  if (alsa.GetExpectedSampleRate() != expected_sample_rate) {
    fprintf(stderr, "sample rate: %d != %d\n", alsa.GetExpectedSampleRate(),
            expected_sample_rate);
    EDGEVOX_ONNX_EXIT(-1);
  }

  int32_t chunk = 0.1 * alsa.GetActualSampleRate();
  while (!stop) {
    const std::vector<float> &s = alsa.Read(chunk);
    std::lock_guard<std::mutex> lock(samples_mutex);
    samples.insert(samples.end(), s.begin(), s.end());
  }
}

static void Handler(int32_t sig) {
  stop = true;
  fprintf(stderr, "\nCaught Ctrl + C. Press Enter to exit\n");
}

int32_t main(int32_t argc, char *argv[]) {
  signal(SIGINT, Handler);

  const char *kUsageMessage = R"usage(
Audio tagging from microphone (Linux only).
Usage:

wget https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
tar xvf edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
rm edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2

./bin/edgevox-onnx-alsa-offline-audio-tagging \
  --zipformer-model=./edgevox-onnx-zipformer-audio-tagging-2024-04-09/model.onnx \
  --labels=./edgevox-onnx-zipformer-audio-tagging-2024-04-09/class_labels_indices.csv \
    device_name

Please refer to
https://github.com/k2-fsa/edgevox-onnx/releases/tag/audio-tagging-models
for a list of pre-trained models to download.

The device name specifies which microphone to use in case there are several
on your system. You can use

  arecord -l

to find all available microphones on your computer. For instance, if it outputs

**** List of CAPTURE Hardware Devices ****
card 3: UACDemoV10 [UACDemoV1.0], device 0: USB Audio [USB Audio]
  Subdevices: 1/1
  Subdevice #0: subdevice #0

and if you want to select card 3 and device 0 on that card, please use:

  plughw:3,0

as the device_name.
)usage";

  edgevox_onnx::ParseOptions po(kUsageMessage);
  edgevox_onnx::AudioTaggingConfig config;
  config.Register(&po);

  po.Read(argc, argv);
  if (po.NumArgs() != 1) {
    fprintf(stderr, "Please provide only 1 argument: the device name\n");
    po.PrintUsage();
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", config.ToString().c_str());

  if (!config.Validate()) {
    fprintf(stderr, "Errors in config!\n");
    return -1;
  }

  EDGEVOX_ONNX_LOGE("Creating audio tagger ...");
  edgevox_onnx::AudioTagging tagger(config);
  EDGEVOX_ONNX_LOGE("Audio tagger created created!");

  std::string device_name = po.GetArg(1);
  fprintf(stderr, "Use recording device: %s\n", device_name.c_str());

  int32_t sample_rate = 16000;  // fixed to 16000Hz for all models from icefall

  std::thread t2(Record, device_name.c_str(), sample_rate);
  using namespace std::chrono_literals;  // NOLINT
  std::this_thread::sleep_for(100ms);    // sleep for 100ms
  std::thread t(DetectKeyPress);

  while (!stop) {
    switch (state) {
      case State::kIdle:
        break;
      case State::kRecording:
        break;
      case State::kDecoding: {
        std::vector<float> buf;
        {
          std::lock_guard<std::mutex> lock(samples_mutex);
          buf = std::move(samples);
        }
        EDGEVOX_ONNX_LOGE("Computing...");
        auto s = tagger.CreateStream();
        s->AcceptWaveform(sample_rate, buf.data(), buf.size());
        auto results = tagger.Compute(s.get());
        EDGEVOX_ONNX_LOGE("Result is:");

        int32_t i = 0;
        std::ostringstream os;
        for (const auto &event : results) {
          os << i << ": " << event.ToString() << "\n";
          i += 1;
        }

        EDGEVOX_ONNX_LOGE("\n%s\n", os.str().c_str());

        state = State::kIdle;
        EDGEVOX_ONNX_LOGE("Press Enter to start");
        break;
      }
    }

    std::this_thread::sleep_for(20ms);  // sleep for 20ms
  }
  t.join();
  t2.join();

  return 0;
}
