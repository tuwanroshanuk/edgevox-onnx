# Introduction

Please refer to
https://github.com/k2-fsa/edgevox-onnx/releases/tag/speaker-segmentation-models
to download a speaker segmentation model
and
refer to
https://github.com/k2-fsa/edgevox-onnx/releases/tag/speaker-recongition-models
to download a speaker embedding extraction model.

Remember to rename the downloaded files.

The following is an example.

```bash
cd wasm/speaker-diarization/assets/

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-segmentation-models/edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
tar xvf edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
rm edgevox-onnx-pyannote-segmentation-3-0.tar.bz2
cp edgevox-onnx-pyannote-segmentation-3-0/model.onnx ./segmentation.onnx
rm -rf edgevox-onnx-pyannote-segmentation-3-0

curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/speaker-recongition-models/3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx
mv 3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx ./embedding.onnx
```
