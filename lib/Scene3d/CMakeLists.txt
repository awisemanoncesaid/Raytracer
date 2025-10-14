cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.10.2)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(cpputils GIT_REPOSITORY https://github.com/awisemanoncesaid/cpputils.git GIT_TAG v1.0.0)
FetchContent_MakeAvailable(cpputils)

set(LIBRARY_SOURCES
    Scene/Scene.cpp

    Camera/Camera.cpp

    Primitive/Face.cpp
    Primitive/Plane.cpp
    Primitive/Sphere.cpp
    Primitive/Mesh.cpp

    Light/AmbientLight.cpp
    Light/DirectionalLight.cpp
    Light/PointLight.cpp
)

# Create the shared library
add_library(Scene3d STATIC ${LIBRARY_SOURCES})

target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Scene3d PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(Scene3d PUBLIC cpputils::Math)
target_link_libraries(Scene3d PUBLIC FrameBuffer)
target_link_libraries(Scene3d PUBLIC cpputils::Color)
target_link_libraries(Scene3d PUBLIC pthread)
