cmake_minimum_required(VERSION 3.0)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(out_dir ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${out_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${out_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${out_dir})

project(work)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
ADD_COMPILE_OPTIONS(-Werror -Wall -Wextra)

add_executable(work main.cc)
