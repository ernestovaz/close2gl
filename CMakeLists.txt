cmake_minimum_required(VERSION 3.23)
find_package(glfw3 3.3 REQUIRED)

project(opengl-renderer)

add_executable(opengl-renderer main.cpp)

target_link_libraries(opengl-renderer glfw)

