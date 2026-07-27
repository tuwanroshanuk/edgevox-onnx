#!/usr/bin/env bash

set -ex

dart pub get

if [ ! -f ./edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k/tokens.txt ]; then
  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k.tar.bz2
  tar xvf edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k.tar.bz2
  rm edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k.tar.bz2
fi

dart run \
  ./bin/nemo-ctc.dart \
  --model ./edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k/model.onnx \
  --tokens ./edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k/tokens.txt \
  --input-wav ./edgevox-onnx-nemo-fast-conformer-ctc-be-de-en-es-fr-hr-it-pl-ru-uk-20k/test_wavs/de-german.wav
