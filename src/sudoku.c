#include "sudoku.h"

// Returns a sudoku struct of size nxn
s_sudoku s_sudoku_create(size_t n) {
  s_sudoku g = (s_sudoku)malloc(sizeof(struct sudoku));
  if (!g) return NULL;

  return g;
}
