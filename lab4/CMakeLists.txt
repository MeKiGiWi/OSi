cmake_minimum_required(VERSION 3.10)
project(DynamicLibraryLab C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fPIC")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)

# Libs must be added before src
add_subdirectory(libs)

add_subdirectory(src)
