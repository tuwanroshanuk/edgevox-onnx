// Copyright 2024 Xiaomi Corporation

// This file shows how to use an offline NeMo Canary model, i.e.,
// non-streaming NeMo Canary model, to decode files.
import com.k2fsa.sherpa.onnx.*;

public class NonStreamingDecodeFileNemoCanary {
  public static void main(String[] args) {
    // please refer to
    // https://k2-fsa.github.io/sherpa/onnx/nemo/canary.html
    // to download model files
    String encoder = "./edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8/encoder.int8.onnx";
    String decoder = "./edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8/decoder.int8.onnx";
    String tokens = "./edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8/tokens.txt";

    String waveFilename = "./edgevox-onnx-nemo-canary-180m-flash-en-es-de-fr-int8/test_wavs/en.wav";

    WaveReader reader = new WaveReader(waveFilename);

    OfflineCanaryModelConfig canary =
        OfflineCanaryModelConfig.builder()
            .setEncoder(encoder)
            .setDecoder(decoder)
            .setSrcLang("en")
            .setTgtLang("en")
            .setUsePnc(true)
            .build();

    OfflineModelConfig modelConfig =
        OfflineModelConfig.builder()
            .setCanary(canary)
            .setTokens(tokens)
            .setNumThreads(1)
            .setDebug(true)
            .build();

    OfflineRecognizerConfig config =
        OfflineRecognizerConfig.builder()
            .setOfflineModelConfig(modelConfig)
            .setDecodingMethod("greedy_search")
            .build();

    OfflineRecognizer recognizer = new OfflineRecognizer(config);
    OfflineStream stream = recognizer.createStream();
    stream.acceptWaveform(reader.getSamples(), reader.getSampleRate());

    recognizer.decode(stream);

    String text = recognizer.getResult(stream).getText();

    System.out.printf("filename:%s\nresult(English):%s\n", waveFilename, text);

    stream.release();
    recognizer.release();
  }
}
