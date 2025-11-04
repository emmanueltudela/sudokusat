#ifndef SUDOKU_CNF_H
#define SUDOKU_CNF_H

#include "sudoku.h"
#include "cnf.h"

// ===== SAT VARIABLES =====

/* This is a variable for the sat formula.
 *
 * Our sudoku problem can be reduced to a sat problem. By "encoding" our first
 * problem in a var like so, we will be able to "inject" it in a sat formula so
 * that when we solve the sat formula we can solve the sudoku grid by looking
 * at the variables valuations.
 *
 * If this var is found true then we will know that the cell (i, j) contains
 * value. If the negation of this var is found true then we will know that the
 * cell (i, j) does not contain value.
 */
typedef struct sat_var {
  int i, j;       // Cell
  int value;      // Cell value
  bool is_negation;  // Is this var negative or not ? (x or not(x))
} sat_var;

/* Returns an integer corresponding to the given sat_var in the given context
 * of the grid (that will be used as a litteral for the sat formula)
 *
 * The "encoding" used garanties one interger per sat_var and is reversible.
 */
int sat_var_to_litt(s_sudoku g, sat_var v);

/* Decote the litteral of the sat formula to it's original meaning
 *
 * Is is simply the opposite operation of sat_var_to_litt
 */
sat_var litt_to_sat_var(s_sudoku g, int litt);

// ======================== (1)

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
