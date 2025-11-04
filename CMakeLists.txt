cmake_minimum_required(VERSION 3.20)
project(sudokusolver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# Main executable

file(GLOB_RECURSE sources       src/*.c include/*.h lib/include/*.h)

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

# Test sudoku_cnf

add_executable(test_sudoku_cnf test/test_sudoku_cnf.c src/sudoku_cnf.c src/cnf.c src/sudoku.c)

target_link_libraries(test_sudoku_cnf PUBLIC m)
target_compile_options(test_sudoku_cnf PUBLIC -std=c99 -Wall -g)
target_include_directories(test_sudoku_cnf PUBLIC include)

add_test(NAME test_sat_var_to_litt COMMAND test_sudoku_cnf test_sat_var_to_litt)
add_test(NAME test_litt_to_sat_var COMMAND test_sudoku_cnf test_litt_to_sat_var)

# Test DPLL

add_executable(test_dpll test/test_dpll.c src/cnf.c)

target_compile_options(test_dpll PUBLIC -std=c99 -Wall -g)
target_include_directories(test_dpll PUBLIC include)

add_test(NAME test_get_unit_clause_litt COMMAND test_dpll test_get_unit_clause_litt)
add_test(NAME test_get_pure_litteral COMMAND test_dpll test_get_pure_litteral)
add_test(NAME test_cnf_is_empty COMMAND test_dpll test_cnf_is_empty)
add_test(NAME test_cnf_contains_empty_clause COMMAND test_dpll test_cnf_contains_empty_clause)
add_test(NAME test_cnf_choose_litteral COMMAND test_dpll test_cnf_choose_litteral)
add_test(NAME test_unit_propagate COMMAND test_dpll test_unit_propagate)
add_test(NAME test_pure_litteral_assign COMMAND test_dpll test_pure_litteral_assign)
