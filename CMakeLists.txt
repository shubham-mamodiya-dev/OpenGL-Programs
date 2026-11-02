cmake_minimum_required(VERSION 3.20)

project(LearnOpenGL LANGUAGE C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Graphics dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# GLAD
add_subdirectory(external/glad)

add_library(opengl_common INTERFACE)


target_link_libraries(opengl_common
INTERFACE
OpenGL::GL
glfw
glm::glm
glad
)


# Experiments
add_subdirectory(src/1_getting_started/1_hello_window)
