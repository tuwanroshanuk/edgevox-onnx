include_directories(${PROJECT_SOURCE_DIR})
add_library(edgevox-onnx-c-api c-api.cc)
target_link_libraries(edgevox-onnx-c-api edgevox-onnx-core)
target_include_directories(edgevox-onnx-c-api PUBLIC ${PROJECT_SOURCE_DIR})

if(BUILD_SHARED_LIBS)
  target_compile_definitions(edgevox-onnx-c-api PUBLIC EDGEVOX_ONNX_BUILD_SHARED_LIBS=1)
  target_compile_definitions(edgevox-onnx-c-api PUBLIC EDGEVOX_ONNX_BUILD_MAIN_LIB=1)
endif()

add_library(edgevox-onnx-cxx-api cxx-api.cc)
target_link_libraries(edgevox-onnx-cxx-api edgevox-onnx-c-api)
target_include_directories(edgevox-onnx-cxx-api PUBLIC ${PROJECT_SOURCE_DIR})

if(ANDROID OR (UNIX AND NOT APPLE))
  set_target_properties(edgevox-onnx-c-api PROPERTIES
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/edgevox-onnx-symbols-c.lds"
  )
elseif(APPLE)
  set_target_properties(edgevox-onnx-c-api PROPERTIES
    LINK_FLAGS "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/edgevox-onnx-symbols-c.exp"
  )
endif()

install(
  TARGETS
    edgevox-onnx-c-api
    edgevox-onnx-cxx-api
  DESTINATION
    lib
)

install(
  FILES
    c-api.h
    cxx-api.h
  DESTINATION
    include/edgevox-onnx/c-api
)
