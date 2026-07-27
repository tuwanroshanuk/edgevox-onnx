add_subdirectory(csrc)

if(EDGEVOX_ONNX_ENABLE_TESTS)
  add_subdirectory(tests)
endif()
