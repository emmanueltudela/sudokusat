cmake_minimum_required(VERSION 3.30)
project(sudokusolver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Main executable

file(GLOB_RECURSE sources       src/*.c include/*.h)

add_executable(solver ${sources})

target_link_libraries(solver PUBLIC m)
target_compile_options(solver PUBLIC -std=c99 -Wall -g)
target_include_directories(solver PUBLIC include)

# Test sudoku

add_executable(test_sudoku test/test_sudoku.c src/sudoku.c)

target_link_libraries(test_sudoku PUBLIC m)
target_compile_options(test_sudoku PUBLIC -std=c99 -Wall -g)
target_include_directories(test_sudoku PUBLIC include)

add_test(NAME test_s_grid_create COMMAND test_sudoku test_s_grid_create)
add_test(NAME test_s_grid_free COMMAND test_sudoku test_s_grid_free)
add_test(NAME test_s_grid_size COMMAND test_sudoku test_s_grid_size)
add_test(NAME test_s_grid_get_cell_value COMMAND test_sudoku test_s_grid_get_cell_value)
add_test(NAME test_s_grid_set_cell_value COMMAND test_sudoku test_s_grid_set_cell_value)
add_test(NAME test_s_grid_create_from_file COMMAND test_sudoku test_s_grid_create_from_file)
add_test(NAME test_s_grid_print COMMAND test_sudoku test_s_grid_print)
