include_directories(${PROJECT_SOURCE_DIR})

if(EDGEVOX_ONNX_ENABLE_PYTHON)
  message(STATUS "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "import sys; print('.'.join(sys.version.split('.')[:2]))"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_VERSION
  )
  message(STATUS "PYTHON_VERSION: ${PYTHON_VERSION}")
endif()

set(sources
  base64-decode.cc
  bbpe.cc
  cat.cc
  circular-buffer.cc
  context-graph.cc
  endpoint.cc
  features.cc
  file-utils.cc
  fst-utils.cc
  homophone-replacer.cc
  hypothesis.cc
  keyword-spotter-impl.cc
  keyword-spotter.cc
  lodr-fst.cc
  math.cc
  normal-data-generator.cc
  offline-canary-model-config.cc
  offline-canary-model.cc
  offline-cohere-transcribe-greedy-search-decoder.cc
  offline-cohere-transcribe-model-config.cc
  offline-cohere-transcribe-model.cc
  offline-ctc-fst-decoder-config.cc
  offline-ctc-fst-decoder.cc
  offline-ctc-greedy-search-decoder.cc
  offline-ctc-model.cc
  offline-dolphin-model-config.cc
  offline-dolphin-model.cc
  offline-fire-red-asr-ctc-model-config.cc
  offline-fire-red-asr-ctc-model.cc
  offline-fire-red-asr-greedy-search-decoder.cc
  offline-fire-red-asr-model-config.cc
  offline-fire-red-asr-model.cc
  offline-lm-config.cc
  offline-lm.cc
  offline-medasr-ctc-model-config.cc
  offline-medasr-ctc-model.cc
  offline-model-config.cc
  offline-moonshine-greedy-search-decoder.cc
  offline-moonshine-model-config.cc
  offline-moonshine-model-v2.cc
  offline-moonshine-model.cc
  offline-moonshine-v2-greedy-search-decoder.cc
  offline-nemo-enc-dec-ctc-model-config.cc
  offline-nemo-enc-dec-ctc-model.cc
  offline-omnilingual-asr-ctc-model-config.cc
  offline-omnilingual-asr-ctc-model.cc
  offline-paraformer-greedy-search-decoder.cc
  offline-paraformer-model-config.cc
  offline-paraformer-model.cc
  offline-recognizer-impl.cc
  offline-recognizer.cc
  offline-rnn-lm.cc
  offline-sense-voice-model-config.cc
  offline-sense-voice-model.cc
  offline-source-separation-impl.cc
  offline-source-separation-model-config.cc
  offline-source-separation-spleeter-model-config.cc
  offline-source-separation-spleeter-model.cc
  offline-source-separation-uvr-model-config.cc
  offline-source-separation-uvr-model.cc
  offline-source-separation.cc
  offline-stream.cc
  offline-tdnn-ctc-model.cc
  offline-tdnn-model-config.cc
  offline-telespeech-ctc-model.cc
  offline-transducer-greedy-search-decoder.cc
  offline-transducer-greedy-search-nemo-decoder.cc
  offline-transducer-model-config.cc
  offline-transducer-model.cc
  offline-transducer-modified-beam-search-decoder.cc
  offline-transducer-modified-beam-search-nemo-decoder.cc
  offline-transducer-nemo-model.cc
  offline-wenet-ctc-model-config.cc
  offline-wenet-ctc-model.cc
  offline-whisper-dtw.cc
  offline-whisper-greedy-search-decoder.cc
  offline-whisper-model-config.cc
  offline-whisper-model.cc
  offline-whisper-timestamp-rules.cc
  offline-zipformer-ctc-model-config.cc
  offline-zipformer-ctc-model.cc
  online-conformer-transducer-model.cc
  online-ctc-fst-decoder-config.cc
  online-ctc-fst-decoder.cc
  online-ctc-greedy-search-decoder.cc
  online-ctc-model.cc
  online-ebranchformer-transducer-model.cc
  online-lm-config.cc
  online-lm.cc
  online-lstm-transducer-model.cc
  online-model-config.cc
  online-nemo-ctc-model-config.cc
  online-nemo-ctc-model.cc
  online-paraformer-model-config.cc
  online-paraformer-model.cc
  online-recognizer-impl.cc
  online-recognizer.cc
  online-rnn-lm.cc
  online-stream.cc
  online-t-one-ctc-model-config.cc
  online-t-one-ctc-model.cc
  online-transducer-decoder.cc
  online-transducer-greedy-search-decoder.cc
  online-transducer-greedy-search-nemo-parakeet-unified-decoder.cc
  online-transducer-greedy-search-nemo-decoder.cc
  online-transducer-model-config.cc
  online-transducer-model.cc
  online-transducer-modified-beam-search-decoder.cc
  online-transducer-nemo-parakeet-unified-model.cc
  online-transducer-nemo-model.cc
  online-wenet-ctc-model-config.cc
  online-wenet-ctc-model.cc
  online-zipformer-transducer-model.cc
  online-zipformer2-ctc-model-config.cc
  online-zipformer2-ctc-model.cc
  online-zipformer2-transducer-model.cc
  onnx-utils.cc
  packed-sequence.cc
  pad-sequence.cc
  parse-options.cc
  phrase-matcher.cc
  provider-config.cc
  provider.cc
  resample.cc
  session.cc
  silero-vad-model-config.cc
  silero-vad-model.cc
  slice.cc
  spoken-language-identification-impl.cc
  spoken-language-identification.cc
  stack.cc
  symbol-table.cc
  ten-vad-model-config.cc
  ten-vad-model.cc
  text-utils.cc
  timer.cc
  transducer-keyword-decoder.cc
  transpose.cc
  unbind.cc
  utils.cc
  vad-model-config.cc
  vad-model.cc
  version.cc
  voice-activity-detector.cc
  wave-reader.cc
  wave-writer.cc
)

# speaker embedding extractor
list(APPEND sources
  speaker-embedding-extractor-impl.cc
  speaker-embedding-extractor-model.cc
  speaker-embedding-extractor-nemo-model.cc
  speaker-embedding-extractor.cc
  speaker-embedding-manager.cc
)

# audio tagging
list(APPEND sources
  audio-tagging-impl.cc
  audio-tagging-label-file.cc
  audio-tagging-model-config.cc
  audio-tagging.cc
  offline-ced-model.cc
  offline-zipformer-audio-tagging-model-config.cc
  offline-zipformer-audio-tagging-model.cc
)

list(APPEND sources
  qnn-config.cc
)

# punctuation
list(APPEND sources
  offline-ct-transformer-model.cc
  offline-punctuation-impl.cc
  offline-punctuation-model-config.cc
  offline-punctuation.cc
  online-cnn-bilstm-model.cc
  online-punctuation-impl.cc
  online-punctuation-model-config.cc
  online-punctuation.cc
)
# Diacritization
list(APPEND sources
  tashkeel-tokenizer.cc
  offline-catt-model.cc
  offline-diacritization-model-config.cc
  offline-diacritization.cc
  offline-diacritization-impl.cc
)
if(EDGEVOX_ONNX_ENABLE_RKNN)
  list(APPEND sources
    ./rknn/context-blocking-queue-rknn.cc
    ./rknn/offline-sense-voice-model-rknn.cc
    ./rknn/offline-paraformer-model-rknn.cc
    ./rknn/online-stream-rknn.cc
    ./rknn/online-transducer-greedy-search-decoder-rknn.cc
    ./rknn/online-transducer-modified-beam-search-decoder-rknn.cc
    ./rknn/online-zipformer-ctc-model-rknn.cc
    ./rknn/online-zipformer-transducer-model-rknn.cc
    ./rknn/silero-vad-model-rknn.cc
    ./rknn/transducer-keyword-decoder-rknn.cc
    ./rknn/utils.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_AXERA)
  list(APPEND sources
    ./axera/ax-engine-guard.cc
    ./axera/offline-sense-voice-model-axera.cc
    ./axera/utils.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_AXCL)
  list(APPEND sources
    ./axcl/axcl-engine-guard.cc
    ./axcl/axcl-engine-io-guard.cc
    ./axcl/axcl-engine-io-info-guard.cc
    ./axcl/axcl-manager.cc
    ./axcl/axcl-model.cc
    ./axcl/offline-sense-voice-model-axcl.cc
    ./axcl/utils.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_RKNN OR EDGEVOX_ONNX_ENABLE_ASCEND_NPU OR EDGEVOX_ONNX_ENABLE_QNN OR EDGEVOX_ONNX_ENABLE_AXERA OR EDGEVOX_ONNX_ENABLE_AXCL)
  list(APPEND sources
    ./rknn/offline-ctc-greedy-search-decoder-rknn.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_ASCEND_NPU)
  list(APPEND sources
    ./ascend/offline-paraformer-model-ascend.cc
    ./ascend/offline-sense-voice-model-ascend.cc
    ./ascend/offline-whisper-model-ascend.cc
    ./ascend/offline-zipformer-ctc-model-ascend.cc
    ./ascend/utils.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_QNN)
  list(APPEND sources
    ./qnn/offline-parakeet-ctc-model-qnn.cc
    ./qnn/offline-parakeet-tdt-model-qnn.cc
    ./qnn/offline-sense-voice-model-qnn.cc
    ./qnn/offline-paraformer-model-qnn.cc
    ./qnn/offline-whisper-model-qnn.cc
    ./qnn/offline-moonshine-model-qnn.cc
    ./qnn/offline-zipformer-transducer-model-qnn.cc
    ./qnn/offline-zipformer-ctc-model-qnn.cc
    ./qnn/online-nemo-transducer-model-qnn.cc
    ./qnn/online-zipformer-transducer-model-qnn.cc
    ./qnn/qnn-backend.cc
    ./qnn/qnn-model.cc
    ./qnn/utils.cc
  )
endif()

list(APPEND sources
  offline-funasr-nano-model-config.cc
  offline-funasr-nano-model.cc
  offline-recognizer-funasr-nano-impl.cc
  funasr-nano-tokenizer.cc
  offline-qwen3-asr-model-config.cc
  offline-qwen3-asr-model.cc
  offline-recognizer-qwen3-asr-impl.cc
  qwen-asr-tokenizer.cc
)

if(EDGEVOX_ONNX_ENABLE_TTS)
  list(APPEND sources
    character-lexicon.cc
    hifigan-vocoder.cc
    kokoro-multi-lang-lexicon.cc
    lexicon.cc
    matcha-tts-lexicon.cc
    melo-tts-lexicon.cc
    offline-tts-character-frontend.cc
    offline-tts-frontend.cc
    offline-tts-impl.cc
    offline-tts-kitten-model-config.cc
    offline-tts-kitten-model.cc
    offline-tts-kokoro-model-config.cc
    offline-tts-kokoro-model.cc
    offline-tts-matcha-model-config.cc
    offline-tts-matcha-model.cc
    offline-tts-model-config.cc
    offline-tts-pocket-model-config.cc
    offline-tts-pocket-model.cc
    offline-tts-chatterbox-model-config.cc
    offline-tts-chatterbox-model.cc
    offline-tts-supertonic-impl.cc
    offline-tts-supertonic-model-config.cc
    offline-tts-supertonic-model.cc
    offline-tts-supertonic-unicode-processor.cc
    offline-tts-vits-model-config.cc
    offline-tts-vits-model.cc
    offline-tts-zipvoice-model-config.cc
    offline-tts-zipvoice-model.cc
    offline-tts.cc
    piper-phonemize-lexicon.cc
    sentence-piece-tokenizer.cc
    vocoder.cc
    vocos-vocoder.cc
  )
endif()

list(APPEND sources
  offline-speech-denoiser-dpdfnet-model-config.cc
  offline-speech-denoiser-dpdfnet-model.cc
  offline-speech-denoiser-gtcrn-model-config.cc
  offline-speech-denoiser-gtcrn-model.cc
  offline-speech-denoiser-impl.cc
  offline-speech-denoiser-model-config.cc
  offline-speech-denoiser.cc
  online-speech-denoiser-impl.cc
  online-speech-denoiser.cc
)

if(EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION)
  list(APPEND sources
    fast-clustering-config.cc
    fast-clustering.cc
    offline-speaker-diarization-impl.cc
    offline-speaker-diarization-result.cc
    offline-speaker-diarization.cc
    offline-speaker-segmentation-model-config.cc
    offline-speaker-segmentation-pyannote-model-config.cc
    offline-speaker-segmentation-pyannote-model.cc
  )
endif()

if(APPLE)
  set_source_files_properties(
    fast-clustering.cc
    PROPERTIES
      COMPILE_OPTIONS "-Wno-unknown-pragmas"
  )
endif()

if(EDGEVOX_ONNX_ENABLE_CHECK)
  list(APPEND sources log.cc)
endif()

# Always static build
add_library(edgevox-onnx-core STATIC ${sources})


if(WIN32 AND EDGEVOX_ONNX_LINK_D3D)
    target_link_libraries(edgevox-onnx-core dxguid.lib d3d12.lib dxgi.lib dxcore.lib)
endif()


if(TARGET directml)
    target_link_libraries(edgevox-onnx-core directml)
endif()

set_target_properties(
    edgevox-onnx-core
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

if(APPLE)
  target_compile_options(edgevox-onnx-core PRIVATE
    -Wno-deprecated-declarations
  )
endif()

if(ANDROID_NDK)
  target_link_libraries(edgevox-onnx-core android log)
endif()

target_link_libraries(edgevox-onnx-core
  kaldi-native-fbank-core
  kaldi-decoder-core
  ssentencepiece_core
)
if(DEFINED OHOS AND x${OHOS} STREQUAL xOHOS)
  target_link_libraries(edgevox-onnx-core
    hilog_ndk.z
    rawfile.z
  )
endif()

if(EDGEVOX_ONNX_ENABLE_RKNN)
  if(DEFINED ENV{EDGEVOX_ONNX_RKNN_TOOLKIT2_LIB_DIR})
    target_link_libraries(edgevox-onnx-core -L$ENV{EDGEVOX_ONNX_RKNN_TOOLKIT2_LIB_DIR} -lrknnrt)
  else()
    target_link_libraries(edgevox-onnx-core rknnrt)
  endif()
endif()

if(EDGEVOX_ONNX_ENABLE_AXERA)
  if(DEFINED ENV{EDGEVOX_ONNX_AXERA_LIB_DIR})
    target_link_libraries(edgevox-onnx-core
      -L$ENV{EDGEVOX_ONNX_AXERA_LIB_DIR}
      -lax_engine
      -lax_interpreter
      -lax_sys
      -lpthread
    )
  else()
    target_link_libraries(edgevox-onnx-core
      ax_engine
      ax_interpreter
      ax_sys
      pthread
    )
  endif()
endif()

if(EDGEVOX_ONNX_ENABLE_AXCL)
  if(DEFINED ENV{EDGEVOX_ONNX_AXCL_LIB_DIR})
    target_link_libraries(edgevox-onnx-core
      -L$ENV{EDGEVOX_ONNX_AXCL_LIB_DIR}
      -laxcl_rt
      )
  else()
    target_link_libraries(edgevox-onnx-core
      axcl_rt
    )
  endif()
endif()

if(EDGEVOX_ONNX_ENABLE_ASCEND_NPU)
    target_include_directories(edgevox-onnx-core PRIVATE ${ASCEND_TOOLKIT_HOME}/include)
    target_link_libraries(edgevox-onnx-core
      -L${ASCEND_TOOLKIT_HOME}/lib64
      -lascendcl
    )
endif()

if(EDGEVOX_ONNX_ENABLE_QNN)
  target_include_directories(edgevox-onnx-core PRIVATE ${QNN_SDK_ROOT}/include/QNN)
endif()

if(EDGEVOX_ONNX_ENABLE_SPACEMIT)
  if(TARGET spacemit_ep)
    target_link_libraries(edgevox-onnx-core spacemit_ep)
  else()
    target_link_libraries(edgevox-onnx-core ${spacemit_ep_lib_files})
  endif()
endif()

if(TARGET onnxruntime)
  target_link_libraries(edgevox-onnx-core onnxruntime)
else()
  target_link_libraries(edgevox-onnx-core ${onnxruntime_lib_files})
endif()

if(NOT WIN32)
  target_link_libraries(edgevox-onnx-core -lm)
endif()

if(NOT BUILD_SHARED_LIBS AND APPLE)
  target_link_libraries(edgevox-onnx-core "-framework Foundation")
endif()

target_link_libraries(edgevox-onnx-core fstfar fst)

if(EDGEVOX_ONNX_ENABLE_TTS)
  target_link_libraries(edgevox-onnx-core
    piper_phonemize)
endif()

if(EDGEVOX_ONNX_ENABLE_CHECK)
  target_compile_definitions(edgevox-onnx-core PUBLIC EDGEVOX_ONNX_ENABLE_CHECK=1)

  if(EDGEVOX_ONNX_HAVE_EXECINFO_H)
    target_compile_definitions(edgevox-onnx-core PRIVATE EDGEVOX_ONNX_HAVE_EXECINFO_H=1)
  endif()

  if(EDGEVOX_ONNX_HAVE_CXXABI_H)
    target_compile_definitions(edgevox-onnx-core PRIVATE EDGEVOX_ONNX_HAVE_CXXABI_H=1)
  endif()
endif()

if(NOT BUILD_SHARED_LIBS AND CMAKE_SYSTEM_NAME STREQUAL Linux)
  # This is for linux arm32 and arm64
  target_link_libraries(edgevox-onnx-core -ldl)
endif()

if(NOT WIN32 AND NOT EDGEVOX_ONNX_ENABLE_WASM AND CMAKE_SYSTEM_NAME STREQUAL Linux)
  target_link_libraries(edgevox-onnx-core -pthread)
endif()

if(EDGEVOX_ONNX_ENABLE_BINARY)
  add_executable(edgevox-onnx edgevox-onnx.cc)
  add_executable(edgevox-onnx-keyword-spotter edgevox-onnx-keyword-spotter.cc)
  add_executable(edgevox-onnx-offline edgevox-onnx-offline.cc)
  add_executable(edgevox-onnx-offline-audio-tagging edgevox-onnx-offline-audio-tagging.cc)
  add_executable(edgevox-onnx-offline-denoiser edgevox-onnx-offline-denoiser.cc)
  add_executable(edgevox-onnx-offline-diacritization edgevox-onnx-offline-diacritization.cc)
  add_executable(edgevox-onnx-offline-language-identification edgevox-onnx-offline-language-identification.cc)
  add_executable(edgevox-onnx-offline-parallel edgevox-onnx-offline-parallel.cc)
  add_executable(edgevox-onnx-offline-punctuation edgevox-onnx-offline-punctuation.cc)
  add_executable(edgevox-onnx-offline-source-separation edgevox-onnx-offline-source-separation.cc)
  add_executable(edgevox-onnx-online-denoiser edgevox-onnx-online-denoiser.cc)
  add_executable(edgevox-onnx-online-punctuation edgevox-onnx-online-punctuation.cc)
  add_executable(edgevox-onnx-version edgevox-onnx-version.cc version.cc)
  add_executable(edgevox-onnx-vad edgevox-onnx-vad.cc)

  add_executable(edgevox-onnx-vad-with-offline-asr edgevox-onnx-vad-with-offline-asr.cc)
  add_executable(edgevox-onnx-vad-with-online-asr edgevox-onnx-vad-with-online-asr.cc)

  if(EDGEVOX_ONNX_ENABLE_TTS)
    add_executable(edgevox-onnx-offline-tts edgevox-onnx-offline-tts.cc)
  endif()

  if(EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION)
    add_executable(edgevox-onnx-offline-speaker-diarization edgevox-onnx-offline-speaker-diarization.cc)
  endif()

  set(main_exes
    edgevox-onnx
    edgevox-onnx-keyword-spotter
    edgevox-onnx-offline
    edgevox-onnx-offline-audio-tagging
    edgevox-onnx-offline-denoiser
    edgevox-onnx-offline-diacritization
    edgevox-onnx-offline-language-identification
    edgevox-onnx-offline-parallel
    edgevox-onnx-offline-punctuation
    edgevox-onnx-offline-source-separation
    edgevox-onnx-online-denoiser
    edgevox-onnx-online-punctuation
    edgevox-onnx-vad
    edgevox-onnx-vad-with-offline-asr
    edgevox-onnx-vad-with-online-asr
  )
  if(EDGEVOX_ONNX_ENABLE_TTS)
    list(APPEND main_exes
      edgevox-onnx-offline-tts
    )
  endif()

  if(EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION)
    list(APPEND main_exes
      edgevox-onnx-offline-speaker-diarization
    )
  endif()

  foreach(exe IN LISTS main_exes)
    target_link_libraries(${exe} edgevox-onnx-core)
  endforeach()

  if(UNIX)
    foreach(exe IN LISTS main_exes)
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")

      if(EDGEVOX_ONNX_ENABLE_PYTHON)
        target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
      elseif(EDGEVOX_ONNX_SPLIT_PYTHON_PACKAGE)
        foreach(ver in ITEMS 3.8 3.9 3.10 3.11 3.12 3.13 3.14)
          target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
        endforeach()
      endif()
    endforeach()
  endif()
endif()

if(NOT BUILD_SHARED_LIBS)
  install(TARGETS edgevox-onnx-core DESTINATION lib)
endif()

if(EDGEVOX_ONNX_ENABLE_BINARY)
  install(
    TARGETS
      ${main_exes}
      edgevox-onnx-version
    DESTINATION
      bin
  )
endif()

if(EDGEVOX_ONNX_HAS_ALSA AND EDGEVOX_ONNX_ENABLE_BINARY)
  add_executable(edgevox-onnx-alsa edgevox-onnx-alsa.cc alsa.cc)
  add_executable(edgevox-onnx-alsa-offline edgevox-onnx-alsa-offline.cc alsa.cc)
  add_executable(edgevox-onnx-alsa-offline-audio-tagging edgevox-onnx-alsa-offline-audio-tagging.cc alsa.cc)
  add_executable(edgevox-onnx-alsa-offline-speaker-identification edgevox-onnx-alsa-offline-speaker-identification.cc alsa.cc)
  add_executable(edgevox-onnx-keyword-spotter-alsa edgevox-onnx-keyword-spotter-alsa.cc alsa.cc)
  add_executable(edgevox-onnx-vad-alsa edgevox-onnx-vad-alsa.cc alsa.cc)
  add_executable(edgevox-onnx-vad-alsa-offline-asr edgevox-onnx-vad-alsa-offline-asr.cc alsa.cc)

  if(EDGEVOX_ONNX_ENABLE_TTS)
    add_executable(edgevox-onnx-offline-tts-play-alsa edgevox-onnx-offline-tts-play-alsa.cc alsa-play.cc)
  endif()

  set(exes
    edgevox-onnx-alsa
    edgevox-onnx-alsa-offline
    edgevox-onnx-alsa-offline-speaker-identification
    edgevox-onnx-keyword-spotter-alsa
    edgevox-onnx-vad-alsa
    edgevox-onnx-vad-alsa-offline-asr
    edgevox-onnx-alsa-offline-audio-tagging
  )

  if(EDGEVOX_ONNX_ENABLE_TTS)
    list(APPEND exes
      edgevox-onnx-offline-tts-play-alsa
    )
  endif()

  #   # To fix the following error for Windows when building exe
  #   #  mismatch detected for 'RuntimeLibrary': value 'MT_StaticRelease' doesn't match value 'MD_Dynamic Release'

  foreach(exe IN LISTS exes)
    target_link_libraries(${exe} edgevox-onnx-core)
  endforeach()

  foreach(exe IN LISTS exes)
    if(DEFINED ENV{EDGEVOX_ONNX_ALSA_LIB_DIR})
      target_link_libraries(${exe} -L$ENV{EDGEVOX_ONNX_ALSA_LIB_DIR} -lasound)
    else()
      target_link_libraries(${exe} asound)
    endif()
  endforeach()

  if(UNIX)
    foreach(exe IN LISTS exes)
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")
    endforeach()

    if(EDGEVOX_ONNX_ENABLE_PYTHON)
      foreach(exe IN LISTS exes)
        target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
      endforeach()
    elseif(EDGEVOX_ONNX_SPLIT_PYTHON_PACKAGE)
      foreach(exe IN LISTS exes)
        foreach(ver in ITEMS 3.8 3.9 3.10 3.11 3.12 3.13 3.14)
          target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
        endforeach()
      endforeach()
    endif()
  endif()

  install(
    TARGETS ${exes}
    DESTINATION
      bin
  )
endif()

if(EDGEVOX_ONNX_ENABLE_PORTAUDIO AND EDGEVOX_ONNX_ENABLE_BINARY)
  if(EDGEVOX_ONNX_ENABLE_TTS)
    add_executable(edgevox-onnx-offline-tts-play
      edgevox-onnx-offline-tts-play.cc
      microphone.cc
    )
  endif()

  add_executable(edgevox-onnx-keyword-spotter-microphone
    edgevox-onnx-keyword-spotter-microphone.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-microphone
    edgevox-onnx-microphone.cc
    microphone.cc
  )


  add_executable(edgevox-onnx-microphone-offline
    edgevox-onnx-microphone-offline.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-vad-microphone
    edgevox-onnx-vad-microphone.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-vad-microphone-simulated-streaming-asr
    edgevox-onnx-vad-microphone-simulated-streaming-asr.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-vad-microphone-offline-asr
    edgevox-onnx-vad-microphone-offline-asr.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-microphone-offline-speaker-identification
    edgevox-onnx-microphone-offline-speaker-identification.cc
    microphone.cc
  )

  add_executable(edgevox-onnx-microphone-offline-audio-tagging
    edgevox-onnx-microphone-offline-audio-tagging.cc
    microphone.cc
  )

  set(exes
    edgevox-onnx-keyword-spotter-microphone
    edgevox-onnx-microphone
    edgevox-onnx-microphone-offline
    edgevox-onnx-microphone-offline-audio-tagging
    edgevox-onnx-microphone-offline-speaker-identification
    edgevox-onnx-vad-microphone
    edgevox-onnx-vad-microphone-simulated-streaming-asr
    edgevox-onnx-vad-microphone-offline-asr
  )

  if(EDGEVOX_ONNX_ENABLE_TTS)
    list(APPEND exes
      edgevox-onnx-offline-tts-play
    )
  endif()

  foreach(exe IN LISTS exes)
    target_link_libraries(${exe} portaudio_static edgevox-onnx-core)
  endforeach()

  if(UNIX)
    foreach(exe IN LISTS exes)
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
      target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")
    endforeach()

    if(EDGEVOX_ONNX_ENABLE_PYTHON)
      foreach(exe IN LISTS exes)
        target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
      endforeach()
    elseif(EDGEVOX_ONNX_SPLIT_PYTHON_PACKAGE)
      foreach(exe IN LISTS exes)
        foreach(ver in ITEMS 3.8 3.9 3.10 3.11 3.12 3.13 3.14)
          target_link_libraries(${exe} "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
        endforeach()
      endforeach()
    endif()
  endif()

  install(
    TARGETS ${exes}
    DESTINATION
      bin
  )
endif()

if(EDGEVOX_ONNX_ENABLE_WEBSOCKET AND EDGEVOX_ONNX_ENABLE_BINARY)
  add_definitions(-DASIO_STANDALONE)
  add_definitions(-D_WEBSOCKETPP_CPP11_STL_)

  add_executable(edgevox-onnx-online-websocket-server
    online-websocket-server-impl.cc
    online-websocket-server.cc
  )
  target_link_libraries(edgevox-onnx-online-websocket-server edgevox-onnx-core)

  add_executable(edgevox-onnx-online-websocket-client
    online-websocket-client.cc
  )
  target_link_libraries(edgevox-onnx-online-websocket-client edgevox-onnx-core)

  if(UNIX)
    target_compile_options(edgevox-onnx-online-websocket-server PRIVATE -Wno-deprecated-declarations)

    target_compile_options(edgevox-onnx-online-websocket-client PRIVATE -Wno-deprecated-declarations)
  endif()

  # For offline websocket
  add_executable(edgevox-onnx-offline-websocket-server
    offline-websocket-server-impl.cc
    offline-websocket-server.cc
  )
  target_link_libraries(edgevox-onnx-offline-websocket-server edgevox-onnx-core)

  if(UNIX)
    target_compile_options(edgevox-onnx-offline-websocket-server PRIVATE -Wno-deprecated-declarations)
  endif()

  if(UNIX)
    target_link_libraries(edgevox-onnx-online-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
    target_link_libraries(edgevox-onnx-online-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")

    target_link_libraries(edgevox-onnx-online-websocket-client "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
    target_link_libraries(edgevox-onnx-online-websocket-client "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")

    target_link_libraries(edgevox-onnx-offline-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib")
    target_link_libraries(edgevox-onnx-offline-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../../../edgevox_onnx/lib")

    if(EDGEVOX_ONNX_ENABLE_PYTHON)
      target_link_libraries(edgevox-onnx-online-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
      target_link_libraries(edgevox-onnx-online-websocket-client "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
      target_link_libraries(edgevox-onnx-offline-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${PYTHON_VERSION}/site-packages/edgevox_onnx/lib")
    elseif(EDGEVOX_ONNX_SPLIT_PYTHON_PACKAGE)
        foreach(ver in ITEMS 3.8 3.9 3.10 3.11 3.12 3.13 3.14)
          target_link_libraries(edgevox-onnx-online-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
          target_link_libraries(edgevox-onnx-online-websocket-client "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
          target_link_libraries(edgevox-onnx-offline-websocket-server "-Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}/../lib/python${ver}/site-packages/edgevox_onnx/lib")
        endforeach()
    endif()
  endif()

  install(
    TARGETS
      edgevox-onnx-online-websocket-server
      edgevox-onnx-online-websocket-client
      edgevox-onnx-offline-websocket-server
    DESTINATION
      bin
  )
endif()

if(EDGEVOX_ONNX_ENABLE_TESTS)
  set(edgevox_onnx_test_srcs
    cat-test.cc
    circular-buffer-test.cc
    context-graph-test.cc
    math-test.cc
    offline-whisper-timestamp-rules-test.cc
    packed-sequence-test.cc
    pad-sequence-test.cc
    regex-lang-test.cc
    slice-test.cc
    stack-test.cc
    text-utils-test.cc
    text2token-test.cc
    transpose-test.cc
    unbind-test.cc
    utfcpp-test.cc
    wave-reader-test.cc
  )
  if(EDGEVOX_ONNX_ENABLE_TTS)
    list(APPEND edgevox_onnx_test_srcs
      sentence-piece-tokenizer-test.cc
      piper-phonemize-test.cc
      offline-tts-supertonic-unicode-processor-test.cc
      chatterbox-tokenizer-test.cc
    )
  endif()

  if(EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION)
    list(APPEND edgevox_onnx_test_srcs
      fast-clustering-test.cc
    )
  endif()

  list(APPEND edgevox_onnx_test_srcs
    speaker-embedding-manager-test.cc
  )

  function(edgevox_onnx_add_test source)
    get_filename_component(name ${source} NAME_WE)
    set(target_name ${name})
    add_executable(${target_name} "${source}")

    target_link_libraries(${target_name}
      PRIVATE
        gtest
        gtest_main
        edgevox-onnx-core
    )

    add_test(NAME "${target_name}"
      COMMAND
        $<TARGET_FILE:${target_name}>
    )
  endfunction()

  foreach(source IN LISTS edgevox_onnx_test_srcs)
    edgevox_onnx_add_test(${source})
  endforeach()
endif()

set(srcs_to_check)
foreach(s IN LISTS sources)
  list(APPEND srcs_to_check ${CMAKE_CURRENT_LIST_DIR}/${s})
endforeach()

# For clang-tidy
add_custom_target(
  clang-tidy-check
  clang-tidy -p ${CMAKE_BINARY_DIR}/compile_commands.json --config-file ${PROJECT_SOURCE_DIR}/.clang-tidy ${srcs_to_check}
  DEPENDS ${sources})

add_custom_target(check DEPENDS clang-tidy-check)
