cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(opengl-renderer)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)
add_executable(opengl-renderer ${SOURCE_FILES})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_dependencies(opengl-renderer OpenGL::OpenGL)

target_link_libraries(opengl-renderer glfw)
target_link_libraries(opengl-renderer OpenGL::OpenGL)

