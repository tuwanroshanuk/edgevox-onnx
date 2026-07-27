#!/usr/bin/env bash

# Please refer to
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-ctc/zipformer-ctc-models.html#edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13-chinese
# to download the model files

set -ex
if [ ! -d ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
  rm edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13.tar.bz2
fi

dotnet run -c Release \
  --tokens ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/tokens.txt \
  --zipformer2-ctc ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/ctc-epoch-20-avg-1-chunk-16-left-128.onnx \
  --files ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/DEV_T0000000000.wav \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/DEV_T0000000001.wav \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/DEV_T0000000002.wav \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/TEST_MEETING_T0000000113.wav \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/TEST_MEETING_T0000000219.wav \
  ./edgevox-onnx-streaming-zipformer-ctc-multi-zh-hans-2023-12-13/test_wavs/TEST_MEETING_T0000000351.wav
