// Copyright 2024 Xiaomi Corporation

// This file shows how to use an offline Moonshine,
// i.e., non-streaming Moonshine model,
// to decode files.
import com.k2fsa.sherpa.onnx.*;

public class NonStreamingDecodeFileMoonshine {
  public static void main(String[] args) {
    // please refer to
    // https://k2-fsa.github.io/sherpa/onnx/moonshine/index.html
    // to download model files

    String preprocessor = "./edgevox-onnx-moonshine-tiny-en-int8/preprocess.onnx";
    String encoder = "./edgevox-onnx-moonshine-tiny-en-int8/encode.int8.onnx";
    String uncachedDecoder = "./edgevox-onnx-moonshine-tiny-en-int8/uncached_decode.int8.onnx";
    String cachedDecoder = "./edgevox-onnx-moonshine-tiny-en-int8/cached_decode.int8.onnx";

    String tokens = "./edgevox-onnx-moonshine-tiny-en-int8/tokens.txt";

    String waveFilename = "./edgevox-onnx-moonshine-tiny-en-int8/test_wavs/0.wav";

    WaveReader reader = new WaveReader(waveFilename);

    OfflineMoonshineModelConfig moonshine =
        OfflineMoonshineModelConfig.builder()
            .setPreprocessor(preprocessor)
            .setEncoder(encoder)
            .setUncachedDecoder(uncachedDecoder)
            .setCachedDecoder(cachedDecoder)
            .build();

    OfflineModelConfig modelConfig =
        OfflineModelConfig.builder()
            .setMoonshine(moonshine)
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

    System.out.printf("filename:%s\nresult:%s\n", waveFilename, text);

    stream.release();
    recognizer.release();
  }
}
