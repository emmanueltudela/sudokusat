#ifndef __SUDOKU_H__
#define __SUDOKU_H__

#include <stdlib.h>

// Represents the initial rules of
// a grid
typedef struct rules {
  int *cells; // cells[i] corresponds to the cell target by the rule i
  int *rules; // rules[i] corresponds to the digit to put in the cell for rule i
  size_t len; // number of rules
} *s_rules;

// Represents sudoku grid in memory
typedef struct sudoku {
  int *arr;
  s_rules rules;
  size_t n;
} *s_sudoku;

s_sudoku s_sudoku_create(size_t n);

#endif
