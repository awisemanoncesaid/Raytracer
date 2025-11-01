# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Raytracer)

# Deps
include(FetchContent)

FetchContent_Declare(
    CommandLineParser
    GIT_REPOSITORY https://github.com/awisemanoncesaid/command-line-parser.git
    GIT_TAG main
)
FetchContent_MakeAvailable(CommandLineParser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/Main.cpp
)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(lib/Math)
add_subdirectory(lib/FrameBuffer)
add_subdirectory(lib/Scene3d)
add_subdirectory(lib/Renderer)

add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer PRIVATE
    CommandLineParser
    FrameBuffer
    Renderer
    Scene3d
    Math
)
