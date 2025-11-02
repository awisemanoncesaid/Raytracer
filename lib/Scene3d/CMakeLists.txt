cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.2
)
FetchContent_MakeAvailable(nlohmann_json)

set(LIBRARY_SOURCES
    Scene/Scene.cpp

    Camera/Camera.cpp

    Primitive/Primitive.cpp
    #Primitive/Face.cpp
    Primitive/Plane.cpp
    Primitive/Sphere.cpp
    #Primitive/Mesh.cpp

    Light/Light.cpp
    Light/AmbientLight.cpp
    Light/DirectionalLight.cpp
    Light/PointLight.cpp
)

add_library(Scene3d STATIC ${LIBRARY_SOURCES})

target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Camera)
target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Light)
target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Primitive)
target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Scene)
target_include_directories(Scene3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Serializable)

target_link_libraries(Scene3d PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(Scene3d PRIVATE Math)
target_link_libraries(Scene3d PUBLIC FrameBuffer)
if (UNIX)
    target_link_libraries(Scene3d PRIVATE pthread)
endif()
