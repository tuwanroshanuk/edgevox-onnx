import 'package:edgevox_onnx/edgevox_onnx.dart' as edgevox_onnx;
import './utils.dart';

final modelDir = 'assets';
// Remember to change `assets` in ../pubspec.yaml
// and download files to ../assets
Future<edgevox_onnx.OfflineModelConfig> getOfflineModelConfig(
    {required int type}) async {
  switch (type) {
    // whisper
    case 0: 
      return edgevox_onnx.OfflineModelConfig(
        whisper:edgevox_onnx.OfflineWhisperModelConfig(
          encoder: await copyAssetFile('$modelDir/whisper/base-encoder.onnx'),
          decoder: await copyAssetFile('$modelDir/whisper/base-decoder.onnx'),
        ),
        tokens: await copyAssetFile('$modelDir/whisper/base-tokens.txt'),
        modelType: 'whisper',
      );
    // senseVoice  
    case 1:
      return edgevox_onnx.OfflineModelConfig(
        senseVoice: edgevox_onnx.OfflineSenseVoiceModelConfig(
          model: await copyAssetFile('$modelDir/senseVoice/model.int8.onnx'), 
        ),
        tokens: await copyAssetFile('$modelDir/senseVoice/tokens.txt'),
      );
    // nemo_transducer-parakeet-tdt
    case 2:
      return edgevox_onnx.OfflineModelConfig(
        transducer: edgevox_onnx.OfflineTransducerModelConfig(
          encoder: await copyAssetFile(
              '$modelDir/nemo_transducer/encoder.int8.onnx'),
          decoder: await copyAssetFile(
              '$modelDir/nemo_transducer/decoder.int8.onnx'),
          joiner: await copyAssetFile(
              '$modelDir/nemo_transducer/joiner.int8.onnx'),
        ),
        tokens: await copyAssetFile('$modelDir/nemo_transducer/tokens.txt'),
        modelType: 'nemo_transducer',
      );
    default:
      throw ArgumentError('Unsupported type: $type');
  }
}
