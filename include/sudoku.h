#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>


// ===== STRUCTS =====

// Private sudoku grid type
typedef struct grid *s_grid;

// ===================


// ===== BASE FUNCTIONS =====

/* Creates a grid of sudoku, filled with 0,
 * of size n*n and returns it.
 *    - n must be a perfect square (!= 0 && != 1)
 *
 * Returns NULL on failure.
 */
s_grid s_grid_create(size_t n);

void s_grid_free(s_grid g);

// ==========================


// ===== GETTERS =====

/* Returns size n of grid g
 *    - g must be a non-null grid
 *
 * Returns 0 on failure
 */
size_t s_grid_size(s_grid g);

/* Returns the cell value at line i and col j
 * in grid g
 *    - 0 <= i < n (n the size of g)
 *    - 0 <= j < n (n the size of g)
 *
 * A cell value of 0 means the cell is empty
 *
 * Returns -1 on failure
 */
int s_grid_get_cell_value(s_grid g, size_t i, size_t j);

// ===================


// ===== SETTERS =====

/* Set the cell at line i and col j the value val
 *    - g must be a non-null grid
 *    - 0 <= val <= n (n the size of g)
 *      A cell value of 0 means an empty cell
 *    - 0 <= i < n (n the size of g)
 *    - 0 <= j < n (n the size of g)
 *
 * Returns 0 on success and -1 on failure
 */
int s_grid_set_cell_value(s_grid g, size_t i, size_t j, size_t val);

// ===================


// ===== UTILITY FUNCTIONS =====

/* Returns a new grid created following the description
 * given in the file (filename)
 *    - filename must be non-null and represent a valid file
 *    - A valid file represents a full grid of sudoku and
 *      must follow these rules :
 *        * Each new line of the file is a line of the grid
 *        * Each line must have the same size
 *        * The number of cells (n) on a line must be
 *          a perfect square (4, 9, 25, ...)
 *        * Each cell must be separated by a semicolon ';'
 *            . No semicolon before the first cell nor after the last
 *                - The number must be >= 0
 *                  (if the number is 0 it means the cell is empty)
 *                - The number must be <= n (n the size of g)
 *
 *      Example of a 4*4 grid :
 *
 *      0;0;1;2
 *      0;1;1;1
 *      1;3;1;4
 *      0;0;0;0
 *
 *      Which represents the grid :
 *      |   |   | 1 | 2 |
 *      -----------------
 *      |   | 1 | 1 | 1 |
 *      -----------------
 *      | 1 | 3 | 1 | 4 |
 *      -----------------
 *      |   |   |   |   |
 *
 * NOTE : If the grid description contains text or something
 * other than numbers it will be replaced by an empty cell in the
 * final grid.
 *
 * Returns NULL on failure
 */
s_grid s_grid_create_from_file(char *filename);

/* Prints a pretty grid to file with the
 * numbers and size of grid g
 *    - file must be a valid file
 *    - g must be a non-null grid
 *
 * Returns 0 on success -1 on failure
 */
int s_grid_print(FILE *file, s_grid g);

// =============================


#endif
