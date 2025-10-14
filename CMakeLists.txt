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

add_test(NAME test_s_sudoku_create COMMAND test_sudoku test_s_sudoku_create)
add_test(NAME test_s_sudoku_free COMMAND test_sudoku test_s_sudoku_free)
add_test(NAME test_s_sudoku_size COMMAND test_sudoku test_s_sudoku_size)
add_test(NAME test_s_sudoku_get_cell_value COMMAND test_sudoku test_s_sudoku_get_cell_value)
add_test(NAME test_s_sudoku_set_cell_value COMMAND test_sudoku test_s_sudoku_set_cell_value)
add_test(NAME test_s_sudoku_create_from_file COMMAND test_sudoku test_s_sudoku_create_from_file)
add_test(NAME test_s_sudoku_print COMMAND test_sudoku test_s_sudoku_print)

# Test cnf

add_executable(test_cnf test/test_cnf.c src/cnf.c)

target_compile_options(test_cnf PUBLIC -std=c99 -Wall -g)
target_include_directories(test_cnf PUBLIC include)

add_test(NAME test_s_cnf_create COMMAND test_cnf test_s_cnf_create)
add_test(NAME test_s_cnf_free COMMAND test_cnf test_s_cnf_free)
add_test(NAME test_s_cnf_copy COMMAND test_cnf test_s_cnf_copy)
add_test(NAME test_s_cnf_add_clause COMMAND test_cnf test_s_cnf_add_clause)
add_test(NAME test_s_cnf_remove_clause COMMAND test_cnf test_s_cnf_remove_clause)
add_test(NAME test_s_cnf_clause_add_litt COMMAND test_cnf test_s_cnf_clause_add_litt)
add_test(NAME test_s_cnf_clause_remove_litt COMMAND test_cnf test_s_cnf_clause_remove_litt)
add_test(NAME test_s_cnf_clause_empty COMMAND test_cnf test_s_cnf_clause_empty)
add_test(NAME test_s_cnf_clause_unit COMMAND test_cnf test_s_cnf_clause_unit)
add_test(NAME test_s_cnf_clause_contains_litt COMMAND test_cnf test_s_cnf_clause_contains_litt)
add_test(NAME test_s_cnf_get_clauses_ids COMMAND test_cnf test_s_cnf_get_clauses_ids)
add_test(NAME test_s_cnf_clause_get_litts COMMAND test_cnf test_s_cnf_clause_get_litts)
add_test(NAME test_s_cnf_print COMMAND test_cnf test_s_cnf_print)
