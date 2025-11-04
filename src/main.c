#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "cnf.h"
#include "sudoku_cnf.h"
#include "dpll.h"

void usage(char *exec) {
  printf("%s <filename>\n", exec);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  s_sudoku g = s_sudoku_create_from_file(argv[1]);
  if (!g) return EXIT_FAILURE;

  s_sudoku_print(stdout, g);

  s_cnf cn = sudoku_to_cnf(g);
  if (!cn) return EXIT_FAILURE;

  // s_cnf_print(cn);

  int *valuations = NULL;
  size_t valuations_length = 0;

  printf("Can be solved ? : %d\n", dpll_valuations(cn, &valuations, &valuations_length));

  printf("Solved grid :\n");

  for (int i = 0; i < valuations_length; i++) {
    int litt = valuations[i];
    sat_var v = litt_to_sat_var(g, litt);
    // printf("i: %d ; j: %d ; v : %d ; litt : x%d\n", v.i, v.j, v.value, litt);
    s_sudoku_set_cell_value(g, v.i, v.j, v.value);
  }

  s_sudoku_print(stdout, g);

  s_cnf_free(cn);
  s_sudoku_free(g);
  return EXIT_SUCCESS;
}
