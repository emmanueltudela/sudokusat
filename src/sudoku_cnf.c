#include <stdbool.h>

#include <math.h>

#include "sudoku.h"
#include "cnf.h"


// ===== PRIVATE =====

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
int sat_var_to_litt(s_sudoku g, sat_var v) {
  int n = s_sudoku_size(g);

  // By encoding this way we are sure that there is only one integer per
  // variable. The operation is also reversible.
  int litt = (n * v.i + v.j) * n + v.value;

  // The negation of our litteral is just -litteral
  if (v.is_negation) litt *= -1;

  return litt;
}

/* Decote the litteral of the sat formula to it's original meaning
 *
 * Is is simply the opposite operation of sat_var_to_litt
 */
sat_var litt_to_sat_var(s_sudoku g, int litt) {
  int n = s_sudoku_size(g);

  sat_var v;

  // Is it a normal variable or the negation of a variable ?
  if (litt < 0) {
    v.is_negation = true;
    litt *= -1;
  } else {
    v.is_negation = false;
  }

  // Get the value
  v.value = litt % n;
  litt /= n;

  // Get j
  v.j = litt % n;
  litt /= n;

  // Get i
  v.i = litt;

  return v;
}

// ======================== (1)

// ===== SUDOKU UTILS ====

// Utility for working with grid coords
typedef struct coords {
  size_t i, j;
} coords;

/* Returns a list of the coords of the cells in line i.
 *
 * The number of returned cells is the size of the grid g
 * For example in a sudoku of size 9 there is 9 cells in a
 * line.
 */
coords *get_sudoku_line(s_sudoku g, size_t i) {
  int n = s_sudoku_size(g);

  coords *cells = malloc(sizeof(coords) * n);
  if (!cells) return NULL;

  for (int j = 0; j < n; j++) {
    cells[j].i = i;
    cells[j].j = j;
  }

  return cells;
}

/* Returns the list of the coords of the cells in col j
 *
 * The number of cells in the result is the size of the grid g
 * For example in a sudoku of size 9 there is 9 cells in a
 * column.
 */
coords *get_sudoku_col(s_sudoku g, size_t j) {
  int n = s_sudoku_size(g);

  coords *cells = malloc(sizeof(coords) * n);
  if (!cells) return NULL;

  for (int i = 0; i < n; i++) {
    cells[i].i = i;
    cells[i].j = j;
  }

  return cells;
}

/* Returns the list of the coords of the cells in block b
 *
 * There is sqrt(n) * sqrt(n) squares in a block where n is the size of the
 * grid.
 *
 * A block is every sqrt(n) * sqrt(n) parts of the sudoku grid
 *
 *     n
 * <------->
 * 0 / 1 / 2
 * 3 / 4 / 5
 * 6 / 7 / 8
 */
coords *get_sudoku_block(s_sudoku g, size_t b) {
  int n = s_sudoku_size(g);

  coords *cells = malloc(sizeof(coords) * n);
  if (!cells) return NULL;

  // Here we determine the size of the blocks
  int sq = (int)sqrt(n);

  // Calculate the first cell of the block
  size_t first_cell_i = b / sq * sq;
  size_t first_cell_j = b % sq * sq;

  // Iterate over the size of the block
  // to get every cells of it
  int k = 0;
  for (int i = first_cell_i; i < first_cell_i + sq; i++) {
    for (int j = first_cell_j; j < first_cell_j + sq; j++) {
      int cells_index = k++;
      cells[cells_index].i = i;
      cells[cells_index].j = j;
    }
  }

  return cells;
}

// ======================= (1)

// ===== SUDOKU CNF =====

// ===== UTILITIES =====

/* Modify the formula to ensure that the set of cells contains every possible
 * values
 *
 * This function will be used with functions that returns a specific set of
 * cells (line, col, block) and will ensure that every line / col / block
 * contains every numbers (wich is the only way to have a valid sudoku
 * solution).
 *
 * The following formula will be merged to the current formula :
 *
 * (cell1 has value 1 OR cell1 has value 2 OR ...) AND
 * (cell2 has value 1 OR cell2 has value 2 OR ...) AND
 * ...
 *
 * In a sudoku grid of size 9 there is 9 possible values, from 1 to 9. Each
 * line, col and block must contain each of these values.
 *
 * We will ensure that there is only one of those values per line, col, block
 * in an other function.
 */
void add_cnf_sudoku_set_complete(s_cnf cn, s_sudoku g, coords *set, size_t set_length) {
  int n = s_sudoku_size(g);

  // For every possible value in the sudoku grid
  for (int possible_value = 1; possible_value < n + 1; possible_value++) {

    // This will contain the clause that will ensure that at
    // least one cell of the set contains this value
    int *clause = malloc(sizeof(int) * set_length);

    // There is at least one cell in the set that has this value
    for (int i = 0; i < set_length; i++) {
      coords cell = set[i]; // get current cell we are working on
      // Create a new var to represent the cell having the possible value
      sat_var cell_has_value_v = {cell.i, cell.j, possible_value, false};
      // Add it to the current clause
      clause[i] = sat_var_to_litt(g, cell_has_value_v);
    }

    // Add the clause to the formula
    s_cnf_add_clause(cn, clause, set_length);
    free(clause);

    // Repeats for every possible values
  }

  // Now the formula contains every clause necessary to ensure that
  // the given set of cells contains every possible values of the
  // sudoku grid.
}

/* Modify the formula to ensure that the set of cells contains each value at
 * most once
 *
 * The following formula will be merged to the current :
 *
 * (cell1 is not 1 OR cell2 is not 1) AND (cell1 is not 1 OR cell3 is not 1) AND ..
 * (cell2 is not 1 OR cell3 is not 1) AND ...
 * FOR EVERY PAIR OF DIFFERENT CELLS AND EVERY VALUE
 *
 * This function will be used with functions that returns a specific set of
 * cells (line, col, block) and will ensure that every line / col / block
 * contains each value at most once (wich is the only way to have a valid
 * sudoku solution).
 */
void add_cnf_sudoku_set_uniq(s_cnf cn, s_sudoku g, coords *set, size_t set_length) {
  int n = s_sudoku_size(g);

  // For every pair of different cells
  for (int i = 0; i < set_length; i++) {
    coords cell1 = set[i]; // get first cell

    // Second cell
    for (int j = 0; j < set_length; j++) {
      if (i == j) continue;   // cell1 and cell2 are different
      coords cell2 = set[j];  // get second cell

      // There is no color where they both have it
      for (int possible_value = 1; possible_value < n + 1; possible_value++) {
        // To tell they can't both have the same value we simply tell that at
        // least one of them doesn't have it
        sat_var cell1_doesnt_have_value = {cell1.i, cell1.j, possible_value, true};
        sat_var cell2_doesnt_have_value = {cell2.i, cell2.j, possible_value, true};

        // Create the two litts
        int litt1 = sat_var_to_litt(g, cell1_doesnt_have_value);
        int litt2 = sat_var_to_litt(g, cell2_doesnt_have_value);

        // Add them to the formula
        int clause[2] = {litt1, litt2};
        s_cnf_add_clause(cn, clause, 2);
      }
    }

    // Repeats for every possible value
  }

}

// ===================== (2)

/* Adds the default conditions of grid g to formula cn. The default conditions
 * are the non-empty cells of the given grid. When we solve the grid we don't
 * want the solution to modify the values that were already in the grid.
 *
 * To ensure that those values are kept we simply add one clause for each of
 * those cells. So we get (Cell i, j has val v) AND (Cell ...) AND (...) AND
 * ... . Each of those variables will need to be true for the overall solution
 * to be true. Like that we ensure the presence of the default values in the
 * solution.
 */
void add_cnf_default_conditions(s_cnf cn, s_sudoku g) {
  int n = s_sudoku_size(g);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int cell_val = s_sudoku_get_cell_value(g, i, j);
      // For each non-empty cell found in the grid force
      // the solution to keep this value
      if (cell_val != GRID_EMPTY_CELL) {
        sat_var v = {i, j, cell_val, 0};
        int litt = sat_var_to_litt(g, v);
        s_cnf_add_clause(cn, &litt, 1);
      }
    }
  }
}

/* Modify the cnf formula to ensure that every cell has at least one value. Of
 * course the cell can't be empty.
 *
 * We add the following formula to the current :
 * (cell1 has value 1 OR cell1 has value 2 OR cell1 has value3) OR ... AND
 * (cell2 has value 1 OR cell2 has value 2 OR cell2 has value3) OR ... AND
 * ...
 * (FOR EVERY CELL OF THE GRID AND EVERY VALUE)
 */
void add_cnf_cells_have_values(s_cnf cn, s_sudoku g) {
  int n = s_sudoku_size(g);

  // For every cells of the grid
  for (int i = 0; i < n; i++) {     // Line
    for (int j = 0; j < n; j++) {   // Col

      // The clause will contain every litteral needed to ensure that the given
      // cell has at least one of the possible values. There is n values in a
      // sudoku grid. For example a grid of size 9 will have 9 values (from 1
      // to 9)
      int clause[n];

      // This cell has at least one of the possible values
      for (int possible_value = 1; possible_value < n + 1; possible_value++) {
        sat_var cell_has_value = {i, j, possible_value, false};
        int litt = sat_var_to_litt(g, cell_has_value);

        // Store the sat var in the clause
        clause[possible_value - 1] = litt;
      }

      // Add the clause to the formula
      s_cnf_add_clause(cn, clause, n);

      // Repeat for the next cell of the grid
    }
  }

}

/* Modify the cnf formula to ensure that every cell has at most one value
 *
 * We add the following formula to the current :
 * (cell1 doesn't have value 1 OR cell1 doesn't have value 2) AND
 * (cell1 doesn't have value 1 OR cell1 doesn't have value 3) AND
 * ...
 * (FOR EVERY CELL AND EVERY PAIR OF DIFFERENT VALUES)
 */
void add_cnf_cells_have_one_value(s_cnf cn, s_sudoku g) {
  int n = s_sudoku_size(g);

  // For every cells of the grid
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {

      // For every pair of two different values
      for (int v = 1; v < n + 1; v++) {
        for (int w = 1; w < n + 1; w++) {
          if (v == w) continue; // v and w must be different

          // To tell that a cell can't have to different values we tell that it
          // doesn't have at least one of them
          sat_var cell_doesnt_have_value_v = {i, j, v, true};
          sat_var cell_doesnt_have_value_w = {i, j, w, true};

          // Create litterals
          int litt1 = sat_var_to_litt(g, cell_doesnt_have_value_v);
          int litt2 = sat_var_to_litt(g, cell_doesnt_have_value_w);

          // Add it to the formula
          int clause[2] = {litt1, litt2};
          s_cnf_add_clause(cn, clause, 2);
        }
      }

      // Repeat for every cells
    }
  }
}

// ====================== (1)

// ===================  (0)


// ===== BASE FUNCTIONS =====

s_cnf sudoku_to_cnf(s_sudoku g) {
  if (!g) return NULL;

  s_cnf cn = s_cnf_create();
  if (!cn) return NULL;

  // Size of the grid
  int n = s_sudoku_size(g);

  // Base conditions
  add_cnf_default_conditions(cn, g);

  // We wan't every cell to have exactly one value
  // To do that we will ensure that every cell has
  // at least one value and at most one value.

  // At least
  add_cnf_cells_have_values(cn, g);

  // At most
  add_cnf_cells_have_one_value(cn, g);

  // We want each line of the grid to contain every possible values exactly one
  // time (rules of sudoku).

  // For every lines
  for (int l = 0; l < n; l++) {
    // Get every cells of the current line
    coords *line = get_sudoku_line(g, l);

    // Ensure it contains every values at least once
    add_cnf_sudoku_set_complete(cn, g, line, n);

    // Ensure it contains every values at most once
    add_cnf_sudoku_set_uniq(cn, g, line, n);

    free(line);
  }

  // Same for columns

  // For every columns
  for (int c = 0; c < n; c++) {
    // Get every cells of the current col
    coords *col = get_sudoku_col(g, c);

    // Ensure it contains every values at least once
    add_cnf_sudoku_set_complete(cn, g, col, n);

    // Ensure it contains every values at most once
    add_cnf_sudoku_set_uniq(cn, g, col, n);

    free(col);
  }

  // Same for blocks

  // For every block
  for (int b = 0; b < n; b++) {
    // Get every cells of the current block
    coords *block = get_sudoku_block(g, b);

    // Ensure it contains every values at least once
    add_cnf_sudoku_set_complete(cn, g, block, n);

    // Ensure it contains every values at most once
    add_cnf_sudoku_set_uniq(cn, g, block, n);

    free(block);
  }

  // All those rules ensure that the solution we can find by solving the
  // formula respect the rules of our initial problem sudoku problem.

  return cn;
}

// ========================== (0)
