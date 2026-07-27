add_subdirectory(csrc)
if(EDGEVOX_ONNX_ENABLE_PYTHON)
  add_subdirectory(python)
endif()

if(EDGEVOX_ONNX_ENABLE_JNI)
  add_subdirectory(jni)
endif()

if(EDGEVOX_ONNX_ENABLE_C_API)
  add_subdirectory(c-api)
endif()
