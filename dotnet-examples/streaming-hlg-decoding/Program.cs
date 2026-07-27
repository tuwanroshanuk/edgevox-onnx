// Copyright (c)  2024  Xiaomi Corporation
//
// This file shows how to do streaming HLG decoding.
//
// 1. Download the model for testing
//
//  curl -SL -O https://github.com/k2-fsa/edgevox-onnx/releases/download/asr-models/edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
//  tar xvf edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
//  rm edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18.tar.bz2
//
// 2. Now run it
//
// dotnet run

using EdgevoxOnnx;

class StreamingHlgDecodingDemo
{
  static void Main(string[] args)
  {
    var config = new OnlineRecognizerConfig();
    config.FeatConfig.SampleRate = 16000;
    config.FeatConfig.FeatureDim = 80;
    config.ModelConfig.Zipformer2Ctc.Model = "./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/ctc-epoch-30-avg-3-chunk-16-left-128.int8.onnx";

    config.ModelConfig.Tokens = "./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/tokens.txt";
    config.ModelConfig.Provider = "cpu";
    config.ModelConfig.NumThreads = 1;
    config.ModelConfig.Debug = 0;
    config.CtcFstDecoderConfig.Graph = "./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/HLG.fst";

    var recognizer = new OnlineRecognizer(config);

    var filename = "./edgevox-onnx-streaming-zipformer-ctc-small-2024-03-18/test_wavs/8k.wav";

    var waveReader = new WaveReader(filename);
    var s = recognizer.CreateStream();
    s.AcceptWaveform(waveReader.SampleRate, waveReader.Samples);

    var tailPadding = new float[(int)(waveReader.SampleRate * 0.3)];
    s.AcceptWaveform(waveReader.SampleRate, tailPadding);
    s.InputFinished();

    while (recognizer.IsReady(s))
    {
      recognizer.Decode(s);
    }

    var r = recognizer.GetResult(s);
    var text = r.Text;
    var tokens = r.Tokens;
    Console.WriteLine("--------------------");
    Console.WriteLine(filename);
    Console.WriteLine("text: {0}", text);
    Console.WriteLine("tokens: [{0}]", string.Join(", ", tokens));
    Console.Write("timestamps: [");
    r.Timestamps.ToList().ForEach(i => Console.Write(string.Format("{0:0.00}", i) + ", "));
    Console.WriteLine("]");
    Console.WriteLine("--------------------");
  }
}
