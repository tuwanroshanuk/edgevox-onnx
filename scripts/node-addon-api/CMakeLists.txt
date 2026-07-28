# See also https://github.com/cmake-js/cmake-js
# npm install cmake-js
# ./node_modules/.bin/cmake-js --help
# ./node_modules/.bin/cmake-js --version
# ./node_modules/.bin/cmake-js compile --help
# ./node_modules/.bin/cmake-js compile --log-level
# ./node_modules/.bin/cmake-js compile --log-level verbose
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(edgevox-onnx)

set(CMAKE_CXX_STANDARD 17)

if(NOT WIN32)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(BUILD_RPATH_USE_ORIGIN TRUE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

if(NOT APPLE)
  set(EDGEVOX_ONNX_RPATH_ORIGIN "$ORIGIN")
else()
  set(EDGEVOX_ONNX_RPATH_ORIGIN "@loader_path")
endif()

if(NOT WIN32)
  set(CMAKE_INSTALL_RPATH ${EDGEVOX_ONNX_RPATH_ORIGIN})
  set(CMAKE_BUILD_RPATH ${EDGEVOX_ONNX_RPATH_ORIGIN})
endif()

include_directories(${CMAKE_JS_INC})

set(addon_src_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(WIN32)
  # Git checkouts on Windows commonly materialize the source symlinks in
  # scripts/node-addon-api/src as small text files. Compile their real targets.
  set(addon_src_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/../../harmony-os/EdgevoxOnnxHar/edgevox_onnx/src/main/cpp")
endif()

set(srcs
  ${addon_src_dir}/audio-tagging.cc
  ${addon_src_dir}/keyword-spotting.cc
  ${addon_src_dir}/non-streaming-asr.cc
  ${addon_src_dir}/non-streaming-speaker-diarization.cc
  ${addon_src_dir}/non-streaming-speech-denoiser.cc
  ${addon_src_dir}/non-streaming-tts.cc
  ${addon_src_dir}/offline-punctuation.cc
  ${addon_src_dir}/online-punctuation.cc
  ${addon_src_dir}/streaming-speech-denoiser.cc
  ${addon_src_dir}/edgevox-onnx-node-addon-api.cc
  ${addon_src_dir}/speaker-identification.cc
  ${addon_src_dir}/spoken-language-identification.cc
  ${addon_src_dir}/streaming-asr.cc
  ${addon_src_dir}/vad.cc
  ${addon_src_dir}/version.cc
  ${addon_src_dir}/wave-reader.cc
  ${addon_src_dir}/wave-writer.cc
  ${addon_src_dir}/resampler.cc
)
include_directories(${addon_src_dir})

if(NOT DEFINED ENV{EDGEVOX_ONNX_INSTALL_DIR})
  message(FATAL_ERROR "
Please run:
git clone https://github.com/k2-fsa/sherpa-onnx
cd edgevox-onnx
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=./install ..
make install
export EDGEVOX_ONNX_INSTALL_DIR=$PWD/install
  ")
endif()

include_directories($ENV{EDGEVOX_ONNX_INSTALL_DIR}/include)

# See https://nodejs.github.io/node-addon-examples/build-tools/cmake-js
# Include Node-API wrappers
execute_process(
  COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
)

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
include_directories(${NODE_ADDON_API_DIR})

link_directories($ENV{EDGEVOX_ONNX_INSTALL_DIR}/lib)

add_library(${PROJECT_NAME} SHARED ${srcs} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

target_link_libraries(${PROJECT_NAME}
  edgevox-onnx-c-api
  onnxruntime
)
if(NOT WIN32)
  target_link_options(${PROJECT_NAME} PRIVATE
    -Wl,-rpath,$ENV{EDGEVOX_ONNX_INSTALL_DIR}/lib
    -Wl,-rpath,${EDGEVOX_ONNX_RPATH_ORIGIN}
  )
endif()

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()
