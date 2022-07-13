cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(opengl-renderer)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)
add_executable(opengl-renderer ${SOURCE_FILES})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(assimp REQUIRED)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

add_dependencies(opengl-renderer OpenGL::OpenGL)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(opengl-renderer ${GTKMM_LIBRARIES})

target_link_libraries(opengl-renderer glfw)
target_link_libraries(opengl-renderer OpenGL::OpenGL)
target_link_libraries(opengl-renderer assimp)
