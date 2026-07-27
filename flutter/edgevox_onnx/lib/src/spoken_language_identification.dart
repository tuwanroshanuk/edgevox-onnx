// Copyright (c)  2024  Xiaomi Corporation
import 'dart:ffi';

import 'package:ffi/ffi.dart';

import './offline_stream.dart';
import './edgevox_onnx_bindings.dart';
import './utils.dart';

/// Spoken language identification.
///
/// This module identifies the language spoken in an audio clip, using the
/// Whisper-based language ID model family exposed by the native library.
///
/// Example:
///
/// ```dart
/// final sli = SpokenLanguageIdentification(
///   SpokenLanguageIdentificationConfig(
///     whisper: const SpokenLanguageIdentificationWhisperConfig(
///       encoder: './edgevox-onnx-whisper-tiny/encoder.int8.onnx',
///       decoder: './edgevox-onnx-whisper-tiny/decoder.int8.onnx',
///     ),
///   ),
/// );
///
/// final stream = sli.createStream();
/// stream.acceptWaveform(samples: wave.samples, sampleRate: wave.sampleRate);
/// print(sli.compute(stream).lang);
/// ```
class SpokenLanguageIdentificationWhisperConfig {
  const SpokenLanguageIdentificationWhisperConfig({
    this.encoder = '',
    this.decoder = '',
    this.tailPaddings = 0,
  });

  factory SpokenLanguageIdentificationWhisperConfig.fromJson(
      Map<String, dynamic> json) {
    return SpokenLanguageIdentificationWhisperConfig(
      encoder: json['encoder'] as String? ?? '',
      decoder: json['decoder'] as String? ?? '',
      tailPaddings: json['tailPaddings'] as int? ?? 0,
    );
  }

  @override
  String toString() {
    return 'SpokenLanguageIdentificationWhisperConfig(encoder: $encoder, decoder: $decoder, tailPaddings: $tailPaddings)';
  }

  Map<String, dynamic> toJson() => {
        'encoder': encoder,
        'decoder': decoder,
        'tailPaddings': tailPaddings,
      };

  final String encoder;
  final String decoder;
  final int tailPaddings;
}

/// Top-level configuration for [SpokenLanguageIdentification].
class SpokenLanguageIdentificationConfig {
  const SpokenLanguageIdentificationConfig({
    this.whisper = const SpokenLanguageIdentificationWhisperConfig(),
    this.numThreads = 1,
    this.debug = false,
    this.provider = 'cpu',
  });

  factory SpokenLanguageIdentificationConfig.fromJson(
      Map<String, dynamic> json) {
    return SpokenLanguageIdentificationConfig(
      whisper: json['whisper'] != null
          ? SpokenLanguageIdentificationWhisperConfig.fromJson(
              json['whisper'] as Map<String, dynamic>)
          : const SpokenLanguageIdentificationWhisperConfig(),
      numThreads: json['numThreads'] as int? ?? 1,
      debug: json['debug'] as bool? ?? false,
      provider: json['provider'] as String? ?? 'cpu',
    );
  }

  @override
  String toString() {
    return 'SpokenLanguageIdentificationConfig(whisper: $whisper, numThreads: $numThreads, debug: $debug, provider: $provider)';
  }

  Map<String, dynamic> toJson() => {
        'whisper': whisper.toJson(),
        'numThreads': numThreads,
        'debug': debug,
        'provider': provider,
      };

  final SpokenLanguageIdentificationWhisperConfig whisper;
  final int numThreads;
  final bool debug;
  final String provider;
}

/// Result returned by [SpokenLanguageIdentification.compute].
class SpokenLanguageIdentificationResult {
  const SpokenLanguageIdentificationResult({
    required this.lang,
  });

  factory SpokenLanguageIdentificationResult.fromJson(
      Map<String, dynamic> json) {
    return SpokenLanguageIdentificationResult(
      lang: json['lang'] as String? ?? '',
    );
  }

  @override
  String toString() {
    return 'SpokenLanguageIdentificationResult(lang: $lang)';
  }

  Map<String, dynamic> toJson() => {
        'lang': lang,
      };

  final String lang;
}

/// Spoken language identifier.
class SpokenLanguageIdentification {
  SpokenLanguageIdentification.fromPtr(
      {required this.ptr, required this.config});

  SpokenLanguageIdentification._({required this.ptr, required this.config});

  /// Release the native language identifier.
  void free() {
    if (EdgevoxOnnxBindings.edgevoxOnnxDestroySpokenLanguageIdentification ==
        null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    if (ptr == nullptr) {
      return;
    }
    EdgevoxOnnxBindings.edgevoxOnnxDestroySpokenLanguageIdentification?.call(ptr);
    ptr = nullptr;
  }

  /// Create a language identifier from [config].
  factory SpokenLanguageIdentification(
      SpokenLanguageIdentificationConfig config) {
    final c = convertConfig(config);

    if (EdgevoxOnnxBindings.edgevoxOnnxCreateSpokenLanguageIdentification ==
        null) {
      freeConfig(c);
      throw Exception("Please initialize edgevox-onnx first");
    }

    final ptr = EdgevoxOnnxBindings.edgevoxOnnxCreateSpokenLanguageIdentification
            ?.call(c) ??
        nullptr;

    if (ptr == nullptr) {
      freeConfig(c);
      throw Exception(
          "Failed to create spoken language identification. Please check your config");
    }

    freeConfig(c);

    return SpokenLanguageIdentification._(ptr: ptr, config: config);
  }

  static Pointer<EdgevoxOnnxSpokenLanguageIdentificationConfig> convertConfig(
      SpokenLanguageIdentificationConfig config) {
    final c = calloc<EdgevoxOnnxSpokenLanguageIdentificationConfig>();

    c.ref.whisper.encoder = config.whisper.encoder.toNativeUtf8();
    c.ref.whisper.decoder = config.whisper.decoder.toNativeUtf8();
    c.ref.whisper.tailPaddings = config.whisper.tailPaddings;

    c.ref.numThreads = config.numThreads;
    c.ref.debug = config.debug ? 1 : 0;
    c.ref.provider = config.provider.toNativeUtf8();

    return c;
  }

  static void freeConfig(
      Pointer<EdgevoxOnnxSpokenLanguageIdentificationConfig> c) {
    malloc.free(c.ref.whisper.encoder);
    malloc.free(c.ref.whisper.decoder);
    malloc.free(c.ref.provider);
    malloc.free(c);
  }

  /// Create an offline stream for one audio clip.
  OfflineStream createStream() {
    if (EdgevoxOnnxBindings
            .edgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream ==
        null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    if (ptr == nullptr) {
      throw Exception("Failed to create offline stream");
    }

    final p = EdgevoxOnnxBindings
            .edgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream
            ?.call(ptr) ??
        nullptr;

    if (p == nullptr) {
      throw Exception("Failed to create offline stream");
    }

    return OfflineStream(ptr: p);
  }

  /// Compute the spoken language for [stream].
  SpokenLanguageIdentificationResult compute(OfflineStream stream) {
    if (EdgevoxOnnxBindings.edgevoxOnnxSpokenLanguageIdentificationCompute ==
        null) {
      throw Exception("Please initialize edgevox-onnx first");
    }

    if (ptr == nullptr || stream.ptr == nullptr) {
      return const SpokenLanguageIdentificationResult(lang: '');
    }

    final result = EdgevoxOnnxBindings
            .edgevoxOnnxSpokenLanguageIdentificationCompute
            ?.call(ptr, stream.ptr) ??
        nullptr;

    if (result == nullptr) {
      return const SpokenLanguageIdentificationResult(lang: '');
    }

    final lang = toDartString(result.ref.lang);

    EdgevoxOnnxBindings.edgevoxOnnxDestroySpokenLanguageIdentificationResult
        ?.call(result);

    return SpokenLanguageIdentificationResult(lang: lang);
  }

  Pointer<EdgevoxOnnxSpokenLanguageIdentification> ptr;
  SpokenLanguageIdentificationConfig config;
}
