// Copyright (c)  2026  Xiaomi Corporation
//
const edgevox_onnx = require('edgevox-onnx');

function createOnlinePunctuation() {
  const config = {
    model: {
      cnnBilstm: './edgevox-onnx-online-punct-en-2024-08-06/model.onnx',
      bpeVocab: './edgevox-onnx-online-punct-en-2024-08-06/bpe.vocab',
      debug: 1,
      numThreads: 1,
      provider: 'cpu',
    },
  };

  return edgevox_onnx.createOnlinePunctuation(config);
}

const punct = createOnlinePunctuation();
const sentences = [
  'How are you i am fine thank you',
  'The African blogosphere is rapidly expanding bringing more voices online in the form of commentaries opinions analyses rants and poetry',
];

console.log('---');
for (const sentence of sentences) {
  const punctText = punct.addPunct(sentence);
  console.log(`Input: ${sentence}`);
  console.log(`Output: ${punctText}`);
  console.log('---');
}

punct.free();
