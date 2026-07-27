#!/usr/bin/env bash

# Please refer to
# https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-transducer/zipformer-transducer-models.html#csukuangfj-edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20-bilingual-chinese-english
# to download the model files

set -ex
if [ ! -d ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20 ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  tar xvf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
  rm edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
fi

if [ ! -f ./itn-zh-number.wav ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn-zh-number.wav
fi

if [ ! -f ./itn_zh_number.fst ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/itn_zh_number.fst
fi

dotnet run -c Release \
  --tokens ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt \
  --encoder ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/encoder-epoch-99-avg-1.int8.onnx \
  --decoder ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/decoder-epoch-99-avg-1.onnx \
  --joiner ./edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/joiner-epoch-99-avg-1.int8.onnx \
  --rule-fsts ./itn_zh_number.fst \
  --decoding-method greedy_search \
  --files ./itn-zh-number.wav
