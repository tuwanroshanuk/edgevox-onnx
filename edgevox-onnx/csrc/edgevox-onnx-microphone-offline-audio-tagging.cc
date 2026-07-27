// edgevox-onnx/csrc/edgevox-onnx-microphone-offline-audio-tagging.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <cctype>  // std::tolower
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include "portaudio.h"  // NOLINT
#include "edgevox-onnx/csrc/audio-tagging.h"
#include "edgevox-onnx/csrc/macros.h"
#include "edgevox-onnx/csrc/microphone.h"

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

static int32_t RecordCallback(const void *input_buffer,
                              void * /*output_buffer*/,
                              unsigned long frames_per_buffer,  // NOLINT
                              const PaStreamCallbackTimeInfo * /*time_info*/,
                              PaStreamCallbackFlags /*status_flags*/,
                              void * /*user_data*/) {
  std::lock_guard<std::mutex> lock(samples_mutex);

  auto p = reinterpret_cast<const float *>(input_buffer);
  samples.insert(samples.end(), p, p + frames_per_buffer);

  return stop ? paComplete : paContinue;
}

static void Handler(int32_t /*sig*/) {
  stop = true;
  fprintf(stderr, "\nCaught Ctrl + C. Press Enter to exit\n");
}

int32_t main(int32_t argc, char *argv[]) {
  signal(SIGINT, Handler);

  const char *kUsageMessage = R"usage(
Audio tagging from microphone.
Usage:

wget https://github.com/k2-fsa/sherpa-onnx/releases/download/audio-tagging-models/sherpa-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
tar xvf sherpa-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
rm sherpa-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2

./bin/edgevox-onnx-microphone-offline-audio-tagging \
  --zipformer-model=./sherpa-onnx-zipformer-audio-tagging-2024-04-09/model.onnx \
  --labels=./sherpa-onnx-zipformer-audio-tagging-2024-04-09/class_labels_indices.csv

Please see
https://github.com/k2-fsa/sherpa-onnx/releases/tag/audio-tagging-models
for more models.
)usage";

  edgevox_onnx::ParseOptions po(kUsageMessage);
  edgevox_onnx::AudioTaggingConfig config;
  config.Register(&po);

  po.Read(argc, argv);
  if (po.NumArgs() != 0) {
    fprintf(stderr, "\nThis program does not support positional arguments\n\n");
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

  edgevox_onnx::Microphone mic;

  int32_t device_index = Pa_GetDefaultInputDevice();
  if (device_index == paNoDevice) {
    fprintf(stderr, "No default input device found\n");
    fprintf(stderr, "If you are using Linux, please switch to \n");
    fprintf(stderr, " ./bin/edgevox-onnx-alsa-offline-audio-tagging \n");
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

  const char *pDeviceIndex = std::getenv("EDGEVOX_ONNX_MIC_DEVICE");
  if (pDeviceIndex) {
    fprintf(stderr, "Use specified device: %s\n", pDeviceIndex);
    device_index = atoi(pDeviceIndex);
  }

  mic.PrintDevices(device_index);
  float mic_sample_rate = 16000;
  const char *pSampleRateStr = std::getenv("EDGEVOX_ONNX_MIC_SAMPLE_RATE");
  if (pSampleRateStr) {
    mic_sample_rate = atof(pSampleRateStr);
    fprintf(stderr, "Use sample rate %f for mic\n", mic_sample_rate);
  }

  if (!mic.OpenDevice(device_index, mic_sample_rate, 1, RecordCallback,
                      nullptr /* user_data */)) {
    fprintf(stderr, "portaudio error: %d\n", device_index);
    EDGEVOX_ONNX_EXIT(EXIT_FAILURE);
  }

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
        s->AcceptWaveform(mic_sample_rate, buf.data(), buf.size());
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

    Pa_Sleep(20);  // sleep for 20ms
  }
  t.join();

  return 0;
}
