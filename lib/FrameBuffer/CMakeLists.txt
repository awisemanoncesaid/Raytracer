cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(libpng URL https://github.com/glennrp/libpng/archive/refs/tags/v1.6.37.zip)
FetchContent_MakeAvailable(libpng)

FetchContent_Declare(zlib URL https://github.com/madler/zlib/archive/refs/tags/v1.3.1.zip)
FetchContent_MakeAvailable(zlib)

# Set the source files for the library
set(LIBRARY_SOURCES
    FrameBuffer.cpp
)

# Create the library
add_library(FrameBuffer STATIC ${LIBRARY_SOURCES})
set_target_properties(FrameBuffer PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(FrameBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(FrameBuffer
    PRIVATE
        png
        z
    PUBLIC
        Math
)
