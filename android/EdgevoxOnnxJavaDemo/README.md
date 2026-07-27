# Introduction

Please run the following commands to download model files before you run this Android demo:

```bash
# Assume we are inside
# /Users/fangjun/open-source/edgevox-onnx/android/EdgevoxOnnxJavaDemo

cd app/src/main/assets/
wget https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2

tar xvf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2
rm edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20.tar.bz2

mv edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/encoder-epoch-99-avg-1.int8.onnx ./
mv edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/decoder-epoch-99-avg-1.onnx ./
mv edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/joiner-epoch-99-avg-1.int8.onnx ./
mv edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/tokens.txt ./

rm -rf edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/*

mv encoder-epoch-99-avg-1.int8.onnx edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/
mv decoder-epoch-99-avg-1.onnx edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/
mv joiner-epoch-99-avg-1.int8.onnx edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/
mv tokens.txt edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/
```

You should have the following directory structure:
```
(py38) fangjuns-MacBook-Pro:assets fangjun$ pwd
/Users/fangjun/open-source/edgevox-onnx/android/EdgevoxOnnxJavaDemo/app/src/main/assets

(py38) fangjuns-MacBook-Pro:assets fangjun$ tree .
.
└── edgevox-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20
    ├── decoder-epoch-99-avg-1.onnx
    ├── encoder-epoch-99-avg-1.int8.onnx
    ├── joiner-epoch-99-avg-1.int8.onnx
    └── tokens.txt

1 directory, 4 files
```

Remember to remove unused files to reduce the file size of the final APK.
