if(NOT $ENV{EDGEVOX_ONNX_IS_USING_BUILD_WASM_SH})
  message(FATAL_ERROR "Please use ./build-wasm-simd-nodejs.sh to build for wasm NodeJS")
endif()

set(exported_functions
  #tts
  PrintOfflineTtsConfig
  EdgevoxOnnxCreateOfflineTts
  EdgevoxOnnxDestroyOfflineTts
  EdgevoxOnnxDestroyOfflineTtsGeneratedAudio
  EdgevoxOnnxOfflineTtsGenerate
  EdgevoxOnnxOfflineTtsGenerateWithCallback
  EdgevoxOnnxOfflineTtsGenerateWithConfig
  EdgevoxOnnxOfflineTtsNumSpeakers
  EdgevoxOnnxOfflineTtsSampleRate
  EdgevoxOnnxWriteWave
  # streaming asr
  EdgevoxOnnxCreateOnlineRecognizer
  EdgevoxOnnxCreateOnlineStream
  EdgevoxOnnxDecodeOnlineStream
  EdgevoxOnnxDestroyOnlineRecognizer
  EdgevoxOnnxDestroyOnlineRecognizerResult
  EdgevoxOnnxDestroyOnlineStream
  EdgevoxOnnxDestroyOnlineStreamResultJson
  EdgevoxOnnxGetOnlineStreamResult
  EdgevoxOnnxGetOnlineStreamResultAsJson
  EdgevoxOnnxIsOnlineStreamReady
  EdgevoxOnnxOnlineStreamAcceptWaveform
  EdgevoxOnnxOnlineStreamGetOption
  EdgevoxOnnxOnlineStreamInputFinished
  EdgevoxOnnxOnlineStreamIsEndpoint
  EdgevoxOnnxOnlineStreamReset
  EdgevoxOnnxOnlineStreamSetOption
  # non-streaming ASR
  PrintOfflineRecognizerConfig
  EdgevoxOnnxAcceptWaveformOffline
  EdgevoxOnnxCreateOfflineRecognizer
  EdgevoxOnnxCreateOfflineStream
  EdgevoxOnnxDecodeMultipleOfflineStreams
  EdgevoxOnnxDecodeOfflineStream
  EdgevoxOnnxDestroyOfflineRecognizer
  EdgevoxOnnxDestroyOfflineRecognizerResult
  EdgevoxOnnxDestroyOfflineStream
  EdgevoxOnnxDestroyOfflineStreamResultJson
  EdgevoxOnnxGetOfflineStreamResult
  EdgevoxOnnxGetOfflineStreamResultAsJson
  EdgevoxOnnxOfflineStreamGetOption
  EdgevoxOnnxOfflineStreamSetOption
  EdgevoxOnnxOfflineRecognizerSetConfig
  # online kws
  EdgevoxOnnxCreateKeywordSpotter
  EdgevoxOnnxCreateKeywordStream
  EdgevoxOnnxDecodeKeywordStream
  EdgevoxOnnxDestroyKeywordResult
  EdgevoxOnnxDestroyKeywordSpotter
  EdgevoxOnnxGetKeywordResult
  EdgevoxOnnxIsKeywordStreamReady
  EdgevoxOnnxResetKeywordStream
  # VAD
  EdgevoxOnnxCreateCircularBuffer
  EdgevoxOnnxDestroyCircularBuffer
  EdgevoxOnnxCircularBufferPush
  EdgevoxOnnxCircularBufferGet
  EdgevoxOnnxCircularBufferFree
  EdgevoxOnnxCircularBufferPop
  EdgevoxOnnxCircularBufferSize
  EdgevoxOnnxCircularBufferHead
  EdgevoxOnnxCircularBufferReset
  EdgevoxOnnxCreateVoiceActivityDetector
  EdgevoxOnnxDestroyVoiceActivityDetector
  EdgevoxOnnxVoiceActivityDetectorAcceptWaveform
  EdgevoxOnnxVoiceActivityDetectorEmpty
  EdgevoxOnnxVoiceActivityDetectorDetected
  EdgevoxOnnxVoiceActivityDetectorPop
  EdgevoxOnnxVoiceActivityDetectorClear
  EdgevoxOnnxVoiceActivityDetectorFront
  EdgevoxOnnxDestroySpeechSegment
  EdgevoxOnnxVoiceActivityDetectorReset
  EdgevoxOnnxVoiceActivityDetectorFlush
  # Speaker diarization
  EdgevoxOnnxCreateOfflineSpeakerDiarization
  EdgevoxOnnxDestroyOfflineSpeakerDiarization
  EdgevoxOnnxOfflineSpeakerDiarizationDestroyResult
  EdgevoxOnnxOfflineSpeakerDiarizationDestroySegment
  EdgevoxOnnxOfflineSpeakerDiarizationGetSampleRate
  EdgevoxOnnxOfflineSpeakerDiarizationProcess
  EdgevoxOnnxOfflineSpeakerDiarizationProcessWithCallback
  EdgevoxOnnxOfflineSpeakerDiarizationResultGetNumSegments
  EdgevoxOnnxOfflineSpeakerDiarizationResultSortByStartTime
  EdgevoxOnnxOfflineSpeakerDiarizationSetConfig
  #
  EdgevoxOnnxFileExists
  EdgevoxOnnxReadWave
  EdgevoxOnnxReadWaveFromBinaryData
  EdgevoxOnnxFreeWave
  EdgevoxOnnxWriteWave
  # speech enhancement
  EdgevoxOnnxCreateOfflineSpeechDenoiser
  EdgevoxOnnxCreateOnlineSpeechDenoiser
  EdgevoxOnnxDestroyDenoisedAudio
  EdgevoxOnnxDestroyOfflineSpeechDenoiser
  EdgevoxOnnxDestroyOnlineSpeechDenoiser
  EdgevoxOnnxOfflineSpeechDenoiserGetSampleRate
  EdgevoxOnnxOfflineSpeechDenoiserRun
  EdgevoxOnnxOnlineSpeechDenoiserGetFrameShiftInSamples
  EdgevoxOnnxOnlineSpeechDenoiserGetSampleRate
  EdgevoxOnnxOnlineSpeechDenoiserRun
  EdgevoxOnnxOnlineSpeechDenoiserFlush
  EdgevoxOnnxOnlineSpeechDenoiserReset
  # punctuation
  EdgevoxOnnxCreateOfflinePunctuation
  EdgevoxOnnxDestroyOfflinePunctuation
  SherpaOfflinePunctuationAddPunct
  SherpaOfflinePunctuationFreeText
  EdgevoxOnnxCreateOnlinePunctuation
  EdgevoxOnnxDestroyOnlinePunctuation
  EdgevoxOnnxOnlinePunctuationAddPunct
  EdgevoxOnnxOnlinePunctuationFreeText
  # version
  EdgevoxOnnxGetGitDate
  EdgevoxOnnxGetGitSha1
  EdgevoxOnnxGetVersionStr
)


set(mangled_exported_functions)
foreach(x IN LISTS exported_functions)
  list(APPEND mangled_exported_functions "_${x}")
endforeach()
list(JOIN mangled_exported_functions "," all_exported_functions)

include_directories(${CMAKE_SOURCE_DIR})
set(MY_FLAGS " -s FORCE_FILESYSTEM=1 -s INITIAL_MEMORY=512MB -s ALLOW_MEMORY_GROWTH=1")
string(APPEND MY_FLAGS " -sSTACK_SIZE=10485760 ") # 10MB
string(APPEND MY_FLAGS " -sALLOW_TABLE_GROWTH ")
string(APPEND MY_FLAGS " -sEXPORTED_FUNCTIONS=[_CopyHeap,_malloc,_free,${all_exported_functions}] ")
string(APPEND MY_FLAGS " -sNODERAWFS=1 ")
string(APPEND MY_FLAGS " -sEXPORTED_RUNTIME_METHODS=['ccall','stringToUTF8','setValue','getValue','lengthBytesUTF8','UTF8ToString','HEAPU8','HEAP16','HEAP32','HEAPU32','HEAPF32','HEAPF64','addFunction','removeFunction'] ")

string(APPEND MY_FLAGS " -sMODULARIZE=1 -sWASM_ASYNC_COMPILATION=0 ")
string(APPEND MY_FLAGS " -fexceptions ")
string(APPEND MY_FLAGS " -sPTHREAD_POOL_SIZE=4 ")

message(STATUS "MY_FLAGS: ${MY_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MY_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_FLAGS}")
set(CMAKE_EXECUTABLE_LINKER_FLAGS "${CMAKE_EXECUTABLE_LINKER_FLAGS} ${MY_FLAGS}")

add_executable(edgevox-onnx-wasm-nodejs edgevox-onnx-wasm-nodejs.cc)
target_link_libraries(edgevox-onnx-wasm-nodejs edgevox-onnx-core edgevox-onnx-c-api)
install(TARGETS edgevox-onnx-wasm-nodejs DESTINATION bin/wasm/nodejs)

install(
  FILES
  ${CMAKE_SOURCE_DIR}/wasm/asr/edgevox-onnx-asr.js
  ${CMAKE_SOURCE_DIR}/wasm/tts/edgevox-onnx-tts.node.js
  ${CMAKE_SOURCE_DIR}/wasm/kws/edgevox-onnx-kws.js
  ${CMAKE_SOURCE_DIR}/wasm/vad/edgevox-onnx-vad.js
  ${CMAKE_SOURCE_DIR}/wasm/nodejs/edgevox-onnx-punctuation.js
  ${CMAKE_SOURCE_DIR}/wasm/speaker-diarization/edgevox-onnx-speaker-diarization.js
  ${CMAKE_SOURCE_DIR}/wasm/speech-enhancement/edgevox-onnx-speech-enhancement.js
  ${CMAKE_SOURCE_DIR}/wasm/nodejs/edgevox-onnx-wave.js
    "$<TARGET_FILE_DIR:edgevox-onnx-wasm-nodejs>/edgevox-onnx-wasm-nodejs.js"
    "$<TARGET_FILE_DIR:edgevox-onnx-wasm-nodejs>/edgevox-onnx-wasm-nodejs.wasm"
  DESTINATION
    bin/wasm/nodejs
)
