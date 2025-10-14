# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Raytracer)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    Main.cpp
)

include_directories(${CMAKE_SOURCE_DIR})

# Create executable
add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer PUBLIC Math)
target_link_libraries(raytracer PUBLIC Signal)
target_link_libraries(raytracer PUBLIC Gui)
target_link_libraries(raytracer PUBLIC FrameBuffer)
target_link_libraries(raytracer PUBLIC CommandLineParser)
target_link_libraries(raytracer PUBLIC pthread)
