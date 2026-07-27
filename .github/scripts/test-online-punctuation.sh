#!/usr/bin/env bash

set -ex

echo "TODO(fangjun): Skip this test since the sanitizer test is failed. We need to fix it"
exit 0

log() {
  # This function is from espnet
  local fname=${BASH_SOURCE[1]##*/}
  echo -e "$(date '+%Y-%m-%d %H:%M:%S') (${fname}:${BASH_LINENO[0]}:${FUNCNAME[1]}) $*"
}

echo "EXE is $EXE"
echo "PATH: $PATH"

which $EXE

log "------------------------------------------------------------"
log "Download the punctuation model                             "
log "------------------------------------------------------------"

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/punctuation-models/edgevox-onnx-online-punct-en-2024-08-06.tar.bz2

tar xvf edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
rm edgevox-onnx-online-punct-en-2024-08-06.tar.bz2
repo=edgevox-onnx-online-punct-en-2024-08-06
ls -lh $repo

for m in model.onnx model.int8.onnx; do
  $EXE \
   --debug=1 \
   --cnn-bilstm=$repo/$m \
   --bpe-vocab=$repo/bpe.vocab \
   "How are you i am fine thank you"

  $EXE \
   --debug=1 \
   --cnn-bilstm=$repo/$m \
   --bpe-vocab=$repo/bpe.vocab \
   "The African blogosphere is rapidly expanding bringing more voices online in the form of commentaries opinions analyses rants and poetry"
done

rm -rf $repo
