// Copyright 2024 Xiaomi Corporation

// This file shows how to use a CED audio tagging model to tag
// input audio files.
import com.k2fsa.sherpa.onnx.*;

public class AudioTaggingCEDFromFile {
  public static void main(String[] args) {
    // please download the model from
    // https://github.com/k2-fsa/edgevox-onnx/releases/tag/audio-tagging-models
    String model = "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/model.int8.onnx";
    String labels = "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/class_labels_indices.csv";
    int topK = 5;

    String[] testWaves =
        new String[] {
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/1.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/2.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/3.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/4.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/5.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/6.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/7.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/8.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/9.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/10.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/11.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/12.wav",
          "./edgevox-onnx-ced-mini-audio-tagging-2024-04-19/test_wavs/13.wav",
        };

    AudioTaggingModelConfig modelConfig =
        AudioTaggingModelConfig.builder().setCED(model).setNumThreads(1).setDebug(true).build();

    AudioTaggingConfig config =
        AudioTaggingConfig.builder().setModel(modelConfig).setLabels(labels).setTopK(topK).build();

    AudioTagging tagger = new AudioTagging(config);
    System.out.println("------");
    for (String filename : testWaves) {
      WaveReader reader = new WaveReader(filename);

      OfflineStream stream = tagger.createStream();
      stream.acceptWaveform(reader.getSamples(), reader.getSampleRate());

      AudioEvent[] events = tagger.compute(stream);

      stream.release();

      System.out.printf("input file: %s\n", filename);
      System.out.printf("Probability\t\tName\n");
      for (AudioEvent e : events) {
        System.out.printf("%.3f\t\t\t%s\n", e.getProb(), e.getName());
      }
      System.out.println("------");
    }

    tagger.release();
  }
}
