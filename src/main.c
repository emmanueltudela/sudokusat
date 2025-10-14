#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "cnf.h"
#include "sudoku_cnf.h"

int main(void) {
  s_sudoku g = s_sudoku_create_from_file("../data/test.txt");
  if (!g) return EXIT_FAILURE;

  s_sudoku_print(stdout, g);

  s_cnf cn = sudoku_to_cnf(g);
  if (!cn) return EXIT_FAILURE;

  s_cnf_print(cn);

  s_cnf_free(cn);
  s_sudoku_free(g);
  return EXIT_SUCCESS;
}
