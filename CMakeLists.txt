cmake_minimum_required(VERSION 3.10)
project("Procedural Terrain Generation")

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories for GLFW and GLM
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glm)
include_directories(${CMAKE_SOURCE_DIR}/include/glm)

# Include FastNoise headers
include_directories(${CMAKE_SOURCE_DIR}/dependencies/FastNoise)

# Add the executable
add_executable(game
    src/main.cpp
    src/World.cpp
    src/Renderer.cpp
    src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/include/glad/glad.c
    src/Block.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/FastNoise/FastNoise.cpp
    src/Terrain.cpp
    src/Chunk.cpp
)

# Link directories for GLFW
link_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/build/src)  # This should point to where libglfw3.a is
link_directories(${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-vc2019)  # You don't need this for MinGW
link_directories(C:/TDM-GCC-32/lib)  # TDM-GCC libraries

# Use the plain signature for linking
target_link_libraries(game ${CMAKE_SOURCE_DIR}/dependencies/glfw/build/src/libglfw3.a OpenGL::GL)

# Link FastNoise library with the plain signature
target_link_libraries(game ${CMAKE_SOURCE_DIR}/dependencies/FastNoise/libFastNoise.a)
