#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "cnf.h"

int main(void) {
  s_grid g = s_grid_create_from_file("../data/test3.txt");
  if (!g) return EXIT_FAILURE;

  s_grid_print(stdout, g);

  s_cnf cn = s_cnf_create();
  if (!cn) return EXIT_FAILURE;

  int cl1[] = {1, 2, 3};
  int cl2[] = {-1, 2, -3};

  s_cnf_add_clause(cn, cl1, 3);
  s_cnf_add_clause(cn, cl2, 3);

  s_cnf_print(cn);

  s_cnf_free(cn);
  s_grid_free(g);
  return EXIT_SUCCESS;
}
