cmake_minimum_required(VERSION 3.30)
project(sudokusolver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE sources       src/*.c include/*.h)

enable_testing()

add_executable(solver ${sources})

target_link_libraries(solver PUBLIC m)
target_compile_options(solver PUBLIC -std=c99 -Wall -g)
target_include_directories(solver PUBLIC include)

add_executable(sudoku_test test/sudoku_test.c)

target_link_libraries(sudoku_test PUBLIC m)
target_compile_options(sudoku_test PUBLIC -std=c99 -Wall -g)
target_include_directories(sudoku_test PUBLIC include)

# SUDOKU
add_test(NAME s_sudoku_reset_test COMMAND sudoku_test s_sudoku_reset)
add_test(NAME s_sudoku_apply_rules_test COMMAND sudoku_test s_sudoku_apply_rules)
add_test(NAME s_sudoku_read_test COMMAND sudoku_test s_sudoku_read)
add_test(NAME s_sudoku_set_rules_test COMMAND sudoku_test s_sudoku_set_rules)
add_test(NAME s_sudoku_coords_to_index_test COMMAND sudoku_test s_sudoku_coords_to_index)
add_test(NAME s_sudoku_index_to_coords_test COMMAND sudoku_test s_sudoku_index_to_coords)
add_test(NAME s_sudoku_sat_encode_litt_test COMMAND sudoku_test s_sudoku_sat_encode_litt)
add_test(NAME s_sudoku_sat_decode_litt_test COMMAND sudoku_test s_sudoku_sat_decode_litt)
add_test(NAME s_sudoku_get_line_test COMMAND sudoku_test s_sudoku_get_line)
add_test(NAME s_sudoku_get_col_test COMMAND sudoku_test s_sudoku_get_col)
add_test(NAME s_sudoku_get_block_test COMMAND sudoku_test s_sudoku_get_block)
add_test(NAME s_sudoku_cnf_constraints_test COMMAND sudoku_test s_sudoku_cnf_constraints)
add_test(NAME s_sudoku_cnf_values_test COMMAND sudoku_test s_sudoku_cnf_values)
add_test(NAME s_sudoku_cnf_uniq_test COMMAND sudoku_test s_sudoku_cnf_uniq)
add_test(NAME s_sudoku_cnf_set_complete_test COMMAND sudoku_test s_sudoku_cnf_set_complete)
add_test(NAME s_sudoku_cnf_set_uniq_test COMMAND sudoku_test s_sudoku_cnf_set_uniq)
add_test(NAME s_sudoku_cnf_line_complete_test COMMAND sudoku_test s_sudoku_cnf_line_complete)
add_test(NAME s_sudoku_cnf_line_uniq_test COMMAND sudoku_test s_sudoku_cnf_line_uniq)
add_test(NAME s_sudoku_cnf_col_complete_test COMMAND sudoku_test s_sudoku_cnf_col_complete)
add_test(NAME s_sudoku_cnf_col_uniq_test COMMAND sudoku_test s_sudoku_cnf_col_uniq)
add_test(NAME s_sudoku_cnf_cel_complete_test COMMAND sudoku_test s_sudoku_cnf_cel_complete)
add_test(NAME s_sudoku_cnf_cel_uniq_test COMMAND sudoku_test s_sudoku_cnf_cel_uniq)
