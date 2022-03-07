cmake_minimum_required(VERSION 3.16.3)
project(yippy VERSION 0.5)
add_definitions(-DVERSION=${CMAKE_PROJECT_VERSION})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -lm")

if ($ENV{OS} STREQUAL "wasm")
  message($ENV{OS})
  message($ENV{WASI_SDK_PATH})
  set (WASI_PATH $ENV{WASI_SDK_PATH})
  set (CMAKE_C_COMPILER "/usr/bin/clang")
  message("wasi path is ${WASI_PATH}")
  set(CMAKE_C_FLAGS "--target=wasm32-wasi --sysroot=${WASI_PATH}/share/wasi-sysroot -Wall -lm")
endif()

add_definitions(-DCC=${CMAKE_C_COMPILER} -DCCVERSION=${CMAKE_C_COMPILER_VERSION} -DCCARCH=${CMAKE_C_COMPILER_ARCHITECTURE_ID})

if(UNIX)
  add_definitions(-DOS="${CMAKE_SYSTEM_NAME}")
elseif(WIN32)
  message("Building for Windows.")
  add_definitions(-DOS="windows" -DWIN32)
endif()

file(GLOB SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
