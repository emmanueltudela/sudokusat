#ifndef __SUDOKU_H__
#define __SUDOKU_H__

#include <stdlib.h>

#include "cnf.h"


// Structs

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

/* Reads and create a sudoku struct from a file
 * The first line must contain the size n of the grid
 * The second line must contain the number of rules
 * The next lines describe the starting conditions of the grid following this scheme
 *      i -- this is the index of the target line
 *      j -- this is the index of the target column
 *      v -- this is the value to put in the cell (must be <= n)
 * each on a new line there must be no excess line break
 */
s_sudoku s_sudoku_read(char *filename);

// Free a sudoku and it's rules (if defined)
void s_sudoku_free(s_sudoku sud);

// Replace current set of rules with a new set of n rules for sudoku grid sud (see s_rules for explanaition of cells and rules
// Also modify the grid to show the rules used
void s_sudoku_set_rules(s_sudoku sud, int *cells, int *rules, size_t n);


// Utility functions

// Returns the index in sudoku grid corresponding to line i and col j
size_t s_sudoku_coords_to_index(s_sudoku sud, size_t i, size_t j);

// Prints a grid of sudoku
void s_sudoku_print(s_sudoku sud);


// SAT functions

// Returns a cnf formula describing constraints of grid
s_cnf s_sudoku_to_cnf(s_sudoku sud);

#endif
