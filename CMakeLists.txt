cmake_minimum_required(VERSION 3.30)
project(sudokusolver)

file(GLOB_RECURSE sources       src/*.c include/*.h)
file(GLOB_RECURSE sources_test  test/*.c)

add_executable(solver ${sources})

target_compile_options(solver PUBLIC -std=c99 -Wall)
target_include_directories(solver PUBLIC include)

execute_process(COMMAND pandoc -o ../concept.pdf ../doc/concept.md)
