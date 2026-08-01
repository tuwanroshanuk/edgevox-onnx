// edgevox-onnx/c-api/c-api.h
//
// Copyright (c)  2023  Xiaomi Corporation
/**
 * @file c-api.h
 * @brief Public C API for edgevox-onnx.
 *
 * This header exposes the main edgevox-onnx inference features through a stable
 * C interface. It is intended for native C/C++ applications and for language
 * bindings that need a C ABI.
 *
 * The file is organized by feature family. The major API groups are:
 *
 * - Utility helpers: version/build information, file checks, WAVE I/O, and a
 *   display helper for incremental text output
 * - Streaming ASR: online recognizers, online streams, endpointing, and
 *   per-stream runtime options
 * - Non-streaming ASR: offline recognizers, offline streams, batch decode, and
 *   result retrieval
 * - Keyword spotting: streaming keyword detection, custom keyword streams, and
 *   keyword result snapshots
 * - Voice activity detection: Silero/Ten VAD models, speech segment buffers,
 *   and detector state management
 * - Text-to-speech: offline TTS model families, generation configuration, and
 *   generated audio helpers
 * - Spoken language identification
 * - Speaker embedding extraction and speaker enrollment/search/verification
 * - Audio tagging
 * - Offline and online punctuation restoration
 * - Linear resampling
 * - Offline speaker diarization
 * - Offline and online speech enhancement / denoising
 * - HarmonyOS-specific constructor variants
 *
 * Common ownership rules:
 *
 * - Opaque handles created by `EdgevoxOnnxCreate*()` functions are generally
 *   destroyed with a matching `EdgevoxOnnxDestroy*()` function
 * - Snapshot/result objects returned by query functions usually need explicit
 *   destruction as documented on each API
 * - Strings or arrays returned by helper/query functions are either:
 *   - statically owned by the library and must not be freed, or
 *   - heap-allocated for the caller and must be released with the matching
 *     `Free`/`Destroy` API
 *
 * General usage pattern:
 *
 * 1. Zero-initialize a config struct with `memset(&config, 0, sizeof(config))`
 * 2. Fill in the required model paths and runtime options
 * 3. Create the corresponding engine with `EdgevoxOnnxCreate*()`
 * 4. Create a stream if the feature uses one
 * 5. Feed audio or text, run the compute/decode API, and retrieve results
 * 6. Release every returned object with the documented matching API
 *
 * The examples in `c-api-examples/` show complete end-to-end usage. Useful
 * starting points include:
 *
 * - `decode-file-c-api.c` for ASR
 * - `kws-c-api.c` for keyword spotting
 * - `vad-whisper-c-api.c` for VAD
 * - `offline-tts-c-api.c` and `kokoro-tts-en-c-api.c` for TTS
 * - `speaker-identification-c-api.c` for speaker embedding and verification
 * - `audio-tagging-c-api.c` for audio tagging
 * - `add-punctuation-c-api.c` and `add-punctuation-online-c-api.c` for
 *   punctuation
 * - `offline-sepaker-diarization-c-api.c` for diarization
 * - `speech-enhancement-gtcrn-c-api.c` and
 *   `online-speech-enhancement-gtcrn-c-api.c` for speech enhancement
 */

#ifndef EDGEVOX_ONNX_C_API_C_API_H_
#define EDGEVOX_ONNX_C_API_C_API_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// See https://github.com/pytorch/pytorch/blob/main/c10/macros/Export.h
// We will set EDGEVOX_ONNX_BUILD_SHARED_LIBS and EDGEVOX_ONNX_BUILD_MAIN_LIB in
// CMakeLists.txt

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

#if defined(_WIN32)
#if defined(EDGEVOX_ONNX_BUILD_SHARED_LIBS)
#define EDGEVOX_ONNX_EXPORT __declspec(dllexport)
#define EDGEVOX_ONNX_IMPORT __declspec(dllimport)
#else
#define EDGEVOX_ONNX_EXPORT
#define EDGEVOX_ONNX_IMPORT
#endif
#else  // WIN32
#define EDGEVOX_ONNX_EXPORT __attribute__((visibility("default")))

#define EDGEVOX_ONNX_IMPORT EDGEVOX_ONNX_EXPORT
#endif  // WIN32

#if defined(EDGEVOX_ONNX_BUILD_MAIN_LIB)
#define EDGEVOX_ONNX_API EDGEVOX_ONNX_EXPORT
#else
#define EDGEVOX_ONNX_API EDGEVOX_ONNX_IMPORT
#endif

#ifndef EDGEVOX_ONNX_DEPRECATED
#if defined(_MSC_VER)
#define EDGEVOX_ONNX_DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
#define EDGEVOX_ONNX_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define EDGEVOX_ONNX_DEPRECATED(msg)
#endif
#endif

/**
 * @brief Return the edgevox-onnx version string.
 *
 * The returned pointer refers to statically allocated memory owned by the
 * library. Do not free it and do not modify it.
 *
 * @return Version string, for example `"1.12.1"`.
 *
 * @code
 * printf("edgevox-onnx version: %s\n", EdgevoxOnnxGetVersionStr());
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetVersionStr();

/**
 * @brief Return the Git SHA1 used to build the library.
 *
 * The returned pointer refers to statically allocated memory owned by the
 * library. Do not free it and do not modify it.
 *
 * @return Short Git SHA1 string, for example `"6982b86c"`.
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetGitSha1();

/**
 * @brief Return the Git build date used to build the library.
 *
 * The returned pointer refers to statically allocated memory owned by the
 * library. Do not free it and do not modify it.
 *
 * @return Build date string, for example `"Fri Jun 20 11:22:52 2025"`.
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetGitDate();

/**
 * @brief Check whether a file exists.
 *
 * @param filename File path to test.
 * @return 1 if the file exists; otherwise 0.
 *
 * @code
 * if (!EdgevoxOnnxFileExists("./Obama.wav")) {
 *   fprintf(stderr, "Please download Obama.wav\n");
 * }
 * @endcode
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxFileExists(const char *filename);

/**
 * @brief Configuration for a streaming transducer model.
 *
 * Please refer to
 * https://k2-fsa.github.io/sherpa/onnx/pretrained_models/index.html
 * to download compatible pre-trained models.
 */
typedef struct EdgevoxOnnxOnlineTransducerModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
  /** Path to the joiner ONNX model. */
  const char *joiner;
} EdgevoxOnnxOnlineTransducerModelConfig;

/**
 * @brief Configuration for a streaming Paraformer model.
 *
 * Please visit
 * https://k2-fsa.github.io/sherpa/onnx/pretrained_models/online-paraformer/index.html
 * to download compatible models.
 */
typedef struct EdgevoxOnnxOnlineParaformerModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
} EdgevoxOnnxOnlineParaformerModelConfig;

/**
 * @brief Configuration for a streaming Zipformer2 CTC model.
 */
typedef struct EdgevoxOnnxOnlineZipformer2CtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOnlineZipformer2CtcModelConfig;

/** @brief Configuration for a streaming NeMo CTC model. */
typedef struct EdgevoxOnnxOnlineNemoCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOnlineNemoCtcModelConfig;

/** @brief Configuration for a streaming T-One CTC model. */
typedef struct EdgevoxOnnxOnlineToneCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOnlineToneCtcModelConfig;

/**
 * @brief Model configuration shared by streaming ASR recognizers.
 *
 * Zero-initialize this struct before use, then fill in the sub-config for the
 * model family you want to use together with the shared fields such as
 * @c tokens, @c provider, and @c num_threads.
 *
 * Exactly one model family should be configured for each recognizer. For
 * example, set only one of @c transducer, @c paraformer, @c zipformer2_ctc,
 * @c nemo_ctc, or @c t_one_ctc.
 *
 * If multiple model families are configured at the same time, the
 * implementation will choose one of them, and which one is used is
 * implementation-defined. Do not rely on any precedence rule.
 */
typedef struct EdgevoxOnnxOnlineModelConfig {
  /** Streaming transducer model files. */
  EdgevoxOnnxOnlineTransducerModelConfig transducer;
  /** Streaming Paraformer model files. */
  EdgevoxOnnxOnlineParaformerModelConfig paraformer;
  /** Streaming Zipformer2 CTC model files. */
  EdgevoxOnnxOnlineZipformer2CtcModelConfig zipformer2_ctc;
  /** Path to the tokens file. */
  const char *tokens;
  /** Number of threads used by the ONNX Runtime backend. */
  int32_t num_threads;
  /** Execution provider, for example "cpu", "cuda", or "coreml". */
  const char *provider;
  /** Non-zero to print model debug information. */
  int32_t debug;
  /** Optional explicit model type override. */
  const char *model_type;
  /**
   * Modeling unit used by the tokens.
   *
   * Valid values include:
   * - "cjkchar"
   * - "bpe"
   * - "cjkchar+bpe"
   */
  const char *modeling_unit;
  /** Path to the BPE vocabulary file when BPE is used. */
  const char *bpe_vocab;
  /** Optional in-memory tokens data. Used instead of @c tokens when non-NULL.
   */
  const char *tokens_buf;
  /** Size in bytes of @c tokens_buf, excluding the trailing '\0'. */
  int32_t tokens_buf_size;
  /** Streaming NeMo CTC model files. */
  EdgevoxOnnxOnlineNemoCtcModelConfig nemo_ctc;
  /** Streaming T-One CTC model files. */
  EdgevoxOnnxOnlineToneCtcModelConfig t_one_ctc;
} EdgevoxOnnxOnlineModelConfig;

/**
 * @brief Feature extraction settings for ASR.
 *
 * The bundled ASR models typically expect 16 kHz mono audio and 80-bin
 * features.
 */
typedef struct EdgevoxOnnxFeatureConfig {
  /** Sample rate expected by the model, for example 16000. */
  int32_t sample_rate;

  /** Feature dimension expected by the model, for example 80. */
  int32_t feature_dim;
} EdgevoxOnnxFeatureConfig;

/** @brief Configuration for HLG/FST-based online CTC decoding. */
typedef struct EdgevoxOnnxOnlineCtcFstDecoderConfig {
  /** Path to the decoding graph. */
  const char *graph;
  /** Decoder max-active setting. */
  int32_t max_active;
} EdgevoxOnnxOnlineCtcFstDecoderConfig;

/** @brief Configuration for homophone replacement. */
typedef struct EdgevoxOnnxHomophoneReplacerConfig {
  /** Unused legacy field kept for ABI compatibility. */
  const char *dict_dir;
  /** Path to the lexicon used by the homophone replacer. */
  const char *lexicon;
  /** Path to the replacement rule FST file. */
  const char *rule_fsts;
} EdgevoxOnnxHomophoneReplacerConfig;

/**
 * @brief Configuration for a streaming ASR recognizer.
 *
 * Zero-initialize this struct before use. Then fill in @c feat_config,
 * @c model_config, and any optional decoding, endpoint, or hotword settings.
 *
 * Example model package:
 * `sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20`
 *
 * @code
 * EdgevoxOnnxOnlineRecognizerConfig config;
 * memset(&config, 0, sizeof(config));
 *
 * config.feat_config.sample_rate = 16000;
 * config.feat_config.feature_dim = 80;
 *
 * config.model_config.transducer.encoder =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "encoder-epoch-99-avg-1.int8.onnx";
 * config.model_config.transducer.decoder =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "decoder-epoch-99-avg-1.onnx";
 * config.model_config.transducer.joiner =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "joiner-epoch-99-avg-1.int8.onnx";
 * config.model_config.tokens =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "tokens.txt";
 * config.model_config.provider = "cpu";
 * config.model_config.num_threads = 1;
 *
 * config.decoding_method = "greedy_search";
 * @endcode
 * @see EdgevoxOnnxCreateOnlineRecognizer
 */
typedef struct EdgevoxOnnxOnlineRecognizerConfig {
  /** Feature extraction settings. */
  EdgevoxOnnxFeatureConfig feat_config;
  /** Streaming model configuration. */
  EdgevoxOnnxOnlineModelConfig model_config;

  /** Decoding method, for example "greedy_search" or "modified_beam_search". */
  const char *decoding_method;

  /** Number of active paths for modified beam search. */
  int32_t max_active_paths;

  /** Set to non-zero to enable endpoint detection. */
  int32_t enable_endpoint;

  /** Endpoint rule 1 trailing silence threshold in seconds. */
  float rule1_min_trailing_silence;

  /** Endpoint rule 2 trailing silence threshold in seconds. */
  float rule2_min_trailing_silence;

  /** Endpoint rule 3 utterance-length threshold in seconds. */
  float rule3_min_utterance_length;

  /** Path to a hotwords file. */
  const char *hotwords_file;

  /** Bonus score added to each hotword token during decoding. */
  float hotwords_score;

  /** Optional HLG/FST online CTC decoder configuration. */
  EdgevoxOnnxOnlineCtcFstDecoderConfig ctc_fst_decoder_config;
  /** Path to punctuation or text-processing rule FSTs. */
  const char *rule_fsts;
  /** Path to FAR archives used by text-processing rules. */
  const char *rule_fars;
  /** Optional blank penalty applied during decoding. */
  float blank_penalty;

  /** Optional in-memory hotwords text used instead of @c hotwords_file. */
  const char *hotwords_buf;
  /** Size in bytes of @c hotwords_buf, excluding the trailing '\0'. */
  int32_t hotwords_buf_size;
  /** Optional homophone replacement configuration. */
  EdgevoxOnnxHomophoneReplacerConfig hr;
} EdgevoxOnnxOnlineRecognizerConfig;

/**
 * @brief Incremental recognition result for a streaming ASR stream.
 *
 * All pointers in this struct are owned by the result object returned from
 * EdgevoxOnnxGetOnlineStreamResult() and become invalid after
 * EdgevoxOnnxDestroyOnlineRecognizerResult() is called.
 * @see EdgevoxOnnxGetOnlineStreamResult
 */
typedef struct EdgevoxOnnxOnlineRecognizerResult {
  /** Recognized text accumulated so far. */
  const char *text;

  /**
   * Contiguous memory block containing token strings separated by '\0'.
   *
   * Use @c tokens_arr for convenient indexed access.
   */
  const char *tokens;

  /** Array of @c count pointers into @c tokens. */
  const char *const *tokens_arr;

  /**
   * Optional token timestamps in seconds.
   *
   * This field may be NULL when the model does not provide timestamps.
   * When non-NULL, it contains @c count entries and is parallel to
   * @c tokens_arr.
   */
  float *timestamps;

  /** Number of entries in @c tokens_arr and, when available, @c timestamps. */
  int32_t count;

  /** JSON serialization of the result. */
  const char *json;
} EdgevoxOnnxOnlineRecognizerResult;

/** @brief Streaming recognizer handle. */
typedef struct EdgevoxOnnxOnlineRecognizer EdgevoxOnnxOnlineRecognizer;
/** @brief Streaming decoding state for one utterance or stream. */
typedef struct EdgevoxOnnxOnlineStream EdgevoxOnnxOnlineStream;

/**
 * @brief Create a streaming ASR recognizer.
 *
 * The returned recognizer runs locally and does not require Internet access.
 *
 * @param config Recognizer configuration.
 * @return A recognizer handle on success, or NULL if the configuration is
 *         invalid. The caller owns the returned object and must free it with
 *         EdgevoxOnnxDestroyOnlineRecognizer().
 *
 * @code
 * EdgevoxOnnxOnlineRecognizerConfig config;
 * memset(&config, 0, sizeof(config));
 * config.feat_config.sample_rate = 16000;
 * config.feat_config.feature_dim = 80;
 * config.model_config.transducer.encoder =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "encoder-epoch-99-avg-1.int8.onnx";
 * config.model_config.transducer.decoder =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "decoder-epoch-99-avg-1.onnx";
 * config.model_config.transducer.joiner =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "joiner-epoch-99-avg-1.int8.onnx";
 * config.model_config.tokens =
 *     "./sherpa-onnx-streaming-zipformer-bilingual-zh-en-2023-02-20/"
 *     "tokens.txt";
 * config.model_config.provider = "cpu";
 * config.model_config.num_threads = 1;
 * config.decoding_method = "greedy_search";
 *
 * const EdgevoxOnnxOnlineRecognizer *recognizer =
 *     EdgevoxOnnxCreateOnlineRecognizer(&config);
 * @endcode
 * @see EdgevoxOnnxOnlineRecognizerConfig, EdgevoxOnnxDestroyOnlineRecognizer
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineRecognizer *
EdgevoxOnnxCreateOnlineRecognizer(
    const EdgevoxOnnxOnlineRecognizerConfig *config);

/**
 * @brief Destroy a streaming recognizer.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 *
 * @code
 * EdgevoxOnnxDestroyOnlineRecognizer(recognizer);
 * recognizer = NULL;
 * @endcode
 * @see EdgevoxOnnxCreateOnlineRecognizer
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlineRecognizer(
    const EdgevoxOnnxOnlineRecognizer *recognizer);

/**
 * @brief Create a streaming ASR state object.
 *
 * One stream corresponds to one decoding state. Reuse the same recognizer to
 * create multiple streams.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @return A newly created stream. The caller owns the returned object and must
 *         free it with EdgevoxOnnxDestroyOnlineStream().
 *
 * @code
 * const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave(
 *     "./sherpa-onnx-streaming-paraformer-bilingual-zh-en/test_wavs/0.wav");
 * const EdgevoxOnnxOnlineStream *stream =
 *     EdgevoxOnnxCreateOnlineStream(recognizer);
 * @endcode
 * @see EdgevoxOnnxDestroyOnlineStream
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateOnlineStream(
    const EdgevoxOnnxOnlineRecognizer *recognizer);

/**
 * @brief Create a streaming ASR state object with per-stream hotwords.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param hotwords Hotwords text to associate with the stream.
 * @return A newly created stream. The caller owns the returned object and must
 *         free it with EdgevoxOnnxDestroyOnlineStream().
 *
 * @code
 * const EdgevoxOnnxOnlineStream *stream =
 *     EdgevoxOnnxCreateOnlineStreamWithHotwords(recognizer,
 *                                              "▁HELLO ▁WORLD");
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineStream *
EdgevoxOnnxCreateOnlineStreamWithHotwords(
    const EdgevoxOnnxOnlineRecognizer *recognizer, const char *hotwords);

/**
 * @brief Destroy a streaming ASR state object.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream() or
 *               EdgevoxOnnxCreateOnlineStreamWithHotwords().
 *
 * @code
 * EdgevoxOnnxDestroyOnlineStream(stream);
 * stream = NULL;
 * @endcode
 * @see EdgevoxOnnxCreateOnlineStream
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlineStream(
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Append audio samples to a streaming ASR stream.
 *
 * The input is mono floating-point PCM normalized to the range [-1, 1].
 * If @p sample_rate differs from the recognizer feature sample rate,
 * edgevox-onnx resamples internally.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @param sample_rate Sample rate of @p samples.
 * @param samples Pointer to @p n samples in the range [-1, 1].
 * @param n Number of samples.
 *
 * @code
 * int32_t start = 0;
 * int32_t chunk_size = 3200;  // 0.2 seconds at 16 kHz
 * EdgevoxOnnxOnlineStreamAcceptWaveform(stream, wave->sample_rate,
 *                                      wave->samples + start, chunk_size);
 * @endcode
 * @see EdgevoxOnnxCreateOnlineStream, EdgevoxOnnxDecodeOnlineStream
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlineStreamAcceptWaveform(
    const EdgevoxOnnxOnlineStream *stream, int32_t sample_rate,
    const float *samples, int32_t n);

/**
 * @brief Check whether a streaming ASR stream is ready to decode.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @return 1 if enough frames are available for decoding; otherwise 0.
 *
 * @code
 * if (EdgevoxOnnxIsOnlineStreamReady(recognizer, stream)) {
 *   EdgevoxOnnxDecodeOnlineStream(recognizer, stream);
 * }
 * @endcode
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxIsOnlineStreamReady(const EdgevoxOnnxOnlineRecognizer *recognizer,
                              const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Decode one step of a streaming ASR stream.
 *
 * Call this only when EdgevoxOnnxIsOnlineStreamReady() returns 1.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 *
 * @code
 * EdgevoxOnnxOnlineStreamAcceptWaveform(stream, sample_rate, samples, n);
 * while (EdgevoxOnnxIsOnlineStreamReady(recognizer, stream)) {
 *   EdgevoxOnnxDecodeOnlineStream(recognizer, stream);
 * }
 * @endcode
 * @see EdgevoxOnnxIsOnlineStreamReady, EdgevoxOnnxGetOnlineStreamResult
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeOnlineStream(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Decode multiple streaming ASR streams in parallel.
 *
 * The caller must ensure every stream in @p streams is ready before calling
 * this function.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param streams Array of @p n stream pointers.
 * @param n Number of streams in @p streams.
 *
 * @code
 * const EdgevoxOnnxOnlineStream *streams[2] = {stream1, stream2};
 * EdgevoxOnnxDecodeMultipleOnlineStreams(recognizer, streams, 2);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeMultipleOnlineStreams(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream **streams, int32_t n);

/**
 * @brief Get the current streaming ASR result for a stream.
 *
 * The returned snapshot is independent from the stream state. The caller owns
 * it and must free it with EdgevoxOnnxDestroyOnlineRecognizerResult().
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @return A newly allocated result snapshot.
 *
 * @code
 * const EdgevoxOnnxOnlineRecognizerResult *r =
 *     EdgevoxOnnxGetOnlineStreamResult(recognizer, stream);
 * printf("%s\n", r->text);
 * // r->tokens_arr[i] and r->timestamps[i] are parallel when timestamps
 * // are available.
 * EdgevoxOnnxDestroyOnlineRecognizerResult(r);
 * @endcode
 * @see EdgevoxOnnxDestroyOnlineRecognizerResult
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineRecognizerResult *
EdgevoxOnnxGetOnlineStreamResult(const EdgevoxOnnxOnlineRecognizer *recognizer,
                                const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Destroy a result returned by EdgevoxOnnxGetOnlineStreamResult().
 *
 * @param r A pointer returned by EdgevoxOnnxGetOnlineStreamResult().
 *
 * @code
 * EdgevoxOnnxDestroyOnlineRecognizerResult(r);
 * r = NULL;
 * @endcode
 * @see EdgevoxOnnxGetOnlineStreamResult
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlineRecognizerResult(
    const EdgevoxOnnxOnlineRecognizerResult *r);

/**
 * @brief Get the current streaming ASR result as JSON.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @return A newly allocated JSON string. Free it with
 *         EdgevoxOnnxDestroyOnlineStreamResultJson().
 *
 * @code
 * const char *json =
 *     EdgevoxOnnxGetOnlineStreamResultAsJson(recognizer, stream);
 * puts(json);
 * EdgevoxOnnxDestroyOnlineStreamResultJson(json);
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetOnlineStreamResultAsJson(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Free a JSON string returned by
 * EdgevoxOnnxGetOnlineStreamResultAsJson().
 *
 * @param s A pointer returned by EdgevoxOnnxGetOnlineStreamResultAsJson().
 *
 * @code
 * EdgevoxOnnxDestroyOnlineStreamResultJson(json);
 * json = NULL;
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlineStreamResultJson(const char *s);

/**
 * @brief Reset a streaming ASR stream after an endpoint or utterance boundary.
 *
 * This clears the decoder state for the stream so that it can be reused for a
 * new utterance.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 *
 * @code
 * if (EdgevoxOnnxOnlineStreamIsEndpoint(recognizer, stream)) {
 *   EdgevoxOnnxOnlineStreamReset(recognizer, stream);
 * }
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlineStreamReset(
    const EdgevoxOnnxOnlineRecognizer *recognizer,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Signal end-of-input for a streaming ASR stream.
 *
 * After calling this function, do not append more samples to the stream.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 *
 * @code
 * EdgevoxOnnxOnlineStreamInputFinished(stream);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlineStreamInputFinished(
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Set a per-stream runtime option.
 *
 * This is a generic extension point for model-specific or runtime-specific
 * options such as "is_final" for streaming Paraformer.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @param key Option name.
 * @param value Option value represented as text.
 *
 * @code
 * EdgevoxOnnxOnlineStreamSetOption(stream, "is_final", "1");
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlineStreamSetOption(
    const EdgevoxOnnxOnlineStream *stream, const char *key, const char *value);

/**
 * @brief Get a per-stream runtime option.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @param key Option name.
 * @return The option value. The returned pointer is owned by the stream, must
 *         not be freed by the caller, and may be invalidated if the option is
 *         overwritten or the stream is destroyed.
 *
 * @code
 * const char *value = EdgevoxOnnxOnlineStreamGetOption(stream, "is_final");
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxOnlineStreamGetOption(
    const EdgevoxOnnxOnlineStream *stream, const char *key);

/**
 * @brief Check whether a per-stream runtime option exists.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @param key Option name.
 * @return 1 if the option exists; otherwise 0.
 *
 * @code
 * int32_t has_option = EdgevoxOnnxOnlineStreamHasOption(stream, "is_final");
 * @endcode
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOnlineStreamHasOption(
    const EdgevoxOnnxOnlineStream *stream, const char *key);

/**
 * @brief Check whether endpoint detection has triggered for a stream.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOnlineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOnlineStream().
 * @return 1 if an endpoint is detected; otherwise 0.
 *
 * @code
 * if (EdgevoxOnnxOnlineStreamIsEndpoint(recognizer, stream)) {
 *   EdgevoxOnnxOnlineStreamReset(recognizer, stream);
 * }
 * @endcode
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxOnlineStreamIsEndpoint(const EdgevoxOnnxOnlineRecognizer *recognizer,
                                 const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Helper for pretty-printing incremental recognition results.
 *
 * This utility is mainly used by example programs on Linux and macOS.
 */
typedef struct EdgevoxOnnxDisplay EdgevoxOnnxDisplay;

/**
 * @brief Create a display helper.
 *
 * @param max_word_per_line Maximum number of words to show per line.
 * @return A newly allocated display helper. Free it with
 *         EdgevoxOnnxDestroyDisplay().
 *
 * @code
 * const EdgevoxOnnxDisplay *display = EdgevoxOnnxCreateDisplay(50);
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxDisplay *EdgevoxOnnxCreateDisplay(
    int32_t max_word_per_line);

/**
 * @brief Destroy a display helper.
 *
 * @param display A pointer returned by EdgevoxOnnxCreateDisplay().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyDisplay(const EdgevoxOnnxDisplay *display);

/**
 * @brief Print one line of text using the display helper.
 *
 * @param display A pointer returned by EdgevoxOnnxCreateDisplay().
 * @param idx Segment or utterance index to print.
 * @param s Text to print.
 *
 * @code
 * EdgevoxOnnxPrint(display, segment_id, r->text);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxPrint(const EdgevoxOnnxDisplay *display,
                                     int32_t idx, const char *s);
// ============================================================
// For offline ASR (i.e., non-streaming ASR)
// ============================================================

/**
 * @brief Configuration for a non-streaming transducer model.
 */
typedef struct EdgevoxOnnxOfflineTransducerModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
  /** Path to the joiner ONNX model. */
  const char *joiner;
} EdgevoxOnnxOfflineTransducerModelConfig;

/** @brief Configuration for a non-streaming Paraformer model. */
typedef struct EdgevoxOnnxOfflineParaformerModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineParaformerModelConfig;

/** @brief Configuration for a non-streaming NeMo CTC model. */
typedef struct EdgevoxOnnxOfflineNemoEncDecCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineNemoEncDecCtcModelConfig;

/**
 * @brief Configuration for a non-streaming Whisper model.
 */
typedef struct EdgevoxOnnxOfflineWhisperModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
  /** Optional language hint, for example "en" or "zh". */
  const char *language;
  /** Optional Whisper task such as "transcribe" or "translate". */
  const char *task;
  /** Number of tail padding frames appended internally. */
  int32_t tail_paddings;

  /** Non-zero to enable token-level timestamps when supported by the model. */
  int32_t enable_token_timestamps;

  /** Non-zero to enable Whisper segment-level timestamps. */
  int32_t enable_segment_timestamps;
} EdgevoxOnnxOfflineWhisperModelConfig;

/** @brief Configuration for a Canary model. */
typedef struct EdgevoxOnnxOfflineCanaryModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
  /** Source language hint. */
  const char *src_lang;
  /** Target language hint. */
  const char *tgt_lang;
  /** Non-zero to enable punctuation and capitalization when supported. */
  int32_t use_pnc;
} EdgevoxOnnxOfflineCanaryModelConfig;

/** @brief Configuration for a Cohere Transcribe model. */
typedef struct EdgevoxOnnxOfflineCohereTranscribeModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
  /** Optional language hint, for example "en" or "zh". */
  const char *language;
  /** Non-zero to enable punctuation. */
  int32_t use_punct;
  /** Non-zero to enable inverse text normalization. */
  int32_t use_itn;
} EdgevoxOnnxOfflineCohereTranscribeModelConfig;

/** @brief Configuration for a FireRedAsr encoder/decoder model. */
typedef struct EdgevoxOnnxOfflineFireRedAsrModelConfig {
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model. */
  const char *decoder;
} EdgevoxOnnxOfflineFireRedAsrModelConfig;

/** @brief Configuration for a FireRedAsr CTC model. */
typedef struct EdgevoxOnnxOfflineFireRedAsrCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineFireRedAsrCtcModelConfig;

/** @brief Configuration for a Moonshine model. */
typedef struct EdgevoxOnnxOfflineMoonshineModelConfig {
  /** Path to the preprocessor ONNX model. */
  const char *preprocessor;
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the uncached decoder ONNX model. */
  const char *uncached_decoder;
  /** Path to the cached decoder ONNX model. */
  const char *cached_decoder;
  /** Path to the merged decoder ONNX model. */
  const char *merged_decoder;
} EdgevoxOnnxOfflineMoonshineModelConfig;

/** @brief Configuration for a TDNN model. */
typedef struct EdgevoxOnnxOfflineTdnnModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineTdnnModelConfig;

/** @brief Configuration for an offline language model. */
typedef struct EdgevoxOnnxOfflineLMConfig {
  /** Path to the language model. */
  const char *model;
  /** Interpolation scale for the language model. */
  float scale;
} EdgevoxOnnxOfflineLMConfig;

/** @brief Configuration for a SenseVoice model. */
typedef struct EdgevoxOnnxOfflineSenseVoiceModelConfig {
  /** Path to the ONNX model. */
  const char *model;
  /** Optional language hint. */
  const char *language;
  /** Non-zero to enable inverse text normalization. */
  int32_t use_itn;
} EdgevoxOnnxOfflineSenseVoiceModelConfig;

/** @brief Configuration for a Dolphin model. */
typedef struct EdgevoxOnnxOfflineDolphinModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineDolphinModelConfig;

/** @brief Configuration for an offline Zipformer CTC model. */
typedef struct EdgevoxOnnxOfflineZipformerCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineZipformerCtcModelConfig;

/** @brief Configuration for an offline WeNet CTC model. */
typedef struct EdgevoxOnnxOfflineWenetCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineWenetCtcModelConfig;

/** @brief Configuration for an omnilingual offline CTC model. */
typedef struct EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig;

/** @brief Configuration for an offline FunASR Nano model. */
typedef struct EdgevoxOnnxOfflineFunASRNanoModelConfig {
  /** Path to the encoder adaptor. */
  const char *encoder_adaptor;
  /** Path to the LLM ONNX model. */
  const char *llm;
  /** Path to the embedding model. */
  const char *embedding;
  /** Path to the tokenizer file. */
  const char *tokenizer;
  /** System prompt. */
  const char *system_prompt;
  /** User prompt. */
  const char *user_prompt;
  /** Maximum number of generated tokens. */
  int32_t max_new_tokens;
  /** Sampling temperature. */
  float temperature;
  /** Top-p sampling threshold. */
  float top_p;
  /** Random seed. */
  int32_t seed;
  /** Optional language hint. */
  const char *language;
  /** Non-zero to enable inverse text normalization. */
  int32_t itn;
  /** Optional hotwords text. */
  const char *hotwords;
} EdgevoxOnnxOfflineFunASRNanoModelConfig;

/** @brief Configuration for an offline Qwen3-ASR model. */
typedef struct EdgevoxOnnxOfflineQwen3ASRModelConfig {
  /** Path to the conv-frontend ONNX model. */
  const char *conv_frontend;
  /** Path to the encoder ONNX model. */
  const char *encoder;
  /** Path to the decoder ONNX model (with KV cache). */
  const char *decoder;
  /** Path to the tokenizer directory (e.g. containing `vocab.json`). */
  const char *tokenizer;
  /** Maximum total sequence length supported by the model. */
  int32_t max_total_len;
  /** Maximum number of new tokens to generate. */
  int32_t max_new_tokens;
  /** Sampling temperature. */
  float temperature;
  /** Top-p (nucleus) sampling threshold. */
  float top_p;
  /** Random seed for reproducible sampling. */
  int32_t seed;
  /** Optional comma-separated hotwords (UTF-8, ASCII ','), e.g. @c
   * "foo,bar,baz". */
  const char *hotwords;
} EdgevoxOnnxOfflineQwen3ASRModelConfig;

/** @brief Configuration for a MedASR CTC model. */
typedef struct EdgevoxOnnxOfflineMedAsrCtcModelConfig {
  /** Path to the ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineMedAsrCtcModelConfig;

/**
 * @brief Model configuration shared by offline ASR recognizers.
 *
 * Zero-initialize this struct before use, then fill in exactly the sub-config
 * needed by the model family you want to run.
 *
 * Exactly one model family should be configured for each recognizer. For
 * example, set only one of @c transducer, @c paraformer, @c nemo_ctc,
 * @c whisper, @c tdnn, @c sense_voice, @c moonshine, @c fire_red_asr,
 * @c dolphin, @c zipformer_ctc, @c canary, @c cohere_transcribe,
 * @c wenet_ctc, @c omnilingual, @c medasr, @c funasr_nano,
 * @c fire_red_asr_ctc, or @c qwen3_asr.
 *
 * If multiple model families are configured at the same time, the
 * implementation will choose one of them, and which one is used is
 * implementation-defined. Do not rely on any precedence rule.
 */
typedef struct EdgevoxOnnxOfflineModelConfig {
  /** Non-streaming transducer model files. */
  EdgevoxOnnxOfflineTransducerModelConfig transducer;
  /** Non-streaming Paraformer model files. */
  EdgevoxOnnxOfflineParaformerModelConfig paraformer;
  /** Non-streaming NeMo CTC model files. */
  EdgevoxOnnxOfflineNemoEncDecCtcModelConfig nemo_ctc;
  /** Whisper model files and options. */
  EdgevoxOnnxOfflineWhisperModelConfig whisper;
  /** TDNN model files. */
  EdgevoxOnnxOfflineTdnnModelConfig tdnn;

  /** Path to the tokens file. */
  const char *tokens;
  /** Number of backend threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider, for example "cpu" or "cuda". */
  const char *provider;
  /** Optional explicit model type override. */
  const char *model_type;
  /** Modeling unit, such as "cjkchar", "bpe", or "cjkchar+bpe". */
  const char *modeling_unit;
  /** Path to the BPE vocabulary file when BPE is used. */
  const char *bpe_vocab;
  /** Path to the TeleSpeech CTC model. */
  const char *telespeech_ctc;
  /** SenseVoice configuration. */
  EdgevoxOnnxOfflineSenseVoiceModelConfig sense_voice;
  /** Moonshine configuration. */
  EdgevoxOnnxOfflineMoonshineModelConfig moonshine;
  /** FireRedAsr configuration. */
  EdgevoxOnnxOfflineFireRedAsrModelConfig fire_red_asr;
  /** Dolphin configuration. */
  EdgevoxOnnxOfflineDolphinModelConfig dolphin;
  /** Zipformer CTC configuration. */
  EdgevoxOnnxOfflineZipformerCtcModelConfig zipformer_ctc;
  /** Canary configuration. */
  EdgevoxOnnxOfflineCanaryModelConfig canary;
  /** WeNet CTC configuration. */
  EdgevoxOnnxOfflineWenetCtcModelConfig wenet_ctc;
  /** Omnilingual CTC configuration. */
  EdgevoxOnnxOfflineOmnilingualAsrCtcModelConfig omnilingual;
  /** MedASR configuration. */
  EdgevoxOnnxOfflineMedAsrCtcModelConfig medasr;
  /** FunASR Nano configuration. */
  EdgevoxOnnxOfflineFunASRNanoModelConfig funasr_nano;
  /** FireRedAsr CTC configuration. */
  EdgevoxOnnxOfflineFireRedAsrCtcModelConfig fire_red_asr_ctc;
  /** Qwen3-ASR configuration. */
  EdgevoxOnnxOfflineQwen3ASRModelConfig qwen3_asr;
  /** Cohere Transcribe configuration. */
  EdgevoxOnnxOfflineCohereTranscribeModelConfig cohere_transcribe;
} EdgevoxOnnxOfflineModelConfig;

/**
 * @brief Configuration for a non-streaming ASR recognizer.
 *
 * Zero-initialize this struct before use.
 *
 * Example using Whisper:
 *
 * @code
 * EdgevoxOnnxOfflineRecognizerConfig config;
 * memset(&config, 0, sizeof(config));
 *
 * config.feat_config.sample_rate = 16000;
 * config.feat_config.feature_dim = 80;
 *
 * config.model_config.whisper.encoder =
 *     "./sherpa-onnx-whisper-tiny/tiny-encoder.onnx";
 * config.model_config.whisper.decoder =
 *     "./sherpa-onnx-whisper-tiny/tiny-decoder.onnx";
 * config.model_config.whisper.language = "en";
 * config.model_config.whisper.task = "transcribe";
 * config.model_config.tokens =
 *     "./sherpa-onnx-whisper-tiny/tiny-tokens.txt";
 * config.model_config.provider = "cpu";
 * config.model_config.num_threads = 1;
 *
 * config.decoding_method = "greedy_search";
 * @endcode
 *
 * Example using SenseVoice:
 *
 * @code
 * config.model_config.sense_voice.model =
 *     "./sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17-int8/model.int8.onnx";
 * config.model_config.sense_voice.language = "auto";
 * config.model_config.sense_voice.use_itn = 1;
 * config.model_config.tokens =
 *     "./sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17-int8/tokens.txt";
 * @endcode
 *
 * Example using Parakeet TDT:
 *
 * @code
 * config.model_config.transducer.encoder =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/encoder.int8.onnx";
 * config.model_config.transducer.decoder =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/decoder.int8.onnx";
 * config.model_config.transducer.joiner =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/joiner.int8.onnx";
 * config.model_config.tokens =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/tokens.txt";
 * config.model_config.model_type = "nemo_transducer";
 * @endcode
 * @see EdgevoxOnnxCreateOfflineRecognizer
 */
typedef struct EdgevoxOnnxOfflineRecognizerConfig {
  /** Feature extraction settings. */
  EdgevoxOnnxFeatureConfig feat_config;
  /** Offline model configuration. */
  EdgevoxOnnxOfflineModelConfig model_config;
  /** Optional language model configuration. */
  EdgevoxOnnxOfflineLMConfig lm_config;

  /** Decoding method, for example "greedy_search" or "modified_beam_search". */
  const char *decoding_method;
  /** Number of active paths for modified beam search. */
  int32_t max_active_paths;

  /** Path to a hotwords file. */
  const char *hotwords_file;

  /** Bonus score added to each hotword token. */
  float hotwords_score;
  /** Path to punctuation or text-processing rule FSTs. */
  const char *rule_fsts;
  /** Path to FAR archives used by text-processing rules. */
  const char *rule_fars;
  /** Optional blank penalty applied during decoding. */
  float blank_penalty;

  /** Optional homophone replacement configuration. */
  EdgevoxOnnxHomophoneReplacerConfig hr;
} EdgevoxOnnxOfflineRecognizerConfig;

/** @brief Non-streaming recognizer handle. */
typedef struct EdgevoxOnnxOfflineRecognizer EdgevoxOnnxOfflineRecognizer;

/** @brief Non-streaming decoding state for one utterance. */
typedef struct EdgevoxOnnxOfflineStream EdgevoxOnnxOfflineStream;

/**
 * @brief Create a non-streaming ASR recognizer.
 *
 * @param config Recognizer configuration.
 * @return A recognizer handle on success, or NULL if the configuration is
 *         invalid. The caller owns the returned object and must free it with
 *         EdgevoxOnnxDestroyOfflineRecognizer().
 *
 * Whisper example:
 *
 * @code
 * EdgevoxOnnxOfflineRecognizerConfig config;
 * memset(&config, 0, sizeof(config));
 * config.feat_config.sample_rate = 16000;
 * config.feat_config.feature_dim = 80;
 * config.model_config.whisper.encoder =
 *     "./sherpa-onnx-whisper-tiny/tiny-encoder.onnx";
 * config.model_config.whisper.decoder =
 *     "./sherpa-onnx-whisper-tiny/tiny-decoder.onnx";
 * config.model_config.whisper.language = "en";
 * config.model_config.whisper.task = "transcribe";
 * config.model_config.tokens =
 *     "./sherpa-onnx-whisper-tiny/tiny-tokens.txt";
 * config.model_config.provider = "cpu";
 * config.model_config.num_threads = 1;
 * config.decoding_method = "greedy_search";
 *
 * const EdgevoxOnnxOfflineRecognizer *recognizer =
 *     EdgevoxOnnxCreateOfflineRecognizer(&config);
 * @endcode
 *
 * SenseVoice example:
 *
 * @code
 * config.model_config.sense_voice.model =
 *     "./sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17-int8/model.int8.onnx";
 * config.model_config.sense_voice.language = "auto";
 * config.model_config.sense_voice.use_itn = 1;
 * config.model_config.tokens =
 *     "./sherpa-onnx-sense-voice-zh-en-ja-ko-yue-2024-07-17-int8/tokens.txt";
 * @endcode
 *
 * Parakeet TDT example:
 *
 * @code
 * config.model_config.transducer.encoder =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/encoder.int8.onnx";
 * config.model_config.transducer.decoder =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/decoder.int8.onnx";
 * config.model_config.transducer.joiner =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/joiner.int8.onnx";
 * config.model_config.tokens =
 *     "./sherpa-onnx-nemo-parakeet-tdt-0.6b-v3-int8/tokens.txt";
 * config.model_config.model_type = "nemo_transducer";
 * @endcode
 * @see EdgevoxOnnxOfflineRecognizerConfig, EdgevoxOnnxDestroyOfflineRecognizer
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineRecognizer *
EdgevoxOnnxCreateOfflineRecognizer(
    const EdgevoxOnnxOfflineRecognizerConfig *config);

/**
 * @brief Update the configuration of an existing offline recognizer.
 *
 * @param recognizer Recognizer handle.
 * @param config New recognizer configuration.
 *
 * @code
 * EdgevoxOnnxOfflineRecognizerSetConfig(recognizer, &config);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOfflineRecognizerSetConfig(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineRecognizerConfig *config);

/**
 * @brief Destroy a non-streaming recognizer.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOfflineRecognizer().
 *
 * @code
 * EdgevoxOnnxDestroyOfflineRecognizer(recognizer);
 * recognizer = NULL;
 * @endcode
 * @see EdgevoxOnnxCreateOfflineRecognizer
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineRecognizer(
    const EdgevoxOnnxOfflineRecognizer *recognizer);

/**
 * @brief Create a non-streaming ASR input stream.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOfflineRecognizer().
 * @return A newly created stream. The caller owns the returned object and must
 *         free it with EdgevoxOnnxDestroyOfflineStream().
 *
 * @code
 * const EdgevoxOnnxWave *wave =
 *     EdgevoxOnnxReadWave("./sherpa-onnx-whisper-tiny.en/test_wavs/0.wav");
 * const EdgevoxOnnxOfflineStream *stream =
 *     EdgevoxOnnxCreateOfflineStream(recognizer);
 * @endcode
 * @see EdgevoxOnnxDestroyOfflineStream, EdgevoxOnnxAcceptWaveformOffline
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineStream *EdgevoxOnnxCreateOfflineStream(
    const EdgevoxOnnxOfflineRecognizer *recognizer);

/**
 * @brief Create a non-streaming ASR input stream with per-stream hotwords.
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOfflineRecognizer().
 * @param hotwords Hotwords text to associate with the stream.
 * @return A newly created stream. The caller owns the returned object and must
 *         free it with EdgevoxOnnxDestroyOfflineStream().
 *
 * @code
 * const EdgevoxOnnxOfflineStream *stream =
 *     EdgevoxOnnxCreateOfflineStreamWithHotwords(recognizer,
 *                                               "▁HELLO ▁WORLD");
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineStream *
EdgevoxOnnxCreateOfflineStreamWithHotwords(
    const EdgevoxOnnxOfflineRecognizer *recognizer, const char *hotwords);

/**
 * @brief Destroy a non-streaming ASR stream.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream() or
 *               EdgevoxOnnxCreateOfflineStreamWithHotwords().
 *
 * @code
 * EdgevoxOnnxDestroyOfflineStream(stream);
 * stream = NULL;
 * @endcode
 * @see EdgevoxOnnxCreateOfflineStream
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineStream(
    const EdgevoxOnnxOfflineStream *stream);

/**
 * @brief Provide the full utterance to an offline ASR stream.
 *
 * The input is mono floating-point PCM normalized to the range [-1, 1].
 * If @p sample_rate differs from the recognizer feature sample rate,
 * edgevox-onnx resamples internally.
 *
 * @warning Call this function at most once for each offline stream. Offline
 * recognition expects the entire utterance in a single call.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @param sample_rate Sample rate of @p samples.
 * @param samples Pointer to @p n samples in the range [-1, 1].
 * @param n Number of samples.
 *
 * @code
 * const EdgevoxOnnxWave *wave =
 *     EdgevoxOnnxReadWave("./sherpa-onnx-whisper-tiny.en/test_wavs/0.wav");
 * const EdgevoxOnnxOfflineStream *stream =
 *     EdgevoxOnnxCreateOfflineStream(recognizer);
 * EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate,
 *                                 wave->samples, wave->num_samples);
 * EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
 * @endcode
 * @see EdgevoxOnnxCreateOfflineStream, EdgevoxOnnxDecodeOfflineStream
 */
EDGEVOX_ONNX_API void EdgevoxOnnxAcceptWaveformOffline(
    const EdgevoxOnnxOfflineStream *stream, int32_t sample_rate,
    const float *samples, int32_t n);

/**
 * @brief Set a per-stream runtime option for offline ASR.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @param key Option name.
 * @param value Option value represented as text.
 *
 * @code
 * EdgevoxOnnxOfflineStreamSetOption(stream, "language", "en");
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOfflineStreamSetOption(
    const EdgevoxOnnxOfflineStream *stream, const char *key, const char *value);

/**
 * @brief Get a per-stream runtime option for offline ASR.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @param key Option name.
 * @return The option value. The returned pointer is owned by the stream, must
 *         not be freed by the caller, and may be invalidated if the option is
 *         overwritten or the stream is destroyed.
 *
 * @code
 * const char *value = EdgevoxOnnxOfflineStreamGetOption(stream, "language");
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxOfflineStreamGetOption(
    const EdgevoxOnnxOfflineStream *stream, const char *key);

/**
 * @brief Check whether a per-stream runtime option exists.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @param key Option name.
 * @return 1 if the option exists; otherwise 0.
 *
 * @code
 * int32_t has_language =
 *     EdgevoxOnnxOfflineStreamHasOption(stream, "language");
 * @endcode
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineStreamHasOption(
    const EdgevoxOnnxOfflineStream *stream, const char *key);

/**
 * @brief Run offline ASR on one stream.
 *
 * Call this after EdgevoxOnnxAcceptWaveformOffline().
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOfflineRecognizer().
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 *
 * @code
 * EdgevoxOnnxDecodeOfflineStream(recognizer, stream);
 * @endcode
 * @see EdgevoxOnnxAcceptWaveformOffline, EdgevoxOnnxGetOfflineStreamResult
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeOfflineStream(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineStream *stream);

/**
 * @brief Run offline ASR on multiple streams in parallel.
 *
 * The caller must have already provided one utterance to each stream via
 * EdgevoxOnnxAcceptWaveformOffline().
 *
 * @param recognizer A pointer returned by EdgevoxOnnxCreateOfflineRecognizer().
 * @param streams Array of @p n offline stream pointers.
 * @param n Number of streams in @p streams.
 *
 * @code
 * const EdgevoxOnnxOfflineStream *streams[2] = {stream1, stream2};
 * EdgevoxOnnxDecodeMultipleOfflineStreams(recognizer, streams, 2);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeMultipleOfflineStreams(
    const EdgevoxOnnxOfflineRecognizer *recognizer,
    const EdgevoxOnnxOfflineStream **streams, int32_t n);

/**
 * @brief Recognition result for a non-streaming ASR stream.
 *
 * All pointers in this struct are owned by the result object returned from
 * EdgevoxOnnxGetOfflineStreamResult() and become invalid after
 * EdgevoxOnnxDestroyOfflineRecognizerResult() is called.
 * @see EdgevoxOnnxGetOfflineStreamResult, EdgevoxOnnxDestroyOfflineRecognizerResult
 */
typedef struct EdgevoxOnnxOfflineRecognizerResult {
  /** Recognized text. */
  const char *text;

  /**
   * Optional token timestamps in seconds.
   *
   * This field may be NULL when the model does not provide token timestamps.
   * When non-NULL, it contains @c count entries and is parallel to
   * @c tokens_arr.
   */
  float *timestamps;

  /** Number of token entries in @c tokens_arr and related per-token arrays. */
  int32_t count;

  /**
   * Contiguous memory block containing token strings separated by '\0'.
   *
   * Use @c tokens_arr for convenient indexed access.
   */
  const char *tokens;

  /** Array of @c count pointers into @c tokens. */
  const char *const *tokens_arr;

  /** JSON serialization of the result. */
  const char *json;

  /** Optional recognized language label. */
  const char *lang;

  /** Optional recognized emotion label. */
  const char *emotion;

  /** Optional recognized event label. */
  const char *event;

  /** Optional token durations in seconds, parallel to @c tokens_arr. */
  float *durations;

  /** Optional token log probabilities, parallel to @c tokens_arr. */
  float *ys_log_probs;

  /** Optional segment start times in seconds, parallel to @c segment_texts_arr.
   */
  const float *segment_timestamps;

  /** Optional segment durations in seconds, parallel to @c segment_texts_arr.
   */
  const float *segment_durations;

  /** Contiguous memory block containing segment texts separated by '\0'. */
  const char *segment_texts;

  /** Array of @c segment_count pointers into @c segment_texts. */
  const char *const *segment_texts_arr;

  /** Number of segment entries in the segment-level arrays. */
  int32_t segment_count;
} EdgevoxOnnxOfflineRecognizerResult;

/**
 * @brief Get the recognition result for an offline ASR stream.
 *
 * Call this after EdgevoxOnnxDecodeOfflineStream() or
 * EdgevoxOnnxDecodeMultipleOfflineStreams().
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @return A newly allocated result snapshot. Free it with
 *         EdgevoxOnnxDestroyOfflineRecognizerResult().
 *
 * @code
 * const EdgevoxOnnxOfflineRecognizerResult *r =
 *     EdgevoxOnnxGetOfflineStreamResult(stream);
 * printf("%s\n", r->text);
 * if (r->timestamps) {
 *   printf("First token starts at %.3f seconds\n", r->timestamps[0]);
 * }
 * EdgevoxOnnxDestroyOfflineRecognizerResult(r);
 * @endcode
 * @see EdgevoxOnnxDestroyOfflineRecognizerResult, EdgevoxOnnxDecodeOfflineStream
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineRecognizerResult *
EdgevoxOnnxGetOfflineStreamResult(const EdgevoxOnnxOfflineStream *stream);

/**
 * @brief Destroy a result returned by EdgevoxOnnxGetOfflineStreamResult().
 *
 * @param r A pointer returned by EdgevoxOnnxGetOfflineStreamResult().
 *
 * @code
 * EdgevoxOnnxDestroyOfflineRecognizerResult(r);
 * r = NULL;
 * @endcode
 * @see EdgevoxOnnxGetOfflineStreamResult
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineRecognizerResult(
    const EdgevoxOnnxOfflineRecognizerResult *r);

/**
 * @brief Get the offline ASR result as JSON.
 *
 * @param stream A pointer returned by EdgevoxOnnxCreateOfflineStream().
 * @return A newly allocated JSON string. Free it with
 *         EdgevoxOnnxDestroyOfflineStreamResultJson().
 *
 * @code
 * const char *json = EdgevoxOnnxGetOfflineStreamResultAsJson(stream);
 * puts(json);
 * EdgevoxOnnxDestroyOfflineStreamResultJson(json);
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetOfflineStreamResultAsJson(
    const EdgevoxOnnxOfflineStream *stream);

/**
 * @brief Free a JSON string returned by
 * EdgevoxOnnxGetOfflineStreamResultAsJson().
 *
 * @param s A pointer returned by EdgevoxOnnxGetOfflineStreamResultAsJson().
 *
 * @code
 * EdgevoxOnnxDestroyOfflineStreamResultJson(json);
 * json = NULL;
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineStreamResultJson(const char *s);

// ============================================================
// For keyword spotting
// ============================================================
/**
 * @brief Snapshot of the current keyword spotting result.
 *
 * Free this object with EdgevoxOnnxDestroyKeywordResult().
 * @see EdgevoxOnnxGetKeywordResult
 */
typedef struct EdgevoxOnnxKeywordResult {
  /**
   * Triggered keyword text.
   *
   * For English models this is usually space-separated words. For Chinese
   * models it is typically the surface form without spaces.
   */
  const char *keyword;

  /**
   * Token sequence as a single string.
   *
   * For BPE-based models this contains the decoded BPE tokens.
   */
  const char *tokens;

  /**
   * Token sequence as an array.
   *
   * The array length is @c count. Each string is owned by this result object.
   */
  const char *const *tokens_arr;

  /** Number of decoded tokens in @c tokens_arr and @c timestamps. */
  int32_t count;

  /**
   * Per-token timestamps in seconds.
   *
   * This array has @c count elements. Element @c i corresponds to
   * `tokens_arr[i]`.
   */
  float *timestamps;

  /** Start time of the current segment in seconds. */
  float start_time;

  /**
   * JSON representation of the result.
   *
   * The JSON includes `keyword`, `tokens`, `timestamps`, and `start_time`.
   */
  const char *json;
} EdgevoxOnnxKeywordResult;

/**
 * @brief Configuration for keyword spotting.
 *
 * The acoustic model is configured through @c model_config. In practice this is
 * usually a streaming transducer model.
 *
 * Keyword definitions can be provided either through @c keywords_file or
 * through @c keywords_buf/@c keywords_buf_size. If both are set, the buffer is
 * used.
 *
 * Example using
 * `sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile`:
 *
 * @code
 * EdgevoxOnnxKeywordSpotterConfig config;
 * memset(&config, 0, sizeof(config));
 *
 * config.model_config.transducer.encoder =
 *     "./sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
 *     "encoder-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
 * config.model_config.transducer.decoder =
 *     "./sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
 *     "decoder-epoch-12-avg-2-chunk-16-left-64.onnx";
 * config.model_config.transducer.joiner =
 *     "./sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
 *     "joiner-epoch-12-avg-2-chunk-16-left-64.int8.onnx";
 * config.model_config.tokens =
 *     "./sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
 *     "tokens.txt";
 * config.model_config.provider = "cpu";
 * config.model_config.num_threads = 1;
 *
 * config.keywords_file =
 *     "./sherpa-onnx-kws-zipformer-wenetspeech-3.3M-2024-01-01-mobile/"
 *     "test_wavs/test_keywords.txt";
 * config.max_active_paths = 4;
 * config.keywords_score = 3.0f;
 * config.keywords_threshold = 0.1f;
 * @endcode
 * @see EdgevoxOnnxCreateKeywordSpotter
 */
typedef struct EdgevoxOnnxKeywordSpotterConfig {
  /** Feature extraction parameters. */
  EdgevoxOnnxFeatureConfig feat_config;
  /** Streaming acoustic model configuration. */
  EdgevoxOnnxOnlineModelConfig model_config;
  /** Maximum number of active decoding paths. */
  int32_t max_active_paths;
  /** Number of trailing blank symbols required before trigger finalization. */
  int32_t num_trailing_blanks;
  /** Bonus score applied to keywords during search. */
  float keywords_score;
  /** Detection threshold. Larger values are more conservative. */
  float keywords_threshold;
  /** Optional keyword file. */
  const char *keywords_file;
  /** Optional in-memory keyword data. If non-null, it overrides @c
   * keywords_file. */
  const char *keywords_buf;
  /** Size in bytes of @c keywords_buf, excluding any trailing `'\0'`. */
  int32_t keywords_buf_size;
} EdgevoxOnnxKeywordSpotterConfig;

/** @brief Opaque keyword spotter handle. */
typedef struct EdgevoxOnnxKeywordSpotter EdgevoxOnnxKeywordSpotter;

/**
 * @brief Create a keyword spotter.
 *
 * @param config Keyword spotter configuration.
 * @return A newly allocated keyword spotter on success, or NULL on error. Free
 *         it with EdgevoxOnnxDestroyKeywordSpotter().
 * @see EdgevoxOnnxKeywordSpotterConfig, EdgevoxOnnxDestroyKeywordSpotter
 */
EDGEVOX_ONNX_API const EdgevoxOnnxKeywordSpotter *EdgevoxOnnxCreateKeywordSpotter(
    const EdgevoxOnnxKeywordSpotterConfig *config);

/**
 * @brief Destroy a keyword spotter.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @see EdgevoxOnnxCreateKeywordSpotter
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyKeywordSpotter(
    const EdgevoxOnnxKeywordSpotter *spotter);

/**
 * @brief Create a keyword spotting stream using the spotter's built-in keyword
 * list.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @return A newly allocated stream. Free it with
 * EdgevoxOnnxDestroyOnlineStream().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineStream *EdgevoxOnnxCreateKeywordStream(
    const EdgevoxOnnxKeywordSpotter *spotter);

/**
 * @brief Create a keyword spotting stream with extra or replacement keywords.
 *
 * The @p keywords string uses the same textual format as the keyword files used
 * by the examples. For instance:
 *
 * @code
 * const EdgevoxOnnxOnlineStream *stream =
 *     EdgevoxOnnxCreateKeywordStreamWithKeywords(
 *         kws, "y ǎn y uán @演员/zh ī m íng @知名");
 * @endcode
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param keywords Inline keyword definition string.
 * @return A newly allocated stream. Free it with
 * EdgevoxOnnxDestroyOnlineStream().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineStream *
EdgevoxOnnxCreateKeywordStreamWithKeywords(
    const EdgevoxOnnxKeywordSpotter *spotter, const char *keywords);

/**
 * @brief Check whether a keyword stream has enough audio for decoding.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param stream A pointer returned by EdgevoxOnnxCreateKeywordStream() or
 *               EdgevoxOnnxCreateKeywordStreamWithKeywords().
 * @return 1 if the stream is ready to decode; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxIsKeywordStreamReady(const EdgevoxOnnxKeywordSpotter *spotter,
                               const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Decode one ready keyword stream.
 *
 * Call this only when EdgevoxOnnxIsKeywordStreamReady() returns 1.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param stream A pointer returned by EdgevoxOnnxCreateKeywordStream() or
 *               EdgevoxOnnxCreateKeywordStreamWithKeywords().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeKeywordStream(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Reset a keyword stream after a keyword is detected.
 *
 * The examples call this immediately after a successful trigger so the next
 * keyword can be detected independently.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param stream A pointer returned by EdgevoxOnnxCreateKeywordStream() or
 *               EdgevoxOnnxCreateKeywordStreamWithKeywords().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxResetKeywordStream(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Decode multiple ready keyword streams in parallel.
 *
 * The caller must ensure every stream in @p streams is ready before calling
 * this function.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param streams Array of ready streams.
 * @param n Number of elements in @p streams.
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDecodeMultipleKeywordStreams(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream **streams, int32_t n);

/**
 * @brief Get the current keyword spotting result for a stream.
 *
 * The returned snapshot may represent either "no trigger yet" or a detected
 * keyword. A common pattern is to check whether `strlen(r->keyword) != 0`.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param stream A pointer returned by EdgevoxOnnxCreateKeywordStream() or
 *               EdgevoxOnnxCreateKeywordStreamWithKeywords().
 * @return A newly allocated result snapshot. Free it with
 *         EdgevoxOnnxDestroyKeywordResult().
 *
 * @code
 * const EdgevoxOnnxKeywordResult *r = EdgevoxOnnxGetKeywordResult(kws, stream);
 * if (r && r->json && strlen(r->keyword)) {
 *   fprintf(stderr, "Detected keyword: %s\n", r->json);
 *   EdgevoxOnnxResetKeywordStream(kws, stream);
 * }
 * EdgevoxOnnxDestroyKeywordResult(r);
 * @endcode
 * @see EdgevoxOnnxDestroyKeywordResult
 */
EDGEVOX_ONNX_API const EdgevoxOnnxKeywordResult *EdgevoxOnnxGetKeywordResult(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Destroy a keyword result snapshot.
 *
 * @param r A pointer returned by EdgevoxOnnxGetKeywordResult().
 * @see EdgevoxOnnxGetKeywordResult
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyKeywordResult(
    const EdgevoxOnnxKeywordResult *r);

/**
 * @brief Get the current keyword spotting result as JSON.
 *
 * @param spotter A pointer returned by EdgevoxOnnxCreateKeywordSpotter().
 * @param stream A pointer returned by EdgevoxOnnxCreateKeywordStream() or
 *               EdgevoxOnnxCreateKeywordStreamWithKeywords().
 * @return A newly allocated JSON string. Free it with
 *         EdgevoxOnnxFreeKeywordResultJson().
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxGetKeywordResultAsJson(
    const EdgevoxOnnxKeywordSpotter *spotter,
    const EdgevoxOnnxOnlineStream *stream);

/**
 * @brief Free a JSON string returned by EdgevoxOnnxGetKeywordResultAsJson().
 *
 * @param s A pointer returned by EdgevoxOnnxGetKeywordResultAsJson().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxFreeKeywordResultJson(const char *s);

// ============================================================
// For VAD
// ============================================================

/** @brief Configuration for a Silero VAD model. */
typedef struct EdgevoxOnnxSileroVadModelConfig {
  /** Path to `silero_vad.onnx`. */
  const char *model;
  /** Speech probability threshold. Frames above this value are speech. */
  float threshold;
  /** Minimum silence duration in seconds used to close a speech segment. */
  float min_silence_duration;
  /** Minimum speech duration in seconds to keep a detected segment. */
  float min_speech_duration;
  /** Input window size in samples. A common value is 512. */
  int32_t window_size;
  /**
   * Maximum speech duration in seconds.
   *
   * When a segment exceeds this value, the detector temporarily uses a higher
   * threshold to encourage a split.
   */
  float max_speech_duration;
} EdgevoxOnnxSileroVadModelConfig;

/** @brief Configuration for a Ten VAD model. */
typedef struct EdgevoxOnnxTenVadModelConfig {
  /** Path to `ten-vad.onnx`. */
  const char *model;
  /** Speech probability threshold. Frames above this value are speech. */
  float threshold;
  /** Minimum silence duration in seconds used to close a speech segment. */
  float min_silence_duration;
  /** Minimum speech duration in seconds to keep a detected segment. */
  float min_speech_duration;
  /** Input window size in samples. A common value is 256. */
  int32_t window_size;
  /**
   * Maximum speech duration in seconds.
   *
   * When a segment exceeds this value, the detector temporarily uses a higher
   * threshold to encourage a split.
   */
  float max_speech_duration;
} EdgevoxOnnxTenVadModelConfig;

/**
 * @brief Configuration shared by voice activity detectors.
 *
 * Exactly one VAD model family should be configured. Set either
 * @c silero_vad.model or @c ten_vad.model.
 *
 * If both are configured, the implementation will choose one of them, and
 * which one is used is implementation-defined. Do not rely on any precedence
 * rule.
 *
 * Example model files:
 * - `./silero_vad.onnx`
 * - `./ten-vad.onnx`
 *
 * @code
 * EdgevoxOnnxVadModelConfig config;
 * memset(&config, 0, sizeof(config));
 *
 * config.silero_vad.model = "./silero_vad.onnx";
 * config.silero_vad.threshold = 0.25f;
 * config.silero_vad.min_silence_duration = 0.5f;
 * config.silero_vad.min_speech_duration = 0.5f;
 * config.silero_vad.max_speech_duration = 10.0f;
 * config.silero_vad.window_size = 512;
 *
 * config.sample_rate = 16000;
 * config.num_threads = 1;
 * config.provider = "cpu";
 * config.debug = 0;
 * @endcode
 * @see EdgevoxOnnxCreateVoiceActivityDetector
 */
typedef struct EdgevoxOnnxVadModelConfig {
  /** Silero VAD configuration. */
  EdgevoxOnnxSileroVadModelConfig silero_vad;
  /** Input sample rate expected by the detector, usually 16000. */
  int32_t sample_rate;
  /** Number of backend threads. */
  int32_t num_threads;
  /** Execution provider, for example "cpu" or "cuda". */
  const char *provider;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Ten VAD configuration. */
  EdgevoxOnnxTenVadModelConfig ten_vad;
} EdgevoxOnnxVadModelConfig;

/** @brief Opaque circular-buffer handle used by helper APIs. */
typedef struct EdgevoxOnnxCircularBuffer EdgevoxOnnxCircularBuffer;

/**
 * @brief Create a floating-point circular buffer.
 *
 * @param capacity Maximum number of samples the buffer can keep.
 * @return A newly allocated buffer. Free it with
 *         EdgevoxOnnxDestroyCircularBuffer().
 *
 * @code
 * const EdgevoxOnnxCircularBuffer *buffer =
 *     EdgevoxOnnxCreateCircularBuffer(16000 * 30);
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxCircularBuffer *EdgevoxOnnxCreateCircularBuffer(
    int32_t capacity);

/**
 * @brief Destroy a circular buffer.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 *
 * @code
 * EdgevoxOnnxDestroyCircularBuffer(buffer);
 * buffer = NULL;
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyCircularBuffer(
    const EdgevoxOnnxCircularBuffer *buffer);

/**
 * @brief Append samples to a circular buffer.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 * @param p Pointer to @p n samples.
 * @param n Number of samples.
 *
 * @code
 * EdgevoxOnnxCircularBufferPush(buffer, wave->samples, wave->num_samples);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxCircularBufferPush(
    const EdgevoxOnnxCircularBuffer *buffer, const float *p, int32_t n);

/**
 * @brief Copy out a slice of samples from a circular buffer.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 * @param start_index Absolute start index in the buffer timeline.
 * @param n Number of samples to copy.
 * @return A newly allocated array containing @p n samples. Free it with
 *         EdgevoxOnnxCircularBufferFree().
 *
 * @code
 * const float *samples = EdgevoxOnnxCircularBufferGet(buffer, start, 3200);
 * EdgevoxOnnxCircularBufferFree(samples);
 * @endcode
 */
EDGEVOX_ONNX_API const float *EdgevoxOnnxCircularBufferGet(
    const EdgevoxOnnxCircularBuffer *buffer, int32_t start_index, int32_t n);

/** @brief Free an array returned by EdgevoxOnnxCircularBufferGet(). */
EDGEVOX_ONNX_API void EdgevoxOnnxCircularBufferFree(const float *p);

/**
 * @brief Drop samples from the front of a circular buffer.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 * @param n Number of samples to remove.
 */
EDGEVOX_ONNX_API void EdgevoxOnnxCircularBufferPop(
    const EdgevoxOnnxCircularBuffer *buffer, int32_t n);

/**
 * @brief Return the number of currently stored samples.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 * @return Number of samples currently in the buffer.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxCircularBufferSize(const EdgevoxOnnxCircularBuffer *buffer);

/**
 * @brief Return the current head index of the buffer timeline.
 *
 * The value is monotonically non-decreasing until
 * EdgevoxOnnxCircularBufferReset() is called.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 * @return The current head index.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxCircularBufferHead(const EdgevoxOnnxCircularBuffer *buffer);

/**
 * @brief Clear a circular buffer and reset its head index.
 *
 * @param buffer A pointer returned by EdgevoxOnnxCreateCircularBuffer().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxCircularBufferReset(
    const EdgevoxOnnxCircularBuffer *buffer);

/**
 * @brief One detected speech segment returned by the VAD.
 *
 * The segment owns @c samples. Free the whole object with
 * EdgevoxOnnxDestroySpeechSegment().
 * @see EdgevoxOnnxVoiceActivityDetectorFront, EdgevoxOnnxDestroySpeechSegment
 */
typedef struct EdgevoxOnnxSpeechSegment {
  /** Start index, in input samples, of this segment. */
  int32_t start;
  /** Newly allocated mono samples for this segment. */
  float *samples;
  /** Number of samples in @c samples. */
  int32_t n;
} EdgevoxOnnxSpeechSegment;

/** @brief Opaque voice activity detector handle. */
typedef struct EdgevoxOnnxVoiceActivityDetector EdgevoxOnnxVoiceActivityDetector;

/**
 * @brief Create a voice activity detector.
 *
 * Example model files are shown in `c-api-examples/vad-whisper-c-api.c`.
 *
 * @param config VAD configuration.
 * @param buffer_size_in_seconds Internal buffering capacity in seconds.
 * @return A newly allocated detector on success, or NULL on configuration
 *         error. Free it with EdgevoxOnnxDestroyVoiceActivityDetector().
 *
 * @code
 * EdgevoxOnnxVadModelConfig config;
 * memset(&config, 0, sizeof(config));
 * config.silero_vad.model = "./silero_vad.onnx";
 * config.silero_vad.threshold = 0.25f;
 * config.silero_vad.min_silence_duration = 0.5f;
 * config.silero_vad.min_speech_duration = 0.5f;
 * config.silero_vad.max_speech_duration = 10.0f;
 * config.silero_vad.window_size = 512;
 * config.sample_rate = 16000;
 * config.num_threads = 1;
 *
 * const EdgevoxOnnxVoiceActivityDetector *vad =
 *     EdgevoxOnnxCreateVoiceActivityDetector(&config, 30.0f);
 * @endcode
 * @see EdgevoxOnnxVadModelConfig, EdgevoxOnnxDestroyVoiceActivityDetector
 */
EDGEVOX_ONNX_API const EdgevoxOnnxVoiceActivityDetector *
EdgevoxOnnxCreateVoiceActivityDetector(const EdgevoxOnnxVadModelConfig *config,
                                      float buffer_size_in_seconds);

/**
 * @brief Destroy a voice activity detector.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 * @see EdgevoxOnnxCreateVoiceActivityDetector
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyVoiceActivityDetector(
    const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Feed audio samples to the VAD.
 *
 * Input samples are mono floating-point PCM in the range [-1, 1].
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 * @param samples Pointer to @p n samples.
 * @param n Number of samples.
 *
 * @code
 * EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(vad,
 *                                               wave->samples + i,
 *                                               window_size);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxVoiceActivityDetectorAcceptWaveform(
    const EdgevoxOnnxVoiceActivityDetector *p, const float *samples, int32_t n);

/**
 * @brief Check whether the detector currently has any completed speech segment.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 * @return 1 if no completed speech segment is available; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxVoiceActivityDetectorEmpty(const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Check whether the detector is currently inside speech.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 * @return 1 if speech is currently detected; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxVoiceActivityDetectorDetected(
    const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Remove the front speech segment from the detector queue.
 *
 * Call this after consuming the segment returned by
 * EdgevoxOnnxVoiceActivityDetectorFront().
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 *
 * @code
 * const EdgevoxOnnxSpeechSegment *segment =
 *     EdgevoxOnnxVoiceActivityDetectorFront(vad);
 * // ... use segment ...
 * EdgevoxOnnxDestroySpeechSegment(segment);
 * EdgevoxOnnxVoiceActivityDetectorPop(vad);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxVoiceActivityDetectorPop(
    const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Remove all queued speech segments.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxVoiceActivityDetectorClear(
    const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Get the first queued speech segment.
 *
 * The returned segment is a copy owned by the caller. Free it with
 * EdgevoxOnnxDestroySpeechSegment().
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 * @return The first queued speech segment, or NULL if none is available.
 *
 * @code
 * while (!EdgevoxOnnxVoiceActivityDetectorEmpty(vad)) {
 *   const EdgevoxOnnxSpeechSegment *segment =
 *       EdgevoxOnnxVoiceActivityDetectorFront(vad);
 *   printf("start=%d, samples=%d\n", segment->start, segment->n);
 *   EdgevoxOnnxDestroySpeechSegment(segment);
 *   EdgevoxOnnxVoiceActivityDetectorPop(vad);
 * }
 * @endcode
 * @see EdgevoxOnnxSpeechSegment, EdgevoxOnnxDestroySpeechSegment
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpeechSegment *
EdgevoxOnnxVoiceActivityDetectorFront(const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Destroy a speech segment returned by
 * EdgevoxOnnxVoiceActivityDetectorFront().
 *
 * @param p A pointer returned by EdgevoxOnnxVoiceActivityDetectorFront().
 * @see EdgevoxOnnxVoiceActivityDetectorFront
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySpeechSegment(
    const EdgevoxOnnxSpeechSegment *p);

/**
 * @brief Reset a voice activity detector so it can process a new stream.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxVoiceActivityDetectorReset(
    const EdgevoxOnnxVoiceActivityDetector *p);

/**
 * @brief Flush buffered tail samples and force final segmentation.
 *
 * Call this after the last chunk of input has been fed.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateVoiceActivityDetector().
 *
 * @code
 * EdgevoxOnnxVoiceActivityDetectorFlush(vad);
 * @endcode
 */
EDGEVOX_ONNX_API void EdgevoxOnnxVoiceActivityDetectorFlush(
    const EdgevoxOnnxVoiceActivityDetector *p);

// ============================================================
// For offline Text-to-Speech (i.e., non-streaming TTS)
// ============================================================

/** @brief Configuration for a VITS TTS model. */
typedef struct EdgevoxOnnxOfflineTtsVitsModelConfig {
  /** Path to the VITS ONNX model, for example `./vits-ljs.onnx`. */
  const char *model;
  /** Path to the lexicon file. Ignored if @c data_dir is provided. */
  const char *lexicon;
  /** Path to the tokens file. */
  const char *tokens;
  /** Optional path to espeak-ng-data. */
  const char *data_dir;
  /** VITS noise scale. */
  float noise_scale;
  /** VITS duration noise scale. */
  float noise_scale_w;
  /** Speech rate scale. Values < 1 are slower; values > 1 are faster. */
  float length_scale;
  /** Unused legacy field kept for ABI compatibility. */
  const char *dict_dir;
  /** Optional OpenVoice V2 reference encoder ONNX model. */
  const char *openvoice_tone_encoder;
  /** Optional OpenVoice V2 tone converter ONNX model. */
  const char *openvoice_tone_converter;
} EdgevoxOnnxOfflineTtsVitsModelConfig;

/** @brief Configuration for a Matcha TTS model. */
typedef struct EdgevoxOnnxOfflineTtsMatchaModelConfig {
  /** Path to the Matcha acoustic model. */
  const char *acoustic_model;
  /** Path to the vocoder model, for example `./vocos-22khz-univ.onnx`. */
  const char *vocoder;
  /** Path to the lexicon file. */
  const char *lexicon;
  /** Path to the tokens file. */
  const char *tokens;
  /** Optional path to espeak-ng-data. */
  const char *data_dir;
  /** Matcha noise scale. */
  float noise_scale;
  /** Speech rate scale. Values < 1 are slower; values > 1 are faster. */
  float length_scale;
  /** Unused legacy field kept for ABI compatibility. */
  const char *dict_dir;
} EdgevoxOnnxOfflineTtsMatchaModelConfig;

/** @brief Configuration for a Kokoro TTS model. */
typedef struct EdgevoxOnnxOfflineTtsKokoroModelConfig {
  /** Path to the Kokoro model, for example `./kokoro-en-v0_19/model.onnx`. */
  const char *model;
  /** Path to the Kokoro voices file. */
  const char *voices;
  /** Path to the tokens file. */
  const char *tokens;
  /** Optional path to espeak-ng-data. */
  const char *data_dir;
  /** Speech rate scale. Values < 1 are slower; values > 1 are faster. */
  float length_scale;
  /** Unused legacy field kept for ABI compatibility. */
  const char *dict_dir;
  /** Optional lexicon file. */
  const char *lexicon;
  /** Optional language hint. */
  const char *lang;
} EdgevoxOnnxOfflineTtsKokoroModelConfig;

/** @brief Configuration for a Kitten TTS model. */
typedef struct EdgevoxOnnxOfflineTtsKittenModelConfig {
  /** Path to the Kitten model. */
  const char *model;
  /** Path to the Kitten voices file. */
  const char *voices;
  /** Path to the tokens file. */
  const char *tokens;
  /** Optional path to espeak-ng-data. */
  const char *data_dir;
  /** Speech rate scale. Values < 1 are slower; values > 1 are faster. */
  float length_scale;
} EdgevoxOnnxOfflineTtsKittenModelConfig;

/** @brief Configuration for a ZipVoice TTS model. */
typedef struct EdgevoxOnnxOfflineTtsZipvoiceModelConfig {
  /** Path to the tokens file. */
  const char *tokens;
  /** Path to the ZipVoice encoder model. */
  const char *encoder;
  /** Path to the ZipVoice decoder model. */
  const char *decoder;
  /** Path to the vocoder model. */
  const char *vocoder;
  /** Optional path to espeak-ng-data. */
  const char *data_dir;
  /** Path to the lexicon file. */
  const char *lexicon;
  /** eSpeak voice/language for OOV words (default: "en-us"). */
  const char *espeak_voice;
  /** Feature scaling factor. */
  float feat_scale;
  /** Time shift parameter. */
  float t_shift;
  /** Target RMS parameter. */
  float target_rms;
  /** Guidance scale parameter. */
  float guidance_scale;
} EdgevoxOnnxOfflineTtsZipvoiceModelConfig;

/** @brief Configuration for a Pocket TTS model. */
typedef struct EdgevoxOnnxOfflineTtsPocketModelConfig {
  /** Path to `lm_flow*.onnx`. */
  const char *lm_flow;
  /** Path to `lm_main*.onnx`. */
  const char *lm_main;
  /** Path to the Pocket encoder model. */
  const char *encoder;
  /** Path to the Pocket decoder model. */
  const char *decoder;
  /** Path to the text conditioner model. */
  const char *text_conditioner;
  /** Path to `vocab.json`. */
  const char *vocab_json;
  /** Path to `token_scores.json`. */
  const char *token_scores_json;
  /** Voice embedding cache capacity. */
  int32_t voice_embedding_cache_capacity;
} EdgevoxOnnxOfflineTtsPocketModelConfig;

/** @brief Configuration for a Supertonic TTS model. */
typedef struct EdgevoxOnnxOfflineTtsSupertonicModelConfig {
  /** Path to the duration predictor model. */
  const char *duration_predictor;
  /** Path to the text encoder model. */
  const char *text_encoder;
  /** Path to the vector estimator model. */
  const char *vector_estimator;
  /** Path to the vocoder model. */
  const char *vocoder;
  /** Path to `tts.json`. */
  const char *tts_json;
  /** Path to the unicode indexer file. */
  const char *unicode_indexer;
  /** Path to the voice style file. */
  const char *voice_style;
} EdgevoxOnnxOfflineTtsSupertonicModelConfig;

/** @brief Configuration for ResembleAI Chatterbox Turbo ONNX. */
typedef struct EdgevoxOnnxOfflineTtsChatterboxModelConfig {
  /** Path to speech_encoder*.onnx. */
  const char *speech_encoder;
  /** Path to embed_tokens*.onnx. */
  const char *embed_tokens;
  /** Path to language_model*.onnx. */
  const char *language_model;
  /** Path to conditional_decoder*.onnx. */
  const char *conditional_decoder;
  /** Directory containing tokenizer.json. */
  const char *tokenizer;
} EdgevoxOnnxOfflineTtsChatterboxModelConfig;

/**
 * @brief Configuration shared by offline TTS models.
 *
 * Exactly one TTS model family should be configured. For example, set only one
 * of @c vits, @c matcha, @c kokoro, @c kitten, @c zipvoice, @c pocket,
 * @c supertonic, or @c chatterbox.
 *
 * If multiple model families are configured at the same time, the
 * implementation will choose one of them, and which one is used is
 * implementation-defined. Do not rely on any precedence rule.
 *
 * Concrete example model packages in this repository include:
 * - `kokoro-en-v0_19`
 * - `sherpa-onnx-pocket-tts-int8-2026-01-26`
 * - `matcha-icefall-en_US-ljspeech`
 * - `sherpa-onnx-zipvoice-distill-int8-zh-en-emilia`
 */
typedef struct EdgevoxOnnxOfflineTtsModelConfig {
  /** VITS configuration. */
  EdgevoxOnnxOfflineTtsVitsModelConfig vits;
  /** Number of backend threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider, for example "cpu" or "cuda". */
  const char *provider;
  /** Matcha configuration. */
  EdgevoxOnnxOfflineTtsMatchaModelConfig matcha;
  /** Kokoro configuration. */
  EdgevoxOnnxOfflineTtsKokoroModelConfig kokoro;
  /** Kitten configuration. */
  EdgevoxOnnxOfflineTtsKittenModelConfig kitten;
  /** ZipVoice configuration. */
  EdgevoxOnnxOfflineTtsZipvoiceModelConfig zipvoice;
  /** Pocket configuration. */
  EdgevoxOnnxOfflineTtsPocketModelConfig pocket;
  /** Supertonic configuration. */
  EdgevoxOnnxOfflineTtsSupertonicModelConfig supertonic;
  /** Chatterbox Turbo configuration. */
  EdgevoxOnnxOfflineTtsChatterboxModelConfig chatterbox;
} EdgevoxOnnxOfflineTtsModelConfig;

/**
 * @brief Configuration for offline text-to-speech.
 *
 * @code
 * EdgevoxOnnxOfflineTtsConfig config;
 * memset(&config, 0, sizeof(config));
 *
 * config.model.kokoro.model = "./kokoro-en-v0_19/model.onnx";
 * config.model.kokoro.voices = "./kokoro-en-v0_19/voices.bin";
 * config.model.kokoro.tokens = "./kokoro-en-v0_19/tokens.txt";
 * config.model.kokoro.data_dir = "./kokoro-en-v0_19/espeak-ng-data";
 * config.model.num_threads = 2;
 * config.model.provider = "cpu";
 * config.model.debug = 0;
 * config.max_num_sentences = 2;
 * @endcode
 * @see EdgevoxOnnxCreateOfflineTts
 */
typedef struct EdgevoxOnnxOfflineTtsConfig {
  /** TTS model configuration. */
  EdgevoxOnnxOfflineTtsModelConfig model;
  /** Optional comma-separated rule FST list. */
  const char *rule_fsts;
  /** Maximum number of sentences processed per chunk. */
  int32_t max_num_sentences;
  /** Optional FAR archives used by text normalization rules. */
  const char *rule_fars;
  /** Default silence scale between sentences. */
  float silence_scale;
} EdgevoxOnnxOfflineTtsConfig;

/**
 * @brief Generated waveform returned by TTS APIs.
 *
 * The returned structure owns @c samples. Free the whole object with
 * EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 * @see EdgevoxOnnxOfflineTtsGenerateWithConfig, EdgevoxOnnxDestroyOfflineTtsGeneratedAudio
 */
typedef struct EdgevoxOnnxGeneratedAudio {
  /** Generated mono samples in the range [-1, 1]. */
  const float *samples;
  /** Number of samples in @c samples. */
  int32_t n;
  /** Output sample rate. */
  int32_t sample_rate;
} EdgevoxOnnxGeneratedAudio;

/**
 * @brief Callback invoked during incremental generation.
 *
 * Return 1 to continue generation. Return 0 to stop early.
 *
 * The @p samples pointer is only valid during the callback. Copy the samples if
 * you need to keep them after the callback returns.
 */
typedef int32_t (*EdgevoxOnnxGeneratedAudioCallback)(const float *samples,
                                                    int32_t n);

/**
 * @brief Same as EdgevoxOnnxGeneratedAudioCallback but with an extra user
 * pointer.
 */
typedef int32_t (*EdgevoxOnnxGeneratedAudioCallbackWithArg)(const float *samples,
                                                           int32_t n,
                                                           void *arg);

/**
 * @brief Progress callback invoked during incremental generation.
 *
 * @param samples Newly generated samples valid only during the callback.
 * @param n Number of samples in @p samples.
 * @param p Progress in the range [0, 1].
 * @return Return 1 to continue generation. Return 0 to stop early.
 */
typedef int32_t (*EdgevoxOnnxGeneratedAudioProgressCallback)(
    const float *samples, int32_t n, float p);

/**
 * @brief Same as EdgevoxOnnxGeneratedAudioProgressCallback but with an extra
 * user pointer.
 */
typedef int32_t (*EdgevoxOnnxGeneratedAudioProgressCallbackWithArg)(
    const float *samples, int32_t n, float p, void *arg);

/** @brief Opaque offline TTS handle. */
typedef struct EdgevoxOnnxOfflineTts EdgevoxOnnxOfflineTts;

/**
 * @brief Create an offline TTS engine.
 *
 * @param config TTS configuration.
 * @return A newly allocated TTS engine on success, or NULL on configuration
 *         error. Free it with EdgevoxOnnxDestroyOfflineTts().
 *
 * @code
 * EdgevoxOnnxOfflineTtsConfig config;
 * memset(&config, 0, sizeof(config));
 * config.model.kokoro.model = "./kokoro-en-v0_19/model.onnx";
 * config.model.kokoro.voices = "./kokoro-en-v0_19/voices.bin";
 * config.model.kokoro.tokens = "./kokoro-en-v0_19/tokens.txt";
 * config.model.kokoro.data_dir = "./kokoro-en-v0_19/espeak-ng-data";
 * config.model.num_threads = 2;
 *
 * const EdgevoxOnnxOfflineTts *tts = EdgevoxOnnxCreateOfflineTts(&config);
 * @endcode
 * @see EdgevoxOnnxOfflineTtsConfig, EdgevoxOnnxDestroyOfflineTts
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTts(
    const EdgevoxOnnxOfflineTtsConfig *config);

/** One immutable logical file supplied entirely from caller-owned memory. */
typedef struct EdgevoxOnnxMemoryResource {
  const char *name;
  const void *data;
  size_t size;
} EdgevoxOnnxMemoryResource;

/**
 * Create TTS from logical in-memory resources. Model config fields contain
 * resource names rather than filesystem paths. The engine copies resources
 * it needs and never creates plaintext files.
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineTts *
EdgevoxOnnxCreateOfflineTtsFromMemory(
    const EdgevoxOnnxOfflineTtsConfig *config,
    const EdgevoxOnnxMemoryResource *resources, size_t num_resources);

/**
 * @brief Destroy an offline TTS engine.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @see EdgevoxOnnxCreateOfflineTts
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineTts(
    const EdgevoxOnnxOfflineTts *tts);

/**
 * @brief Return the output sample rate of a TTS engine.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @return Output sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxOfflineTtsSampleRate(const EdgevoxOnnxOfflineTts *tts);

/**
 * @brief Return the number of available speaker IDs.
 *
 * Single-speaker models often return 1.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @return Number of speakers supported by the model.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxOfflineTtsNumSpeakers(const EdgevoxOnnxOfflineTts *tts);

/**
 * @brief Generate speech from text using the simple sid/speed interface.
 *
 * @deprecated Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param sid Speaker ID for multi-speaker models.
 * @param speed Speech rate. Values > 1 are faster.
 * @return Generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 *
 * @code
 * const EdgevoxOnnxGeneratedAudio *audio =
 *     EdgevoxOnnxOfflineTtsGenerate(tts, "Hello from edgevox-onnx!", 0, 1.0f);
 * EdgevoxOnnxWriteWave(audio->samples, audio->n, audio->sample_rate,
 *                     "./generated.wav");
 * EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(audio);
 * @endcode
 * @see EdgevoxOnnxDestroyOfflineTtsGeneratedAudio, EdgevoxOnnxGenerationConfig
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerate(
        const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
        float speed);

/**
 * @brief Generate speech and receive incremental audio chunks through a
 * callback.
 *
 * @deprecated Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 *
 * The callback receives newly generated samples. The sample pointer is valid
 * only for the duration of the callback.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param sid Speaker ID for multi-speaker models.
 * @param speed Speech rate. Values > 1 are faster.
 * @param callback Incremental callback. Return 0 to stop generation early.
 * @return Final generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallback(
        const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
        float speed, EdgevoxOnnxGeneratedAudioCallback callback);

/**
 * @brief Generate speech with a progress callback.
 *
 * @deprecated Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param sid Speaker ID for multi-speaker models.
 * @param speed Speech rate. Values > 1 are faster.
 * @param callback Progress callback. Return 0 to stop generation early.
 * @return Final generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 *
 * @code
 * int32_t Progress(const float *samples, int32_t n, float p) {
 *   fprintf(stderr, "Progress: %.2f%%\n", p * 100);
 *   return 1;
 * }
 *
 * const EdgevoxOnnxGeneratedAudio *audio =
 *     EdgevoxOnnxOfflineTtsGenerateWithProgressCallback(tts, text, 0, 1.0f,
 *                                                      Progress);
 * @endcode
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithProgressCallback(
        const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
        float speed, EdgevoxOnnxGeneratedAudioProgressCallback callback);

/**
 * @brief Generate speech with a progress callback that receives a user pointer.
 *
 * @deprecated Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param sid Speaker ID for multi-speaker models.
 * @param speed Speech rate. Values > 1 are faster.
 * @param callback Progress callback with user pointer. Return 0 to stop early.
 * @param arg User pointer forwarded to @p callback.
 * @return Final generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio
        *EdgevoxOnnxOfflineTtsGenerateWithProgressCallbackWithArg(
            const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
            float speed,
            EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback,
            void *arg);

/**
 * @brief Same as EdgevoxOnnxOfflineTtsGenerateWithCallback() but with a user
 * pointer.
 *
 * @deprecated Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param sid Speaker ID for multi-speaker models.
 * @param speed Speech rate. Values > 1 are faster.
 * @param callback Incremental callback with user pointer.
 * @param arg User pointer forwarded to @p callback.
 * @return Final generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithCallbackWithArg(
        const EdgevoxOnnxOfflineTts *tts, const char *text, int32_t sid,
        float speed, EdgevoxOnnxGeneratedAudioCallbackWithArg callback,
        void *arg);

/**
 * @brief Deprecated ZipVoice-specific generation API.
 *
 * Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead.
 */
EDGEVOX_ONNX_API EDGEVOX_ONNX_DEPRECATED(
    "Use EdgevoxOnnxOfflineTtsGenerateWithConfig() instead") const
    EdgevoxOnnxGeneratedAudio *EdgevoxOnnxOfflineTtsGenerateWithZipvoice(
        const EdgevoxOnnxOfflineTts *tts, const char *text,
        const char *prompt_text, const float *prompt_samples, int32_t n_prompt,
        int32_t prompt_sr, float speed, int32_t num_steps);

/**
 * @brief Generation-time parameters shared by advanced TTS APIs.
 *
 * This struct supports both simple multi-speaker synthesis and more advanced
 * zero-shot or reference-conditioned models.
 *
 * Example for Pocket TTS:
 *
 * @code
 * EdgevoxOnnxGenerationConfig cfg;
 * memset(&cfg, 0, sizeof(cfg));
 * cfg.speed = 1.0f;
 * cfg.reference_audio = wave->samples;
 * cfg.reference_audio_len = wave->num_samples;
 * cfg.reference_sample_rate = wave->sample_rate;
 * cfg.extra = "{\"max_reference_audio_len\": 10.0, \"seed\": 42}";
 * @endcode
 * @see EdgevoxOnnxOfflineTtsGenerateWithConfig
 */
typedef struct EdgevoxOnnxGenerationConfig {
  /** Silence scale between sentences. */
  float silence_scale;
  /** Speech rate. Used only by models that support it. */
  float speed;
  /** Speaker ID for multi-speaker models. */
  int32_t sid;
  /** Optional reference audio for zero-shot or voice-cloning models. */
  const float *reference_audio;
  /** Length of @c reference_audio in samples. */
  int32_t reference_audio_len;
  /** Sample rate of @c reference_audio. */
  int32_t reference_sample_rate;
  /** Optional reference text associated with @c reference_audio. */
  const char *reference_text;
  /** Optional number of flow-matching steps. */
  int32_t num_steps;
  /** Optional model-specific JSON string with extra key/value pairs. */
  const char *extra;
} EdgevoxOnnxGenerationConfig;

/**
 * @brief Generate speech using the advanced configuration interface.
 *
 * This is the preferred API for new integrations. It supports callback-based
 * progress reporting and model-specific options such as reference audio.
 *
 * @param tts A pointer returned by EdgevoxOnnxCreateOfflineTts().
 * @param text Input text.
 * @param config Generation-time configuration.
 * @param callback Optional progress callback with user pointer. Return 0 to
 *                 stop early.
 * @param arg User pointer forwarded to @p callback.
 * @return Generated audio, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTtsGeneratedAudio().
 *
 * @code
 * EdgevoxOnnxGenerationConfig cfg;
 * memset(&cfg, 0, sizeof(cfg));
 * cfg.sid = 0;
 * cfg.speed = 1.0f;
 * cfg.silence_scale = 0.2f;
 *
 * const EdgevoxOnnxGeneratedAudio *audio =
 *     EdgevoxOnnxOfflineTtsGenerateWithConfig(tts,
 *         "Today as always, men fall into two groups.",
 *         &cfg, NULL, NULL);
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxGeneratedAudio *
EdgevoxOnnxOfflineTtsGenerateWithConfig(
    const EdgevoxOnnxOfflineTts *tts, const char *text,
    const EdgevoxOnnxGenerationConfig *config,
    EdgevoxOnnxGeneratedAudioProgressCallbackWithArg callback, void *arg);

/**
 * @brief Destroy audio returned by a TTS generation API.
 *
 * @param p A pointer returned by one of the EdgevoxOnnxOfflineTtsGenerate*
 *          functions.
 * @see EdgevoxOnnxOfflineTtsGenerateWithConfig
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineTtsGeneratedAudio(
    const EdgevoxOnnxGeneratedAudio *p);

/**
 * @brief Write floating-point PCM to a mono 16-bit WAVE file.
 *
 * @param samples Pointer to @p n samples in the range [-1, 1].
 * @param n Number of samples.
 * @param sample_rate Sample rate in Hz.
 * @param filename Output filename.
 * @return 1 on success; 0 on failure.
 *
 * @code
 * EdgevoxOnnxWriteWave(audio->samples, audio->n, audio->sample_rate,
 *                     "./generated-kokoro-en.wav");
 * @endcode
 * @see EdgevoxOnnxReadWave
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxWriteWave(const float *samples, int32_t n,
                                            int32_t sample_rate,
                                            const char *filename);

/**
 * @brief Return the number of bytes needed for a mono 16-bit WAVE file.
 *
 * @param n_samples Number of PCM samples.
 * @return Required buffer size in bytes.
 */
EDGEVOX_ONNX_API int64_t EdgevoxOnnxWaveFileSize(int32_t n_samples);

/**
 * @brief Write a mono 16-bit WAVE file to a caller-provided buffer.
 *
 * Allocate at least EdgevoxOnnxWaveFileSize(@p n) bytes before calling.
 *
 * @param samples Pointer to @p n samples in the range [-1, 1].
 * @param n Number of samples.
 * @param sample_rate Sample rate in Hz.
 * @param buffer Output buffer.
 */
EDGEVOX_ONNX_API void EdgevoxOnnxWriteWaveToBuffer(const float *samples,
                                                 int32_t n, int32_t sample_rate,
                                                 char *buffer);

/**
 * @brief Write multi-channel audio to a WAVE file (16-bit PCM).
 *
 * @param samples       samples[c] is a pointer to channel c samples in [-1, 1].
 * @param n             Number of samples per channel.
 * @param sample_rate   Sample rate in Hz.
 * @param num_channels  Number of channels.
 * @param filename      Output filename.
 * @return 1 on success; 0 on failure.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxWriteWaveMultiChannel(
    const float *const *samples, int32_t n, int32_t sample_rate,
    int32_t num_channels, const char *filename);

/**
 * @brief Decoded mono WAVE file content.
 *
 * Free this object with EdgevoxOnnxFreeWave().
 * @see EdgevoxOnnxReadWave, EdgevoxOnnxFreeWave
 */
typedef struct EdgevoxOnnxWave {
  /** Samples normalized to the range [-1, 1]. */
  const float *samples;
  /** Sample rate in Hz. */
  int32_t sample_rate;
  /** Number of samples. */
  int32_t num_samples;
} EdgevoxOnnxWave;

/**
 * @brief Read a mono 16-bit PCM WAVE file.
 *
 * @param filename Input WAVE filename.
 * @return A newly allocated wave object, or NULL on error. Free it with
 *         EdgevoxOnnxFreeWave().
 *
 * @code
 * const EdgevoxOnnxWave *wave = EdgevoxOnnxReadWave("./Obama.wav");
 * if (wave) {
 *   printf("sample_rate=%d, num_samples=%d\n",
 *          wave->sample_rate, wave->num_samples);
 *   EdgevoxOnnxFreeWave(wave);
 * }
 * @endcode
 * @see EdgevoxOnnxFreeWave, EdgevoxOnnxWave
 */
EDGEVOX_ONNX_API const EdgevoxOnnxWave *EdgevoxOnnxReadWave(const char *filename);

/**
 * @brief Read a mono 16-bit PCM WAVE file from binary memory.
 *
 * @param data Pointer to the WAVE file bytes.
 * @param n Size of @p data in bytes.
 * @return A newly allocated wave object, or NULL on error. Free it with
 *         EdgevoxOnnxFreeWave().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxWave *EdgevoxOnnxReadWaveFromBinaryData(
    const char *data, int32_t n);

/**
 * @brief Destroy a wave object returned by EdgevoxOnnxReadWave() or
 * EdgevoxOnnxReadWaveFromBinaryData().
 * @see EdgevoxOnnxReadWave
 */
EDGEVOX_ONNX_API void EdgevoxOnnxFreeWave(const EdgevoxOnnxWave *wave);

/**
 * @brief Decoded multi-channel WAVE file content.
 *
 * Free this object with EdgevoxOnnxFreeMultiChannelWave().
 */
typedef struct EdgevoxOnnxMultiChannelWave {
  /** samples[c] points to channel c samples normalized to [-1, 1].
   * Note: The sample data for all channels are stored in a single contiguous
   * memory block, one channel after another.
   * */
  const float *const *samples;
  /** Number of channels. */
  int32_t num_channels;
  /** Number of samples per channel. */
  int32_t num_samples;
  /** Sample rate in Hz. */
  int32_t sample_rate;
} EdgevoxOnnxMultiChannelWave;

/**
 * @brief Read a multi-channel 16-bit PCM WAVE file.
 *
 * @param filename Input WAVE filename.
 * @return A newly allocated multi-channel wave object, or NULL on error.
 *         Free it with EdgevoxOnnxFreeMultiChannelWave().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxMultiChannelWave *
EdgevoxOnnxReadWaveMultiChannel(const char *filename);

/**
 * @brief Destroy a multi-channel wave object.
 *
 * @param wave A pointer returned by EdgevoxOnnxReadWaveMultiChannel().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxFreeMultiChannelWave(
    const EdgevoxOnnxMultiChannelWave *wave);

// ============================================================
// For spoken language identification
// ============================================================

/**
 * @brief Whisper-based model files for spoken language identification.
 *
 * Example:
 *
 * @code
 * EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig whisper;
 * memset(&whisper, 0, sizeof(whisper));
 * whisper.encoder = "./sherpa-onnx-whisper-tiny/tiny-encoder.int8.onnx";
 * whisper.decoder = "./sherpa-onnx-whisper-tiny/tiny-decoder.int8.onnx";
 * @endcode
 */
typedef struct EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig {
  /** Whisper encoder model. */
  const char *encoder;
  /** Whisper decoder model. */
  const char *decoder;
  /** Optional tail padding in samples appended internally before inference. */
  int32_t tail_paddings;
} EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig;

/**
 * @brief Configuration for spoken language identification.
 *
 * The current implementation uses Whisper-based models.
 *
 * Example using `sherpa-onnx-whisper-tiny`:
 *
 * @code
 * EdgevoxOnnxSpokenLanguageIdentificationConfig config;
 * memset(&config, 0, sizeof(config));
 * config.whisper.encoder = "./sherpa-onnx-whisper-tiny/tiny-encoder.int8.onnx";
 * config.whisper.decoder = "./sherpa-onnx-whisper-tiny/tiny-decoder.int8.onnx";
 * config.num_threads = 1;
 * config.provider = "cpu";
 * @endcode
 */
typedef struct EdgevoxOnnxSpokenLanguageIdentificationConfig {
  /** Whisper model configuration. */
  EdgevoxOnnxSpokenLanguageIdentificationWhisperConfig whisper;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxSpokenLanguageIdentificationConfig;

/** @brief Opaque spoken-language identification handle. */
typedef struct EdgevoxOnnxSpokenLanguageIdentification
    EdgevoxOnnxSpokenLanguageIdentification;

/**
 * @brief Create a spoken-language identifier.
 *
 * @param config Spoken-language identification configuration.
 * @return A newly allocated identifier on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroySpokenLanguageIdentification().
 * @see EdgevoxOnnxDestroySpokenLanguageIdentification
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpokenLanguageIdentification *
EdgevoxOnnxCreateSpokenLanguageIdentification(
    const EdgevoxOnnxSpokenLanguageIdentificationConfig *config);

/**
 * @brief Destroy a spoken-language identifier.
 *
 * @param slid A pointer returned by
 * EdgevoxOnnxCreateSpokenLanguageIdentification().
 * @see EdgevoxOnnxCreateSpokenLanguageIdentification
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySpokenLanguageIdentification(
    const EdgevoxOnnxSpokenLanguageIdentification *slid);

/**
 * @brief Create an offline stream for spoken-language identification.
 *
 * Feed audio to the returned stream with EdgevoxOnnxAcceptWaveformOffline(), and
 * then call EdgevoxOnnxSpokenLanguageIdentificationCompute().
 *
 * @param slid A pointer returned by
 * EdgevoxOnnxCreateSpokenLanguageIdentification().
 * @return A newly allocated offline stream. Free it with
 *         EdgevoxOnnxDestroyOfflineStream().
 */
EDGEVOX_ONNX_API EdgevoxOnnxOfflineStream *
EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(
    const EdgevoxOnnxSpokenLanguageIdentification *slid);

/**
 * @brief Result of spoken-language identification.
 *
 * Free this object with EdgevoxOnnxDestroySpokenLanguageIdentificationResult().
 */
typedef struct EdgevoxOnnxSpokenLanguageIdentificationResult {
  /**
   * Predicted language code such as `"en"`, `"de"`, `"zh"`, or `"es"`.
   */
  const char *lang;
} EdgevoxOnnxSpokenLanguageIdentificationResult;

/**
 * @brief Run spoken-language identification on an offline stream.
 *
 * Example:
 *
 * @code
 * EdgevoxOnnxOfflineStream *stream =
 *     EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream(slid);
 * EdgevoxOnnxAcceptWaveformOffline(stream, wave->sample_rate, wave->samples,
 *                                 wave->num_samples);
 * const EdgevoxOnnxSpokenLanguageIdentificationResult *result =
 *     EdgevoxOnnxSpokenLanguageIdentificationCompute(slid, stream);
 * printf("lang=%s\n", result->lang);
 * EdgevoxOnnxDestroySpokenLanguageIdentificationResult(result);
 * EdgevoxOnnxDestroyOfflineStream(stream);
 * @endcode
 *
 * @param slid A pointer returned by
 * EdgevoxOnnxCreateSpokenLanguageIdentification().
 * @param s A pointer returned by
 *          EdgevoxOnnxSpokenLanguageIdentificationCreateOfflineStream().
 * @return A newly allocated result object. Free it with
 *         EdgevoxOnnxDestroySpokenLanguageIdentificationResult().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpokenLanguageIdentificationResult *
EdgevoxOnnxSpokenLanguageIdentificationCompute(
    const EdgevoxOnnxSpokenLanguageIdentification *slid,
    const EdgevoxOnnxOfflineStream *s);

/**
 * @brief Destroy a spoken-language identification result.
 *
 * @param r A pointer returned by
 * EdgevoxOnnxSpokenLanguageIdentificationCompute().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySpokenLanguageIdentificationResult(
    const EdgevoxOnnxSpokenLanguageIdentificationResult *r);

// ============================================================
// For speaker embedding extraction
// ============================================================
/**
 * @brief Configuration for speaker embedding extraction.
 *
 * Example using
 * `3dspeaker_speech_campplus_sv_zh-cn_16k-common.onnx`:
 *
 * @code
 * EdgevoxOnnxSpeakerEmbeddingExtractorConfig config;
 * memset(&config, 0, sizeof(config));
 * config.model = "./3dspeaker_speech_campplus_sv_zh-cn_16k-common.onnx";
 * config.num_threads = 1;
 * config.provider = "cpu";
 * @endcode
 * @see EdgevoxOnnxCreateSpeakerEmbeddingExtractor
 */
typedef struct EdgevoxOnnxSpeakerEmbeddingExtractorConfig {
  /** Speaker embedding model file. */
  const char *model;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxSpeakerEmbeddingExtractorConfig;

/** @brief Opaque speaker embedding extractor handle. */
typedef struct EdgevoxOnnxSpeakerEmbeddingExtractor
    EdgevoxOnnxSpeakerEmbeddingExtractor;

/**
 * @brief Create a speaker embedding extractor.
 *
 * @param config Speaker embedding extractor configuration.
 * @return A newly allocated extractor on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroySpeakerEmbeddingExtractor().
 * @see EdgevoxOnnxSpeakerEmbeddingExtractorConfig, EdgevoxOnnxDestroySpeakerEmbeddingExtractor
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpeakerEmbeddingExtractor *
EdgevoxOnnxCreateSpeakerEmbeddingExtractor(
    const EdgevoxOnnxSpeakerEmbeddingExtractorConfig *config);

/**
 * @brief Destroy a speaker embedding extractor.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 * @see EdgevoxOnnxCreateSpeakerEmbeddingExtractor
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySpeakerEmbeddingExtractor(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p);

/**
 * @brief Return the embedding dimension produced by the extractor.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 * @return Embedding dimension.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingExtractorDim(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p);

/**
 * @brief Create a streaming feature buffer for embedding extraction.
 *
 * Feed samples with EdgevoxOnnxOnlineStreamAcceptWaveform(), then call
 * EdgevoxOnnxSpeakerEmbeddingExtractorIsReady() and
 * EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding().
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 * @return A newly allocated online stream. Free it with
 *         EdgevoxOnnxDestroyOnlineStream().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineStream *
EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p);

/**
 * @brief Check whether enough audio has been provided to compute an embedding.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 * @param s A pointer returned by
 * EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream().
 * @return 1 if the stream is ready; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p,
    const EdgevoxOnnxOnlineStream *s);

/**
 * @brief Compute the embedding for a stream.
 *
 * The returned vector has `EdgevoxOnnxSpeakerEmbeddingExtractorDim(p)` elements.
 * Free it with EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding().
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 * @param s A pointer returned by
 * EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream().
 * @return A newly allocated embedding vector.
 *
 * @code
 * const EdgevoxOnnxOnlineStream *stream =
 *     EdgevoxOnnxSpeakerEmbeddingExtractorCreateStream(ex);
 * EdgevoxOnnxOnlineStreamAcceptWaveform(stream, wave->sample_rate,
 * wave->samples, wave->num_samples);
 * EdgevoxOnnxOnlineStreamInputFinished(stream);
 * if (EdgevoxOnnxSpeakerEmbeddingExtractorIsReady(ex, stream)) {
 *   const float *v =
 *       EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(ex, stream);
 *   EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(v);
 * }
 * EdgevoxOnnxDestroyOnlineStream(stream);
 * @endcode
 */
EDGEVOX_ONNX_API const float *
EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding(
    const EdgevoxOnnxSpeakerEmbeddingExtractor *p,
    const EdgevoxOnnxOnlineStream *s);

/**
 * @brief Destroy an embedding vector returned by
 * EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding().
 *
 * @param v A pointer returned by
 *          EdgevoxOnnxSpeakerEmbeddingExtractorComputeEmbedding().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxSpeakerEmbeddingExtractorDestroyEmbedding(
    const float *v);

/**
 * @brief Opaque speaker embedding manager handle.
 *
 * @see EdgevoxOnnxCreateSpeakerEmbeddingManager
 */
typedef struct EdgevoxOnnxSpeakerEmbeddingManager
    EdgevoxOnnxSpeakerEmbeddingManager;

/**
 * @brief Create a speaker embedding manager.
 *
 * The manager stores enrolled speaker embeddings and supports speaker search
 * and verification.
 *
 * @param dim Embedding dimension. This should match
 *            EdgevoxOnnxSpeakerEmbeddingExtractorDim().
 * @return A newly allocated manager. Free it with
 *         EdgevoxOnnxDestroySpeakerEmbeddingManager().
 * @see EdgevoxOnnxDestroySpeakerEmbeddingManager
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpeakerEmbeddingManager *
EdgevoxOnnxCreateSpeakerEmbeddingManager(int32_t dim);

/**
 * @brief Destroy a speaker embedding manager.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @see EdgevoxOnnxCreateSpeakerEmbeddingManager
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySpeakerEmbeddingManager(
    const EdgevoxOnnxSpeakerEmbeddingManager *p);

/**
 * @brief Add one enrollment embedding for a speaker.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name.
 * @param v Embedding vector with exactly `dim` elements.
 * @return 1 on success; 0 on error.
 */
EDGEVOX_ONNX_API int32_t
EdgevoxOnnxSpeakerEmbeddingManagerAdd(const EdgevoxOnnxSpeakerEmbeddingManager *p,
                                     const char *name, const float *v);

/**
 * @brief Add multiple enrollment embeddings for one speaker.
 *
 * @p v is a NULL-terminated array of embedding pointers:
 * `v[0]`, `v[1]`, ..., `v[n - 1]`, followed by `v[n] == NULL`.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name.
 * @param v NULL-terminated array of embedding pointers.
 * @return 1 on success; 0 on error.
 *
 * @code
 * const float *spk1_vec[4] = {e1, e2, e3, NULL};
 * EdgevoxOnnxSpeakerEmbeddingManagerAddList(manager, "fangjun", spk1_vec);
 * @endcode
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerAddList(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float **v);

/**
 * @brief Add multiple enrollment embeddings packed in one flat array.
 *
 * The input contains @p n embeddings laid out consecutively, so the total
 * array length must be `n * dim`.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name.
 * @param v Flattened embedding array.
 * @param n Number of embeddings in @p v.
 * @return 1 on success; 0 on error.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerAddListFlattened(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float *v, int32_t n);

/**
 * @brief Remove a speaker from the manager.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name to remove.
 * @return 1 if removed; otherwise 0. Returns 0 if the speaker does not exist.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerRemove(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name);

/**
 * @brief Search for the best matching enrolled speaker.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param v Query embedding vector.
 * @param threshold Minimum similarity threshold in the range [0, 1].
 * @return A newly allocated speaker name on match, or NULL if no speaker
 *         passes the threshold. Free the returned name with
 *         EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch().
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxSpeakerEmbeddingManagerSearch(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const float *v,
    float threshold);

/**
 * @brief Free a string returned by EdgevoxOnnxSpeakerEmbeddingManagerSearch().
 *
 * @param name A pointer returned by
 *             EdgevoxOnnxSpeakerEmbeddingManagerSearch().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxSpeakerEmbeddingManagerFreeSearch(
    const char *name);

/**
 * @brief One speaker match returned by the best-matches API.
 */
typedef struct EdgevoxOnnxSpeakerEmbeddingManagerSpeakerMatch {
  /** Similarity score. Larger means more similar. */
  float score;
  /** Speaker name. */
  const char *name;
} EdgevoxOnnxSpeakerEmbeddingManagerSpeakerMatch;

/**
 * @brief Collection of best speaker matches.
 *
 * Free this object with EdgevoxOnnxSpeakerEmbeddingManagerFreeBestMatches().
 */
typedef struct EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult {
  /** Pointer to an array of @c count matches. */
  const EdgevoxOnnxSpeakerEmbeddingManagerSpeakerMatch *matches;
  /** Number of valid entries in @c matches. */
  int32_t count;
} EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult;

/**
 * @brief Return up to @p n best matches above a similarity threshold.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param v Query embedding vector.
 * @param threshold Minimum similarity threshold in the range [0, 1].
 * @param n Maximum number of matches to return.
 * @return A newly allocated result object, or NULL if no matches are found.
 *         Free it with EdgevoxOnnxSpeakerEmbeddingManagerFreeBestMatches().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult *
EdgevoxOnnxSpeakerEmbeddingManagerGetBestMatches(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const float *v, float threshold,
    int32_t n);

/**
 * @brief Destroy a best-matches result.
 *
 * @param r A pointer returned by
 * EdgevoxOnnxSpeakerEmbeddingManagerGetBestMatches().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxSpeakerEmbeddingManagerFreeBestMatches(
    const EdgevoxOnnxSpeakerEmbeddingManagerBestMatchesResult *r);

/**
 * @brief Verify whether a query embedding matches a named speaker.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name to compare against.
 * @param v Query embedding vector.
 * @param threshold Minimum similarity threshold in the range [0, 1].
 * @return 1 if the speaker matches; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerVerify(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name,
    const float *v, float threshold);

/**
 * @brief Check whether a speaker is enrolled.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @param name Speaker name.
 * @return 1 if the speaker exists; otherwise 0.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerContains(
    const EdgevoxOnnxSpeakerEmbeddingManager *p, const char *name);

/**
 * @brief Return the number of enrolled speakers.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @return Number of enrolled speakers.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxSpeakerEmbeddingManagerNumSpeakers(
    const EdgevoxOnnxSpeakerEmbeddingManager *p);

/**
 * @brief Return all enrolled speaker names.
 *
 * The returned array is NULL-terminated. If no speakers are enrolled, the
 * returned array still exists and its first element is NULL.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateSpeakerEmbeddingManager().
 * @return A newly allocated NULL-terminated array of speaker names. Free it
 *         with EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers().
 */
EDGEVOX_ONNX_API const char *const *
EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers(
    const EdgevoxOnnxSpeakerEmbeddingManager *p);

/**
 * @brief Free an array returned by
 * EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers().
 *
 * @param names A pointer returned by
 * EdgevoxOnnxSpeakerEmbeddingManagerGetAllSpeakers().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxSpeakerEmbeddingManagerFreeAllSpeakers(
    const char *const *names);

// ============================================================
// For audio tagging
// ============================================================
/** @brief Zipformer audio-tagging model configuration. */
typedef struct EdgevoxOnnxOfflineZipformerAudioTaggingModelConfig {
  /** Model filename. */
  const char *model;
} EdgevoxOnnxOfflineZipformerAudioTaggingModelConfig;

/**
 * @brief Audio-tagging model configuration.
 *
 * Configure exactly one model family. If multiple model families are provided,
 * one of them will be used and the choice is implementation-defined.
 *
 * Example using
 * `sherpa-onnx-zipformer-audio-tagging-2024-04-09`:
 *
 * @code
 * EdgevoxOnnxAudioTaggingModelConfig model;
 * memset(&model, 0, sizeof(model));
 * model.zipformer.model =
 *     "./sherpa-onnx-zipformer-audio-tagging-2024-04-09/model.int8.onnx";
 * model.num_threads = 1;
 * model.provider = "cpu";
 * @endcode
 */
typedef struct EdgevoxOnnxAudioTaggingModelConfig {
  /** Zipformer model configuration. */
  EdgevoxOnnxOfflineZipformerAudioTaggingModelConfig zipformer;
  /** Alternative CED model file. */
  const char *ced;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxAudioTaggingModelConfig;

/**
 * @brief Configuration for audio tagging.
 *
 * @code
 * EdgevoxOnnxAudioTaggingConfig config;
 * memset(&config, 0, sizeof(config));
 * config.model.zipformer.model =
 *     "./sherpa-onnx-zipformer-audio-tagging-2024-04-09/model.int8.onnx";
 * config.model.num_threads = 1;
 * config.model.provider = "cpu";
 * config.labels =
 *     "./sherpa-onnx-zipformer-audio-tagging-2024-04-09/class_labels_indices.csv";
 * config.top_k = 5;
 * @endcode
 * @see EdgevoxOnnxCreateAudioTagging
 */
typedef struct EdgevoxOnnxAudioTaggingConfig {
  /** Acoustic model configuration. */
  EdgevoxOnnxAudioTaggingModelConfig model;
  /** CSV file containing class labels. */
  const char *labels;
  /** Default number of results to return when `top_k == -1` at inference time.
   */
  int32_t top_k;
} EdgevoxOnnxAudioTaggingConfig;

/**
 * @brief One audio-tagging prediction.
 */
typedef struct EdgevoxOnnxAudioEvent {
  /** Event label. */
  const char *name;
  /** Integer label index. */
  int32_t index;
  /** Probability or confidence score. */
  float prob;
} EdgevoxOnnxAudioEvent;

/** @brief Opaque audio tagger handle. */
typedef struct EdgevoxOnnxAudioTagging EdgevoxOnnxAudioTagging;

/**
 * @brief Create an audio tagger.
 *
 * @param config Audio-tagging configuration.
 * @return A newly allocated audio tagger on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyAudioTagging().
 * @see EdgevoxOnnxAudioTaggingConfig, EdgevoxOnnxDestroyAudioTagging
 */
EDGEVOX_ONNX_API const EdgevoxOnnxAudioTagging *EdgevoxOnnxCreateAudioTagging(
    const EdgevoxOnnxAudioTaggingConfig *config);

/**
 * @brief Destroy an audio tagger.
 *
 * @param tagger A pointer returned by EdgevoxOnnxCreateAudioTagging().
 * @see EdgevoxOnnxCreateAudioTagging
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyAudioTagging(
    const EdgevoxOnnxAudioTagging *tagger);

/**
 * @brief Create an offline stream for audio tagging.
 *
 * @param tagger A pointer returned by EdgevoxOnnxCreateAudioTagging().
 * @return A newly allocated offline stream. Free it with
 *         EdgevoxOnnxDestroyOfflineStream().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineStream *
EdgevoxOnnxAudioTaggingCreateOfflineStream(const EdgevoxOnnxAudioTagging *tagger);

/**
 * @brief Run audio tagging on an offline stream.
 *
 * The returned array is NULL-terminated. If @p top_k is -1, the value stored in
 * `config.top_k` is used instead.
 *
 * @param tagger A pointer returned by EdgevoxOnnxCreateAudioTagging().
 * @param s A pointer returned by EdgevoxOnnxAudioTaggingCreateOfflineStream().
 * @param top_k Number of top results to return, or -1 to use the configured
 *              default.
 * @return A newly allocated NULL-terminated array of result pointers ordered by
 *         descending probability. Free it with
 *         EdgevoxOnnxAudioTaggingFreeResults().
 *
 * @code
 * const EdgevoxOnnxAudioEvent *const *results =
 *     EdgevoxOnnxAudioTaggingCompute(tagger, stream, 5);
 * for (int32_t i = 0; results[i] != NULL; ++i) {
 *   printf("%d %.3f %s\n", results[i]->index, results[i]->prob,
 *          results[i]->name);
 * }
 * EdgevoxOnnxAudioTaggingFreeResults(results);
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxAudioEvent *const *
EdgevoxOnnxAudioTaggingCompute(const EdgevoxOnnxAudioTagging *tagger,
                              const EdgevoxOnnxOfflineStream *s, int32_t top_k);

/**
 * @brief Destroy results returned by EdgevoxOnnxAudioTaggingCompute().
 *
 * @param p A pointer returned by EdgevoxOnnxAudioTaggingCompute().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxAudioTaggingFreeResults(
    const EdgevoxOnnxAudioEvent *const *p);

// ============================================================
// For punctuation
// ============================================================

/**
 * @brief Offline punctuation model configuration.
 *
 * Example:
 *
 * @code
 * EdgevoxOnnxOfflinePunctuationModelConfig model;
 * memset(&model, 0, sizeof(model));
 * model.ct_transformer =
 *     "./sherpa-onnx-punct-ct-transformer-zh-en-vocab272727-2024-04-12/model.onnx";
 * model.num_threads = 1;
 * model.provider = "cpu";
 * @endcode
 */
typedef struct EdgevoxOnnxOfflinePunctuationModelConfig {
  /** Offline punctuation model file. */
  const char *ct_transformer;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxOfflinePunctuationModelConfig;

/** @brief Configuration for offline punctuation. */
typedef struct EdgevoxOnnxOfflinePunctuationConfig {
  /** Model configuration. */
  EdgevoxOnnxOfflinePunctuationModelConfig model;
} EdgevoxOnnxOfflinePunctuationConfig;

/** @brief Opaque offline punctuation handle. */
typedef struct EdgevoxOnnxOfflinePunctuation EdgevoxOnnxOfflinePunctuation;

/**
 * @brief Create an offline punctuation processor.
 *
 * @param config Offline punctuation configuration.
 * @return A newly allocated punctuation processor on success, or NULL on
 *         error. Free it with EdgevoxOnnxDestroyOfflinePunctuation().
 * @see EdgevoxOnnxDestroyOfflinePunctuation, SherpaOfflinePunctuationAddPunct
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflinePunctuation *
EdgevoxOnnxCreateOfflinePunctuation(
    const EdgevoxOnnxOfflinePunctuationConfig *config);

/**
 * @brief Destroy an offline punctuation processor.
 *
 * @param punct A pointer returned by EdgevoxOnnxCreateOfflinePunctuation().
 * @see EdgevoxOnnxCreateOfflinePunctuation
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflinePunctuation(
    const EdgevoxOnnxOfflinePunctuation *punct);

/**
 * @brief Add punctuation to a complete input text.
 *
 * @param punct A pointer returned by EdgevoxOnnxCreateOfflinePunctuation().
 * @param text Input text without punctuation.
 * @return A newly allocated punctuated string. Free it with
 *         SherpaOfflinePunctuationFreeText().
 * @see SherpaOfflinePunctuationFreeText
 */
EDGEVOX_ONNX_API const char *SherpaOfflinePunctuationAddPunct(
    const EdgevoxOnnxOfflinePunctuation *punct, const char *text);

/**
 * @brief Free a string returned by SherpaOfflinePunctuationAddPunct().
 *
 * @param text A pointer returned by SherpaOfflinePunctuationAddPunct().
 * @see SherpaOfflinePunctuationAddPunct
 */
EDGEVOX_ONNX_API void SherpaOfflinePunctuationFreeText(const char *text);

/**
 * @brief Online punctuation model configuration.
 *
 * Example using `sherpa-onnx-online-punct-en-2024-08-06`:
 *
 * @code
 * EdgevoxOnnxOnlinePunctuationModelConfig model;
 * memset(&model, 0, sizeof(model));
 * model.cnn_bilstm =
 * "./sherpa-onnx-online-punct-en-2024-08-06/model.int8.onnx"; model.bpe_vocab =
 * "./sherpa-onnx-online-punct-en-2024-08-06/bpe.vocab"; model.num_threads = 1;
 * model.provider = "cpu";
 * @endcode
 */
typedef struct EdgevoxOnnxOnlinePunctuationModelConfig {
  /** Online punctuation model file. */
  const char *cnn_bilstm;
  /** BPE vocabulary used by the model. */
  const char *bpe_vocab;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxOnlinePunctuationModelConfig;

/** @brief Configuration for online punctuation. */
typedef struct EdgevoxOnnxOnlinePunctuationConfig {
  /** Model configuration. */
  EdgevoxOnnxOnlinePunctuationModelConfig model;
} EdgevoxOnnxOnlinePunctuationConfig;

/** @brief Opaque online punctuation handle. */
typedef struct EdgevoxOnnxOnlinePunctuation EdgevoxOnnxOnlinePunctuation;

/**
 * @brief Create an online punctuation processor.
 *
 * @param config Online punctuation configuration.
 * @return A newly allocated punctuation processor on success, or NULL on
 *         error. Free it with EdgevoxOnnxDestroyOnlinePunctuation().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlinePunctuation *
EdgevoxOnnxCreateOnlinePunctuation(
    const EdgevoxOnnxOnlinePunctuationConfig *config);

/**
 * @brief Destroy an online punctuation processor.
 *
 * @param punctuation A pointer returned by EdgevoxOnnxCreateOnlinePunctuation().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlinePunctuation(
    const EdgevoxOnnxOnlinePunctuation *punctuation);

/**
 * @brief Add punctuation to one text chunk using the online punctuation model.
 *
 * @param punctuation A pointer returned by EdgevoxOnnxCreateOnlinePunctuation().
 * @param text Input text chunk.
 * @return A newly allocated punctuated string. Free it with
 *         EdgevoxOnnxOnlinePunctuationFreeText().
 *
 * @code
 * const char *out =
 *     EdgevoxOnnxOnlinePunctuationAddPunct(punct,
 *         "how are you i am fine thank you");
 * printf("%s\n", out);
 * EdgevoxOnnxOnlinePunctuationFreeText(out);
 * @endcode
 */
EDGEVOX_ONNX_API const char *EdgevoxOnnxOnlinePunctuationAddPunct(
    const EdgevoxOnnxOnlinePunctuation *punctuation, const char *text);

/**
 * @brief Free a string returned by EdgevoxOnnxOnlinePunctuationAddPunct().
 *
 * @param text A pointer returned by EdgevoxOnnxOnlinePunctuationAddPunct().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlinePunctuationFreeText(const char *text);

// For resampling
/** @brief Opaque linear resampler handle. */
typedef struct EdgevoxOnnxLinearResampler EdgevoxOnnxLinearResampler;

/**
 * @brief Create a linear resampler.
 *
 * If @p filter_cutoff_hz or @p num_zeros is 0, the following defaults
 * are used (same convention as alsa-play.cc):
 *
 * @code
 * float min_freq = samp_rate_in_hz < samp_rate_out_hz ? samp_rate_in_hz
 *                                                 : samp_rate_out_hz;
 * float filter_cutoff_hz = 0.99f * 0.5f * min_freq;
 * int32_t num_zeros = 6;
 * @endcode
 *
 * @param samp_rate_in_hz Input sample rate in Hz. Must be > 0.
 * @param samp_rate_out_hz Output sample rate in Hz. Must be > 0.
 * @param filter_cutoff_hz Low-pass cutoff frequency in Hz. Pass 0 to use
 *                         the default formula above. Must be >= 0.
 * @param num_zeros Low-pass filter width control parameter. Pass 0 to use
 *                  the default value of 6. Must be >= 0.
 * @return A newly allocated resampler, or nullptr on invalid input. Free it
 *         with EdgevoxOnnxDestroyLinearResampler().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxLinearResampler *
EdgevoxOnnxCreateLinearResampler(int32_t samp_rate_in_hz,
                                int32_t samp_rate_out_hz,
                                float filter_cutoff_hz, int32_t num_zeros);

/**
 * @brief Destroy a linear resampler.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateLinearResampler().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyLinearResampler(
    const EdgevoxOnnxLinearResampler *p);

/**
 * @brief Reset a linear resampler to its initial state.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateLinearResampler().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxLinearResamplerReset(
    const EdgevoxOnnxLinearResampler *p);

/**
 * @brief Output chunk returned by EdgevoxOnnxLinearResamplerResample().
 *
 * Free this object with EdgevoxOnnxLinearResamplerResampleFree().
 */
typedef struct EdgevoxOnnxResampleOut {
  /** Output samples. */
  const float *samples;
  /** Number of output samples. */
  int32_t n;
} EdgevoxOnnxResampleOut;

/**
 * @brief Resample one chunk of input audio.
 *
 * Set @p flush to 1 for the final chunk so buffered samples are emitted.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateLinearResampler().
 * @param input Input sample array.
 * @param input_dim Number of input samples.
 * @param flush 1 if this is the final chunk; otherwise 0.
 * @return A newly allocated output chunk. Free it with
 *         EdgevoxOnnxLinearResamplerResampleFree().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxResampleOut *EdgevoxOnnxLinearResamplerResample(
    const EdgevoxOnnxLinearResampler *p, const float *input, int32_t input_dim,
    int32_t flush);

/**
 * @brief Destroy a resampler output chunk.
 *
 * @param p A pointer returned by EdgevoxOnnxLinearResamplerResample().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxLinearResamplerResampleFree(
    const EdgevoxOnnxResampleOut *p);

/**
 * @brief Return the resampler input sample rate.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateLinearResampler().
 * @return Input sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxLinearResamplerResampleGetInputSampleRate(
    const EdgevoxOnnxLinearResampler *p);

/**
 * @brief Return the resampler output sample rate.
 *
 * @param p A pointer returned by EdgevoxOnnxCreateLinearResampler().
 * @return Output sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxLinearResamplerResampleGetOutputSampleRate(
    const EdgevoxOnnxLinearResampler *p);

// =========================================================================
// For offline speaker diarization (i.e., non-streaming speaker diarization)
// =========================================================================
/** @brief Pyannote speaker-segmentation model configuration. */
typedef struct EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig {
  /** Segmentation model filename. */
  const char *model;
} EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig;

/**
 * @brief Segmentation model configuration for offline speaker diarization.
 *
 * Configure exactly one model family. If multiple model families are provided,
 * one is chosen and the choice is implementation-defined.
 */
typedef struct EdgevoxOnnxOfflineSpeakerSegmentationModelConfig {
  /** Pyannote segmentation model configuration. */
  EdgevoxOnnxOfflineSpeakerSegmentationPyannoteModelConfig pyannote;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxOfflineSpeakerSegmentationModelConfig;

/**
 * @brief Fast clustering configuration.
 *
 * If @c num_clusters is greater than 0, @c threshold is ignored. When the
 * number of speakers is known in advance, setting @c num_clusters is strongly
 * recommended.
 */
typedef struct EdgevoxOnnxFastClusteringConfig {
  /** Known number of speakers. If > 0, threshold-based clustering is bypassed.
   */
  int32_t num_clusters;
  /** Distance threshold used when the number of speakers is unknown. */
  float threshold;
} EdgevoxOnnxFastClusteringConfig;

/**
 * @brief Configuration for offline speaker diarization.
 *
 * Example based on `offline-sepaker-diarization-c-api.c`:
 *
 * @code
 * EdgevoxOnnxOfflineSpeakerDiarizationConfig config;
 * memset(&config, 0, sizeof(config));
 * config.segmentation.pyannote.model =
 *     "./sherpa-onnx-pyannote-segmentation-3-0/model.onnx";
 * config.embedding.model =
 *     "./3dspeaker_speech_eres2net_base_sv_zh-cn_3dspeaker_16k.onnx";
 * config.clustering.num_clusters = 4;
 * @endcode
 */
typedef struct EdgevoxOnnxOfflineSpeakerDiarizationConfig {
  /** Speaker segmentation model configuration. */
  EdgevoxOnnxOfflineSpeakerSegmentationModelConfig segmentation;
  /** Speaker embedding extractor configuration. */
  EdgevoxOnnxSpeakerEmbeddingExtractorConfig embedding;
  /** Clustering configuration. */
  EdgevoxOnnxFastClusteringConfig clustering;
  /** Segments shorter than this duration in seconds are discarded. */
  float min_duration_on;
  /** Small gaps shorter than this duration in seconds may be merged. */
  float min_duration_off;
} EdgevoxOnnxOfflineSpeakerDiarizationConfig;

/** @brief Opaque offline speaker diarization handle. */
typedef struct EdgevoxOnnxOfflineSpeakerDiarization
    EdgevoxOnnxOfflineSpeakerDiarization;

/**
 * @brief Create an offline speaker diarization pipeline.
 *
 * @param config Offline speaker diarization configuration.
 * @return A newly allocated diarizer on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOfflineSpeakerDiarization().
 * @see EdgevoxOnnxDestroyOfflineSpeakerDiarization
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config);

/**
 * @brief Destroy an offline speaker diarizer.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @see EdgevoxOnnxCreateOfflineSpeakerDiarization
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineSpeakerDiarization(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd);

/**
 * @brief Return the expected input sample rate.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @return Required input sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd);

/**
 * @brief Update clustering-related settings of an existing diarizer.
 *
 * Only `config->clustering` is used. Other fields are ignored.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @param config Configuration whose `clustering` field will be applied.
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOfflineSpeakerDiarizationSetConfig(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd,
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config);

/** @brief Opaque offline speaker diarization result. */
typedef struct EdgevoxOnnxOfflineSpeakerDiarizationResult
    EdgevoxOnnxOfflineSpeakerDiarizationResult;

/**
 * @brief One diarization segment.
 */
typedef struct EdgevoxOnnxOfflineSpeakerDiarizationSegment {
  /** Segment start time in seconds. */
  float start;
  /** Segment end time in seconds. */
  float end;
  /** Speaker label, typically an integer cluster ID. */
  int32_t speaker;
} EdgevoxOnnxOfflineSpeakerDiarizationSegment;

/**
 * @brief Return the number of speakers in a diarization result.
 *
 * @param r A pointer returned by one of the
 *          EdgevoxOnnxOfflineSpeakerDiarizationProcess*() functions.
 * @return Number of speaker clusters.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSpeakers(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r);

/**
 * @brief Return the number of diarization segments.
 *
 * @param r A pointer returned by one of the
 *          EdgevoxOnnxOfflineSpeakerDiarizationProcess*() functions.
 * @return Number of segments.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r);

/**
 * @brief Return segments sorted by start time.
 *
 * The returned array contains exactly
 * EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments() entries.
 *
 * @param r A pointer returned by one of the
 *          EdgevoxOnnxOfflineSpeakerDiarizationProcess*() functions.
 * @return A newly allocated segment array. Free it with
 *         EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarizationSegment *
EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r);

/**
 * @brief Destroy a segment array returned by
 * EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime().
 *
 * @param s A pointer returned by
 *          EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment(
    const EdgevoxOnnxOfflineSpeakerDiarizationSegment *s);

/**
 * @brief Progress callback for offline speaker diarization.
 *
 * The current implementation reports progress but ignores the callback's
 * return value.
 */
typedef int32_t (*EdgevoxOnnxOfflineSpeakerDiarizationProgressCallback)(
    int32_t num_processed_chunks, int32_t num_total_chunks, void *arg);

/**
 * @brief Same as EdgevoxOnnxOfflineSpeakerDiarizationProgressCallback but
 * without a user pointer.
 */
typedef int32_t (*EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg)(
    int32_t num_processed_chunks, int32_t num_total_chunks);

/**
 * @brief Run offline speaker diarization.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @param samples Input mono PCM samples normalized to [-1, 1].
 * @param n Number of input samples.
 * @return A newly allocated diarization result. Free it with
 *         EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcess(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n);

/**
 * @brief Run offline speaker diarization with a progress callback.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @param samples Input mono PCM samples normalized to [-1, 1].
 * @param n Number of input samples.
 * @param callback Progress callback.
 * @param arg User pointer forwarded to @p callback.
 * @return A newly allocated diarization result. Free it with
 *         EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult().
 *
 * @code
 * static int32_t ProgressCallback(int32_t done, int32_t total, void *arg) {
 *   fprintf(stderr, "progress %.2f%%\n", 100.0f * done / total);
 *   return 0;
 * }
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n, EdgevoxOnnxOfflineSpeakerDiarizationProgressCallback callback,
    void *arg);

/**
 * @brief Run offline speaker diarization with a progress callback that has no
 * user pointer.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeakerDiarization().
 * @param samples Input mono PCM samples normalized to [-1, 1].
 * @param n Number of input samples.
 * @param callback Progress callback.
 * @return A newly allocated diarization result. Free it with
 *         EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarizationResult *
EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallbackNoArg(
    const EdgevoxOnnxOfflineSpeakerDiarization *sd, const float *samples,
    int32_t n,
    EdgevoxOnnxOfflineSpeakerDiarizationProgressCallbackNoArg callback);

/**
 * @brief Destroy a diarization result.
 *
 * @param r A pointer returned by one of the
 *          EdgevoxOnnxOfflineSpeakerDiarizationProcess*() functions.
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult(
    const EdgevoxOnnxOfflineSpeakerDiarizationResult *r);

// =========================================================================
// For offline speech enhancement
// =========================================================================
/** @brief GTCRN offline denoiser model configuration. */
typedef struct EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig {
  /** Model filename. */
  const char *model;
} EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig;

/** @brief DPDFNet offline denoiser model configuration. */
typedef struct EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig {
  /** Model filename. */
  const char *model;
} EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig;

/**
 * @brief Speech denoiser model configuration shared by offline and online APIs.
 *
 * Configure exactly one model family. If multiple model families are provided,
 * one is chosen and the choice is implementation-defined.
 */
typedef struct EdgevoxOnnxOfflineSpeechDenoiserModelConfig {
  /** GTCRN model configuration. */
  EdgevoxOnnxOfflineSpeechDenoiserGtcrnModelConfig gtcrn;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
  /** DPDFNet model configuration. */
  EdgevoxOnnxOfflineSpeechDenoiserDpdfNetModelConfig dpdfnet;
} EdgevoxOnnxOfflineSpeechDenoiserModelConfig;

/** @brief Configuration for offline speech denoising. */
typedef struct EdgevoxOnnxOfflineSpeechDenoiserConfig {
  /** Model configuration. */
  EdgevoxOnnxOfflineSpeechDenoiserModelConfig model;
} EdgevoxOnnxOfflineSpeechDenoiserConfig;

/** @brief Opaque offline speech denoiser handle. */
typedef struct EdgevoxOnnxOfflineSpeechDenoiser EdgevoxOnnxOfflineSpeechDenoiser;

/**
 * @brief Create an offline speech denoiser.
 *
 * Example using `gtcrn_simple.onnx`:
 *
 * @code
 * EdgevoxOnnxOfflineSpeechDenoiserConfig config;
 * memset(&config, 0, sizeof(config));
 * config.model.gtcrn.model = "./gtcrn_simple.onnx";
 * @endcode
 *
 * @param config Offline denoiser configuration.
 * @return A newly allocated denoiser on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOfflineSpeechDenoiser().
 * @see EdgevoxOnnxDestroyOfflineSpeechDenoiser
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeechDenoiser *
EdgevoxOnnxCreateOfflineSpeechDenoiser(
    const EdgevoxOnnxOfflineSpeechDenoiserConfig *config);

/**
 * @brief Destroy an offline speech denoiser.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeechDenoiser().
 * @see EdgevoxOnnxCreateOfflineSpeechDenoiser
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineSpeechDenoiser(
    const EdgevoxOnnxOfflineSpeechDenoiser *sd);

/**
 * @brief Return the expected sample rate for the denoiser.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeechDenoiser().
 * @return Required input sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate(
    const EdgevoxOnnxOfflineSpeechDenoiser *sd);

/**
 * @brief Denoised audio returned by offline or online speech enhancement APIs.
 *
 * Free this object with EdgevoxOnnxDestroyDenoisedAudio().
 */
typedef struct EdgevoxOnnxDenoisedAudio {
  /** Output samples in the range [-1, 1]. */
  const float *samples;
  /** Number of output samples. */
  int32_t n;
  /** Output sample rate in Hz. */
  int32_t sample_rate;
} EdgevoxOnnxDenoisedAudio;

/**
 * @brief Run offline speech denoising on a complete waveform.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOfflineSpeechDenoiser().
 * @param samples Input mono PCM samples normalized to [-1, 1].
 * @param n Number of input samples.
 * @param sample_rate Input sample rate in Hz.
 * @return A newly allocated denoised waveform. Free it with
 *         EdgevoxOnnxDestroyDenoisedAudio().
 *
 * @code
 * const EdgevoxOnnxDenoisedAudio *denoised =
 *     EdgevoxOnnxOfflineSpeechDenoiserRun(sd, wave->samples, wave->num_samples,
 *                                        wave->sample_rate);
 * EdgevoxOnnxWriteWave(denoised->samples, denoised->n, denoised->sample_rate,
 *                     "./enhanced.wav");
 * EdgevoxOnnxDestroyDenoisedAudio(denoised);
 * @endcode
 */
EDGEVOX_ONNX_API const EdgevoxOnnxDenoisedAudio *
EdgevoxOnnxOfflineSpeechDenoiserRun(const EdgevoxOnnxOfflineSpeechDenoiser *sd,
                                   const float *samples, int32_t n,
                                   int32_t sample_rate);

/**
 * @brief Destroy denoised audio returned by a speech enhancement API.
 *
 * @param p A pointer returned by EdgevoxOnnxOfflineSpeechDenoiserRun(),
 *          EdgevoxOnnxOnlineSpeechDenoiserRun(), or
 *          EdgevoxOnnxOnlineSpeechDenoiserFlush().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyDenoisedAudio(
    const EdgevoxOnnxDenoisedAudio *p);

// =========================================================================
// For streaming speech enhancement
// =========================================================================
/** @brief Configuration for streaming speech denoising. */
typedef struct EdgevoxOnnxOnlineSpeechDenoiserConfig {
  /** Model configuration. */
  EdgevoxOnnxOfflineSpeechDenoiserModelConfig model;
} EdgevoxOnnxOnlineSpeechDenoiserConfig;

/** @brief Opaque online speech denoiser handle. */
typedef struct EdgevoxOnnxOnlineSpeechDenoiser EdgevoxOnnxOnlineSpeechDenoiser;

/**
 * @brief Create an online speech denoiser.
 *
 * @param config Online denoiser configuration.
 * @return A newly allocated denoiser on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOnlineSpeechDenoiser().
 * @see EdgevoxOnnxDestroyOnlineSpeechDenoiser
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineSpeechDenoiser *
EdgevoxOnnxCreateOnlineSpeechDenoiser(
    const EdgevoxOnnxOnlineSpeechDenoiserConfig *config);

/**
 * @brief Destroy an online speech denoiser.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 * @see EdgevoxOnnxCreateOnlineSpeechDenoiser
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOnlineSpeechDenoiser(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd);

/**
 * @brief Return the expected input sample rate for the online denoiser.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 * @return Required input sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd);

/**
 * @brief Return the recommended chunk size in samples for streaming input.
 *
 * Example programs feed audio to the online denoiser in this chunk size.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 * @return Frame shift in samples.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd);

/**
 * @brief Process one chunk of streaming audio.
 *
 * This function is not thread-safe. It may return NULL when not enough input
 * has been accumulated to produce denoised output yet.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 * @param samples Input chunk normalized to [-1, 1].
 * @param n Number of input samples.
 * @param sample_rate Input sample rate in Hz.
 * @return A newly allocated denoised chunk, or NULL if no output is available
 *         yet. Free non-NULL results with EdgevoxOnnxDestroyDenoisedAudio().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxDenoisedAudio *
EdgevoxOnnxOnlineSpeechDenoiserRun(const EdgevoxOnnxOnlineSpeechDenoiser *sd,
                                  const float *samples, int32_t n,
                                  int32_t sample_rate);

/**
 * @brief Flush buffered samples and reset the online denoiser.
 *
 * This also resets the denoiser so it can be reused for a new utterance.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 * @return A newly allocated denoised chunk, or NULL if no buffered output
 *         remains. Free non-NULL results with EdgevoxOnnxDestroyDenoisedAudio().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxDenoisedAudio *
EdgevoxOnnxOnlineSpeechDenoiserFlush(const EdgevoxOnnxOnlineSpeechDenoiser *sd);

/**
 * @brief Reset an online denoiser so it can process a new stream.
 *
 * @param sd A pointer returned by EdgevoxOnnxCreateOnlineSpeechDenoiser().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxOnlineSpeechDenoiserReset(
    const EdgevoxOnnxOnlineSpeechDenoiser *sd);

// =========================================================================
// Source separation
// =========================================================================

/** @brief Spleeter source-separation model configuration. */
typedef struct EdgevoxOnnxOfflineSourceSeparationSpleeterModelConfig {
  /** Path to the vocals ONNX model. */
  const char *vocals;
  /** Path to the accompaniment ONNX model. */
  const char *accompaniment;
} EdgevoxOnnxOfflineSourceSeparationSpleeterModelConfig;

/** @brief UVR (MDX-Net) source-separation model configuration. */
typedef struct EdgevoxOnnxOfflineSourceSeparationUvrModelConfig {
  /** Path to the UVR ONNX model. */
  const char *model;
} EdgevoxOnnxOfflineSourceSeparationUvrModelConfig;

/** @brief Source-separation model configuration. */
typedef struct EdgevoxOnnxOfflineSourceSeparationModelConfig {
  EdgevoxOnnxOfflineSourceSeparationSpleeterModelConfig spleeter;
  EdgevoxOnnxOfflineSourceSeparationUvrModelConfig uvr;
  int32_t num_threads;
  int32_t debug;
  const char *provider;
} EdgevoxOnnxOfflineSourceSeparationModelConfig;

/** @brief Top-level source-separation configuration. */
typedef struct EdgevoxOnnxOfflineSourceSeparationConfig {
  EdgevoxOnnxOfflineSourceSeparationModelConfig model;
} EdgevoxOnnxOfflineSourceSeparationConfig;

/** @brief Opaque source-separation engine handle. */
typedef struct EdgevoxOnnxOfflineSourceSeparation
    EdgevoxOnnxOfflineSourceSeparation;

/**
 * @brief Create a source-separation engine.
 *
 * @param config Source-separation configuration.
 * @return A newly allocated engine on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOfflineSourceSeparation().
 * @see EdgevoxOnnxDestroyOfflineSourceSeparation
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSourceSeparation *
EdgevoxOnnxCreateOfflineSourceSeparation(
    const EdgevoxOnnxOfflineSourceSeparationConfig *config);

/**
 * @brief Destroy a source-separation engine.
 *
 * @param ss A pointer returned by EdgevoxOnnxCreateOfflineSourceSeparation().
 * @see EdgevoxOnnxCreateOfflineSourceSeparation
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineSourceSeparation(
    const EdgevoxOnnxOfflineSourceSeparation *ss);

/**
 * @brief Return the output sample rate of the source-separation engine.
 *
 * @param ss A pointer returned by EdgevoxOnnxCreateOfflineSourceSeparation().
 * @return Output sample rate in Hz.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSourceSeparationGetOutputSampleRate(
    const EdgevoxOnnxOfflineSourceSeparation *ss);

/**
 * @brief Return the number of stems produced by the engine.
 *
 * For Spleeter 2-stems this returns 2 (vocals + accompaniment).
 *
 * @param ss A pointer returned by EdgevoxOnnxCreateOfflineSourceSeparation().
 * @return Number of output stems.
 */
EDGEVOX_ONNX_API int32_t EdgevoxOnnxOfflineSourceSeparationGetNumberOfStems(
    const EdgevoxOnnxOfflineSourceSeparation *ss);

/** @brief A single stem (one output track) with one or more channels. */
typedef struct EdgevoxOnnxSourceSeparationStem {
  /** samples[c] points to the heap-allocated sample array for channel c. */
  float **samples;
  /** Number of channels in this stem. */
  int32_t num_channels;
  /** Number of samples per channel. */
  int32_t n;
} EdgevoxOnnxSourceSeparationStem;

/** @brief Output of a source-separation run. */
typedef struct EdgevoxOnnxSourceSeparationOutput {
  /** Heap-allocated array of stems (length num_stems). */
  const EdgevoxOnnxSourceSeparationStem *stems;
  /** Number of stems. */
  int32_t num_stems;
  /** Sample rate of every stem in Hz. */
  int32_t sample_rate;
} EdgevoxOnnxSourceSeparationOutput;

/**
 * @brief Run source separation on multi-channel audio.
 *
 * All input channels must have the same number of samples.
 *
 * @param ss            A pointer returned by
 *                      EdgevoxOnnxCreateOfflineSourceSeparation().
 * @param samples       samples[c] is a float array for channel c, values in
 *                      [-1, 1].
 * @param num_channels  Number of input channels.
 * @param num_samples   Number of samples per channel (all channels must have
 *                      the same length).
 * @param sample_rate   Input sample rate in Hz.
 * @return A newly allocated output on success, or NULL on error. Free it
 *         with EdgevoxOnnxDestroySourceSeparationOutput().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSourceSeparationOutput *
EdgevoxOnnxOfflineSourceSeparationProcess(
    const EdgevoxOnnxOfflineSourceSeparation *ss, const float *const *samples,
    int32_t num_channels, int32_t num_samples, int32_t sample_rate);

/**
 * @brief Destroy the output of a source-separation run.
 *
 * @param p A pointer returned by EdgevoxOnnxOfflineSourceSeparationProcess().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroySourceSeparationOutput(
    const EdgevoxOnnxSourceSeparationOutput *p);

#ifdef __OHOS__

/**
 * @brief HarmonyOS native resource manager type.
 *
 * Pass the resource manager provided by the HarmonyOS application runtime when
 * using the `*OHOS()` constructors below.
 */
typedef struct NativeResourceManager NativeResourceManager;

/**
 * @brief Create an offline speech denoiser on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOfflineSpeechDenoiser().
 *
 * @param config Offline denoiser configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated denoiser, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineSpeechDenoiser().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeechDenoiser *
EdgevoxOnnxCreateOfflineSpeechDenoiserOHOS(
    const EdgevoxOnnxOfflineSpeechDenoiserConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create an online speech denoiser on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOnlineSpeechDenoiser().
 *
 * @param config Online denoiser configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated denoiser, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOnlineSpeechDenoiser().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineSpeechDenoiser *
EdgevoxOnnxCreateOnlineSpeechDenoiserOHOS(
    const EdgevoxOnnxOnlineSpeechDenoiserConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create an online recognizer on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOnlineRecognizer().
 *
 * @param config Recognizer configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated recognizer, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOnlineRecognizer().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlineRecognizer *
EdgevoxOnnxCreateOnlineRecognizerOHOS(
    const EdgevoxOnnxOnlineRecognizerConfig *config, NativeResourceManager *mgr);

/**
 * @brief Create an offline recognizer on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOfflineRecognizer().
 *
 * @param config Recognizer configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated recognizer, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineRecognizer().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineRecognizer *
EdgevoxOnnxCreateOfflineRecognizerOHOS(
    const EdgevoxOnnxOfflineRecognizerConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create a voice activity detector on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateVoiceActivityDetector().
 *
 * @param config VAD model configuration.
 * @param buffer_size_in_seconds Internal buffer duration in seconds.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated VAD instance, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyVoiceActivityDetector().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxVoiceActivityDetector *
EdgevoxOnnxCreateVoiceActivityDetectorOHOS(
    const EdgevoxOnnxVadModelConfig *config, float buffer_size_in_seconds,
    NativeResourceManager *mgr);

/**
 * @brief Create an offline TTS engine on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOfflineTts().
 *
 * @param config Offline TTS configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated TTS engine, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineTts().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineTts *EdgevoxOnnxCreateOfflineTtsOHOS(
    const EdgevoxOnnxOfflineTtsConfig *config, NativeResourceManager *mgr);

/**
 * @brief Create an offline punctuation processor on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOfflinePunctuation().
 *
 * @param config Offline punctuation configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated punctuation processor, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOfflinePunctuation().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflinePunctuation *
EdgevoxOnnxCreateOfflinePunctuationOHOS(
    const EdgevoxOnnxOfflinePunctuationConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create an online punctuation processor on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateOnlinePunctuation().
 *
 * @param config Online punctuation configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated punctuation processor, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOnlinePunctuation().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOnlinePunctuation *
EdgevoxOnnxCreateOnlinePunctuationOHOS(
    const EdgevoxOnnxOnlinePunctuationConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create a speaker embedding extractor on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of
 * EdgevoxOnnxCreateSpeakerEmbeddingExtractor().
 *
 * @param config Speaker embedding extractor configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated extractor, or NULL on error. Free it with
 *         EdgevoxOnnxDestroySpeakerEmbeddingExtractor().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxSpeakerEmbeddingExtractor *
EdgevoxOnnxCreateSpeakerEmbeddingExtractorOHOS(
    const EdgevoxOnnxSpeakerEmbeddingExtractorConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create a keyword spotter on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of EdgevoxOnnxCreateKeywordSpotter().
 *
 * @param config Keyword spotter configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated keyword spotter, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyKeywordSpotter().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxKeywordSpotter *
EdgevoxOnnxCreateKeywordSpotterOHOS(const EdgevoxOnnxKeywordSpotterConfig *config,
                                   NativeResourceManager *mgr);

/**
 * @brief Create an offline speaker diarizer on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of
 * EdgevoxOnnxCreateOfflineSpeakerDiarization().
 *
 * @param config Offline speaker diarization configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated diarizer, or NULL on error. Free it with
 *         EdgevoxOnnxDestroyOfflineSpeakerDiarization().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSpeakerDiarization *
EdgevoxOnnxCreateOfflineSpeakerDiarizationOHOS(
    const EdgevoxOnnxOfflineSpeakerDiarizationConfig *config,
    NativeResourceManager *mgr);

/**
 * @brief Create a source separation engine on HarmonyOS.
 *
 * This is the HarmonyOS counterpart of
 * EdgevoxOnnxCreateOfflineSourceSeparation().
 *
 * @param config Source separation configuration.
 * @param mgr HarmonyOS resource manager used to resolve bundled assets.
 * @return A newly allocated source separation engine, or NULL on error. Free it
 *         with EdgevoxOnnxDestroyOfflineSourceSeparation().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineSourceSeparation *
EdgevoxOnnxCreateOfflineSourceSeparationOHOS(
    const EdgevoxOnnxOfflineSourceSeparationConfig *config,
    NativeResourceManager *mgr);
#endif

// ============================================================
// For diacritization
// ============================================================

/**
 * @brief Offline diacritization model configuration.
 */
typedef struct EdgevoxOnnxOfflineDiacritizationModelConfig {
  /** Offline diacritization encoder model file. */
  const char *catt_encoder;
  /** Offline diacritization decoder model file. */
  const char *catt_decoder;
  /** Number of inference threads. */
  int32_t num_threads;
  /** Non-zero to print debug information. */
  int32_t debug;
  /** Execution provider such as `"cpu"`. */
  const char *provider;
} EdgevoxOnnxOfflineDiacritizationModelConfig;

/** @brief Configuration for offline diacritization. */
typedef struct EdgevoxOnnxOfflineDiacritizationConfig {
  /** Model configuration. */
  EdgevoxOnnxOfflineDiacritizationModelConfig model;
} EdgevoxOnnxOfflineDiacritizationConfig;

/** @brief Opaque offline diacritization handle. */
typedef struct EdgevoxOnnxOfflineDiacritization EdgevoxOnnxOfflineDiacritization;

/**
 * @brief Create an offline diacritization processor.
 *
 * @param config Offline diacritization configuration.
 * @return A newly allocated diacritization processor on success, or NULL on
 *         error. Free it with EdgevoxOnnxDestroyOfflineDiacritization().
 */
EDGEVOX_ONNX_API const EdgevoxOnnxOfflineDiacritization *
EdgevoxOnnxCreateOfflineDiacritization(
    const EdgevoxOnnxOfflineDiacritizationConfig *config);

/**
 * @brief Destroy an offline diacritization processor.
 *
 * @param diacrt A pointer returned by EdgevoxOnnxCreateOfflineDiacritization().
 */
EDGEVOX_ONNX_API void EdgevoxOnnxDestroyOfflineDiacritization(
    const EdgevoxOnnxOfflineDiacritization *diacrt);

/**
 * @brief Add diacritics to a complete input text.
 *
 * @param diacrt A pointer returned by EdgevoxOnnxCreateOfflineDiacritization().
 * @param text Input text without diacritics.
 * @return A newly allocated diacritized string. Free it with
 *         SherpaOfflineDiacritizationFreeText().
 */
EDGEVOX_ONNX_API const char *SherpaOfflineDiacritizationAddDiacritics(
    const EdgevoxOnnxOfflineDiacritization *diacrt, const char *text);

/**
 * @brief Free a string returned by SherpaOfflineDiacritizationAddDiacritics().
 *
 * @param text A pointer returned by SherpaOfflineDiacritizationAddDiacritics().
 */
EDGEVOX_ONNX_API void SherpaOfflineDiacritizationFreeText(const char *text);

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // EDGEVOX_ONNX_C_API_C_API_H_
