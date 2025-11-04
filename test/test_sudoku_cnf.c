#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "sudoku_cnf.h"
#include "sudoku.h"

void test_sat_var_to_litt() {
  s_sudoku g = s_sudoku_create(4);
  assert(g);

  sat_var sv = {0, 0, 2, false};

  int litt = sat_var_to_litt(g, sv);
  assert(litt > 0);

  sv = (sat_var){0, 1, 2, true};

  litt = sat_var_to_litt(g, sv);
  assert(litt < 0);

  s_sudoku_free(g);
}

void test_litt_to_sat_var() {
  s_sudoku g = s_sudoku_create(9);
  assert(g);

  sat_var sv = {0, 0, 9, false};

  int litt = sat_var_to_litt(g, sv);
  sv = litt_to_sat_var(g, litt);
  assert(sv.i == 0);
  assert(sv.j == 0);
  assert(sv.value == 9);
  assert(!sv.is_negation);

  sv = (sat_var){0, 1, 2, true};

  litt = sat_var_to_litt(g, sv);
  sv = litt_to_sat_var(g, litt);
  assert(sv.i == 0);
  assert(sv.j == 1);
  assert(sv.value == 2);
  assert(sv.is_negation);

  s_sudoku_free(g);
}

void usage(char *exec) {
  printf("%s testname     -> Execute the given testname\n", exec);
  printf("%s all    -> Execute every tests\n", exec);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  bool execute_all = strcmp(argv[1], "all") == 0;

  if (strcmp(argv[1], "test_sat_var_to_litt") == 0 || execute_all) {
    test_sat_var_to_litt();
  }
  if (strcmp(argv[1], "test_litt_to_sat_var") == 0 || execute_all) {
    test_litt_to_sat_var();
  }
  return EXIT_SUCCESS;
}
