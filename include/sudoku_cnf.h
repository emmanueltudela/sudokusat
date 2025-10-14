#ifndef SUDOKU_CNF_H
#define SUDOKU_CNF_H

#include "sudoku.h"
#include "cnf.h"


// ===== BASE FUNCTIONS =====

/* Reduction of sudoku problem (is there a solution for given sudoku grid ?) to
 * sat problem (is this boolean formula satisfiable ?)
 *
 * If the returned cnf (special form of boolean formula with only conjunctions
 * of disjunctions) is proven satifiable by a sat solver then the sudoku grid
 * is also satisfiable and we can use the sat solver to find this solution
 *      - g must be a valid grid
 *
 * Returns NULL on failure
 */
s_cnf sudoku_to_cnf(s_sudoku g);

// ==========================


#endif
