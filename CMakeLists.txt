cmake_minimum_required(VERSION 3.19)
project(yippy VERSION 0.3)
add_definitions(-DVERSION=${CMAKE_PROJECT_VERSION})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -lm")

if(UNIX)
  message("Building ${CMAKE_PROJECT_NAME} v${CMAKE_PROJECT_VERSION} for ${CMAKE_SYSTEM_NAME}.")
  add_definitions(-DOS="${CMAKE_SYSTEM_NAME}" -DWIN32)
elseif(WIN32)
  message("Building for Windows.")
  add_definitions(-DOS="windows" -DWIN32)
endif()

file(GLOB SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
