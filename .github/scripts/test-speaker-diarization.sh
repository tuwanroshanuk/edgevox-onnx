#!/usr/bin/env bash

set -ex

log() {
  # This function is from espnet
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $*"
}

echo "EXE is $EXE"
echo "PATH: $PATH"

which $EXE

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-segmentation-models/edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
tar xvf edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
rm edgevox-onnx-pyannote-segmentation-3-0.tar.bz2

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-recongition-models/3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-segmentation-models/0-four-speakers-zh.wav

log "specify number of clusters"
$EXE \
  --clustering.num-clusters=4 \
  --segmentation.pyannote-model=./edgevox-onnx-pyannote-segmentation-3-0/model.onnx \
  --embedding.model=./3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx \
  ./0-four-speakers-zh.wav

log "specify threshold for clustering"

$EXE \
  --clustering.cluster-threshold=0.90 \
  --segmentation.pyannote-model=./edgevox-onnx-pyannote-segmentation-3-0/model.onnx \
  --embedding.model=./3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx \
  ./0-four-speakers-zh.wav

rm -rf edgevox-onnx-pyannote-*
rm -fv *.onnx
rm -fv *.wav
