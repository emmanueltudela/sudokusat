cmake_minimum_required(VERSION 3.30)
project(sudokusolver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE sources       src/*.c include/*.h)
file(GLOB_RECURSE sources_test  test/*.c)

add_executable(solver ${sources})

target_compile_options(solver PUBLIC -std=c99 -Wall)
target_include_directories(solver PUBLIC include)
