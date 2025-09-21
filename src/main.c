#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "cnf.h"

int main(void) {
  s_sudoku sud = s_sudoku_create(9);
  s_sudoku_print(sud);

  s_cnf cn = s_cnf_create();
  int litt1[] = {1, 2, 3};
  int litt2[] = {-2, 3, -1};
  s_cnf_add_clause(cn, litt1, 3);
  s_cnf_add_clause(cn, litt2, 3);
  s_cnf_print(cn);

  s_sudoku_free(sud);
  s_cnf_free(cn);
  return EXIT_SUCCESS;
}
