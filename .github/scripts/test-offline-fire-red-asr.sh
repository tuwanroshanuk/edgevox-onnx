#!/usr/bin/env bash

set -e

log() {
  # This function is from espnet
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $*"
}

export GIT_CLONE_PROTECTION_ACTIVE=false

echo "EXE is $EXE"
echo "PATH: $PATH"

which $EXE

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
tar xvf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2
rm edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25.tar.bz2

for w in 0.wav 1.wav 2.wav 3-sichuan.wav 3.wav 4-tianjin.wav 5-henan.wav 8k.wav; do
$EXE \
  --fire-red-asr-ctc=./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/model.int8.onnx \
  --tokens=./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/tokens.txt \
  ./edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25/test_wavs/$w
done

rm -rf edgevox-onnx-fire-red-asr2-ctc-zh_en-int8-2026-02-25
