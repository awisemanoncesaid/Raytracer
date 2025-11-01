cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

add_library(Math STATIC)

target_sources(Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Math PUBLIC glm)
