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
  int *arr;       // Grid of the sudoku game
  s_rules rules;  // Set of rules to prefill the game
  size_t n;       // Width of the grid, ultimate size is n*n (n must be divisible by 3)
} *s_sudoku;


// Base functions

// Returns a sudoku struct of size nxn
s_sudoku s_sudoku_create(size_t n);

// Free a sudoku and it's rules (if defined)
void s_sudoku_free(s_sudoku sud);

// Add a set of n rules to sudoku grid sud (see s_rules for explanaition of cells and rules
void s_sudoku_add_rules(s_sudoku sud, int *cells, int *rules, size_t n);


// Utility functions

// Returns the index in sudoku grid corresponding to line i and col j
size_t s_sudoku_coords_to_index(s_sudoku sud, size_t i, size_t j);

// Prints a grid of sudoku
void s_sudoku_print(s_sudoku sud);


// SAT functions

#endif
