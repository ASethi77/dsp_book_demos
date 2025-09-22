cmake_minimum_required(VERSION 3.20)
project(signal_processing_book)
enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 20 REQUIRED)

find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(libdsp)
add_subdirectory(demos)