#!/usr/bin/env bash

set -ex

log() {
  # This function is from espnet
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $*"
}

echo "SLID_EXE is $SLID_EXE"
echo "SID_EXE is $SID_EXE"
echo "AT_EXE is $AT_EXE"
echo "PUNCT_EXE is $PUNCT_EXE"
echo "PATH: $PATH"

log "------------------------------------------------------------"
log "Test adding punctuations                                    "
log "------------------------------------------------------------"

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12-int8.tar.bz2
ls -lh
tar xf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12-int8.tar.bz2
ls -lh edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12-int8
rm edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12-int8.tar.bz2
$PUNCT_EXE
rm -rf edgevox-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12-int8

log "------------------------------------------------------------"
log "Test audio tagging                                          "
log "------------------------------------------------------------"

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/audio-tagging-models/edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
tar xvf edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2
rm edgevox-onnx-zipformer-audio-tagging-2024-04-09.tar.bz2

$AT_EXE

rm -rf edgevox-onnx-zipformer-audio-tagging-2024-04-09


log "------------------------------------------------------------"
log "Download whisper tiny for spoken language identification    "
log "------------------------------------------------------------"

rm -rf edgevox-onnx-whisper-tiny*
curl -LS -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-whisper-tiny.tar.bz2
tar xvf edgevox-onnx-whisper-tiny.tar.bz2
rm edgevox-onnx-whisper-tiny.tar.bz2

$SLID_EXE

rm -rf edgevox-onnx-whisper-tiny*

log "------------------------------------------------------------"
log "Download file for speaker identification and verification   "
log "------------------------------------------------------------"

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-recongition-models/3dspeaker_speech_campplus_sv_zh-cn_16k-common.onnx
git clone https://github.com/csukuangfj/sr-data

$SID_EXE

rm -fv *.onnx
rm -rf sr-data
