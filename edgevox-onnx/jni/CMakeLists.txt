include_directories(${PROJECT_SOURCE_DIR})

if(NOT DEFINED ANDROID_ABI)
  if(NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "Please set the environment variable JAVA_HOME")
  endif()
  include_directories($ENV{JAVA_HOME}/include)
  include_directories($ENV{JAVA_HOME}/include/linux)
  include_directories($ENV{JAVA_HOME}/include/darwin)
  include_directories($ENV{JAVA_HOME}/include/win32)
endif()

set(sources
  audio-tagging.cc
  common.cc
  jni.cc
  keyword-spotter.cc
  offline-diacritization.cc
  offline-punctuation.cc
  offline-recognizer.cc
  offline-speech-denoiser.cc
  offline-stream.cc
  online-speech-denoiser.cc
  online-punctuation.cc
  online-recognizer.cc
  online-stream.cc
  speaker-embedding-extractor.cc
  speaker-embedding-manager.cc
  speech-denoiser.cc
  spoken-language-identification.cc
  version.cc
  voice-activity-detector.cc
  wave-reader.cc
  wave-writer.cc
)

if(EDGEVOX_ONNX_ENABLE_TTS)
  list(APPEND sources
    offline-tts.cc
  )
endif()

if(EDGEVOX_ONNX_ENABLE_SPEAKER_DIARIZATION)
  list(APPEND sources
    offline-speaker-diarization.cc
  )
endif()

add_library(edgevox-onnx-jni SHARED ${sources})

target_compile_definitions(edgevox-onnx-jni PRIVATE EDGEVOX_ONNX_BUILD_SHARED_LIBS=1)
target_compile_definitions(edgevox-onnx-jni PRIVATE EDGEVOX_ONNX_BUILD_MAIN_LIB=1)

if(ANDROID OR (UNIX AND NOT APPLE))
  set_target_properties(edgevox-onnx-jni PROPERTIES
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/edgevox-onnx-symbols.lds"
  )
elseif(APPLE)
  set_target_properties(edgevox-onnx-jni PROPERTIES
    LINK_FLAGS "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/edgevox-onnx-symbols.exp"
  )
endif()

target_link_libraries(edgevox-onnx-jni edgevox-onnx-core)
install(TARGETS edgevox-onnx-jni DESTINATION lib)
