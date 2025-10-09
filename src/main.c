#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main(void) {
  s_grid g = s_grid_create_from_file("../data/test3.txt");
  if (!g) return EXIT_FAILURE;

  s_grid_print(stdout, g);

  s_grid_free(g);
  return EXIT_SUCCESS;
}
