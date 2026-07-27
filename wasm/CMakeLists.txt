if(EDGEVOX_ONNX_ENABLE_WASM_TTS)
  add_subdirectory(tts)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_ASR)
  add_subdirectory(asr)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_KWS)
  add_subdirectory(kws)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_VAD)
  add_subdirectory(vad)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_VAD_ASR)
  add_subdirectory(vad-asr)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_SPEECH_ENHANCEMENT)
  add_subdirectory(speech-enhancement)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_SPEAKER_DIARIZATION)
  add_subdirectory(speaker-diarization)
endif()

if(EDGEVOX_ONNX_ENABLE_WASM_NODEJS)
  add_subdirectory(nodejs)
endif()
