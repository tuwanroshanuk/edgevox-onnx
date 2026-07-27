import 'dart:ffi';
import 'package:ffi/ffi.dart';

import './edgevox_onnx_bindings.dart';

/// Online punctuation restoration.
///
/// This wrapper is intended for shorter or incremental text fragments. See
/// `dart-api-examples/add-punctuations/` for working examples.
class OnlinePunctuationModelConfig {
  OnlinePunctuationModelConfig(
      {required this.cnnBiLstm,
      required this.bpeVocab,
      this.numThreads = 1,
      this.provider = 'cpu',
      this.debug = true});

  factory OnlinePunctuationModelConfig.fromJson(Map<String, dynamic> json) {
    return OnlinePunctuationModelConfig(
      cnnBiLstm: json['cnnBiLstm'],
      bpeVocab: json['bpeVocab'],
      numThreads: json['numThreads'],
      provider: json['provider'],
      debug: json['debug'],
    );
  }

  @override
  String toString() {
    return 'OnlinePunctuationModelConfig(cnnBiLstm: $cnnBiLstm, '
        'bpeVocab: $bpeVocab, numThreads: $numThreads, '
        'provider: $provider, debug: $debug)';
  }

  Map<String, dynamic> toJson() {
    return {
      'cnnBiLstm': cnnBiLstm,
      'bpeVocab': bpeVocab,
      'numThreads': numThreads,
      'provider': provider,
      'debug': debug,
    };
  }

  final String cnnBiLstm;
  final String bpeVocab;
  final int numThreads;
  final String provider;
  final bool debug;
}

/// Top-level configuration for [OnlinePunctuation].
class OnlinePunctuationConfig {
  OnlinePunctuationConfig({
    required this.model,
  });

  factory OnlinePunctuationConfig.fromJson(Map<String, dynamic> json) {
    return OnlinePunctuationConfig(
      model: OnlinePunctuationModelConfig.fromJson(json['model']),
    );
  }

  @override
  String toString() {
    return 'OnlinePunctuationConfig(model: $model)';
  }

  Map<String, dynamic> toJson() {
    return {
      'model': model.toJson(),
    };
  }

  final OnlinePunctuationModelConfig model;
}

/// Online punctuation restorer.
class OnlinePunctuation {
  OnlinePunctuation.fromPtr({required this.ptr, required this.config});

  OnlinePunctuation._({required this.ptr, required this.config});

  /// Create an online punctuator from [config].
  factory OnlinePunctuation({required OnlinePunctuationConfig config}) {
    if (EdgevoxOnnxBindings.edgevoxOnnxCreateOnlinePunctuation == null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    final c = calloc<EdgevoxOnnxOnlinePunctuationConfig>();

    final cnnBiLstmPtr = config.model.cnnBiLstm.toNativeUtf8();
    final bpeVocabPtr = config.model.bpeVocab.toNativeUtf8();
    c.ref.model.cnnBiLstm = cnnBiLstmPtr;
    c.ref.model.bpeVocab = bpeVocabPtr;
    c.ref.model.numThreads = config.model.numThreads;
    c.ref.model.debug = config.model.debug ? 1 : 0;

    final providerPtr = config.model.provider.toNativeUtf8();
    c.ref.model.provider = providerPtr;

    final ptr = EdgevoxOnnxBindings.edgevoxOnnxCreateOnlinePunctuation?.call(c) ??
        nullptr;

    calloc.free(providerPtr);
    calloc.free(cnnBiLstmPtr);
    calloc.free(bpeVocabPtr);
    calloc.free(c);

    if (ptr == nullptr) {
      throw Exception(
          "Failed to create online punctuation. Please check your config");
    }

    return OnlinePunctuation._(ptr: ptr, config: config);
  }

  /// Release the native punctuator.
  void free() {
    if (EdgevoxOnnxBindings.edgevoxOnnxDestroyOnlinePunctuation == null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    if (ptr == nullptr) {
      return;
    }
    EdgevoxOnnxBindings.edgevoxOnnxDestroyOnlinePunctuation?.call(ptr);
    ptr = nullptr;
  }

  /// Add punctuation to [text].
  String addPunct(String text) {
    if (EdgevoxOnnxBindings.edgevoxOnnxOnlinePunctuationAddPunct == null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    if (ptr == nullptr) {
      return '';
    }

    final textPtr = text.toNativeUtf8();

    final p = EdgevoxOnnxBindings.edgevoxOnnxOnlinePunctuationAddPunct
            ?.call(ptr, textPtr) ??
        nullptr;

    calloc.free(textPtr);

    if (p == nullptr) {
      return '';
    }

    final ans = p.toDartString();

    EdgevoxOnnxBindings.edgevoxOnnxOnlinePunctuationFreeText?.call(p);

    return ans;
  }

  Pointer<EdgevoxOnnxOnlinePunctuation> ptr;
  final OnlinePunctuationConfig config;
}
