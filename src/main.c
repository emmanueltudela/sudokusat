#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main(void) {
  s_sudoku sud = s_sudoku_create(9);
  s_sudoku_print(sud);
  return EXIT_SUCCESS;
}
