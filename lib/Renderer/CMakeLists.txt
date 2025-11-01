cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_SOURCES
    Renderer.cpp
)

add_library(Renderer STATIC ${LIBRARY_SOURCES})

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Renderer PUBLIC Scene3d)
target_link_libraries(Renderer PUBLIC Math)
target_link_libraries(Renderer PUBLIC FrameBuffer)
if (UNIX)
    target_link_libraries(Renderer PRIVATE pthread)
endif()
