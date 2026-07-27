# Introduction

Please refer to
https://github.com/k2-fsa/edgevox-onnx/releases/tag/tts-models
to download a model.

The following is an example:
```bash
cd edgevox-onnx/wasm/tts/assets

wget -q https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/vits-piper-en_US-libritts_r-medium.tar.bz2
tar xf vits-piper-en_US-libritts_r-medium.tar.bz2
rm vits-piper-en_US-libritts_r-medium.tar.bz2
mv vits-piper-en_US-libritts_r-medium/en_US-libritts_r-medium.onnx ./model.onnx
mv vits-piper-en_US-libritts_r-medium/tokens.txt ./
mv vits-piper-en_US-libritts_r-medium/espeak-ng-data ./
rm -rf vits-piper-en_US-libritts_r-medium
```

ZipVoice example:

```bash
cd edgevox-onnx/wasm/tts/assets

wget -q https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
tar xf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2
rm edgevox-onnx-zipvoice-distill-int8-zh-en-emilia.tar.bz2

mv edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/encoder.int8.onnx ./
mv edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/decoder.int8.onnx ./
mv edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/tokens.txt ./
mv edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/lexicon.txt ./
mv edgevox-onnx-zipvoice-distill-int8-zh-en-emilia/espeak-ng-data ./
rm -rf edgevox-onnx-zipvoice-distill-int8-zh-en-emilia

wget -q https://github.com/k2-fsa/edgevox-onnx/releases/download/vocoder-models/vocos_24khz.onnx
```

PocketTTS example:

```bash
cd edgevox-onnx/wasm/tts/assets

wget -q https://github.com/k2-fsa/edgevox-onnx/releases/download/tts-models/edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
tar xf edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2
rm edgevox-onnx-pocket-tts-int8-2026-01-26.tar.bz2

mv edgevox-onnx-pocket-tts-int8-2026-01-26/lm_flow.int8.onnx ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/lm_main.int8.onnx ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/encoder.onnx ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/decoder.int8.onnx ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/text_conditioner.onnx ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/vocab.json ./
mv edgevox-onnx-pocket-tts-int8-2026-01-26/token_scores.json ./
rm -rf edgevox-onnx-pocket-tts-int8-2026-01-26
```

You should have the following files in `assets` before you can run
`build-wasm-simd-tts.sh`

```
assets fangjun$ tree -L 1
.
├── README.md
├── espeak-ng-data
├── mode.onnx
└── tokens.txt

1 directory, 3 files
```

You can find example build scripts at:

  - English TTS: https://github.com/k2-fsa/edgevox-onnx/blob/master/.github/workflows/wasm-simd-hf-space-en-tts.yaml
  - German TTS: https://github.com/k2-fsa/edgevox-onnx/blob/master/.github/workflows/wasm-simd-hf-space-de-tts.yaml
