#include <stdbool.h>

#include "sudoku.h"
#include "math.h"


// ===== STRUCTS =====

typedef struct grid {
  size_t *grid;
  size_t n;
} *s_grid;

// ===================


// ===== PRIVATE =====

/* Returns wether n is a perfect square
 *    - n must be a positive number
 */
bool is_perfect_square(int n) {
  return sqrt(n) * sqrt(n) == n;
}

/* Returns true if the given col and line are in the
 * given grid range or not
 *    - g must be a non-null grid
 */
bool valid_grid_coords(s_grid g, size_t i, size_t j) {
  bool valid_line = i >= 0 && i < g->n;
  bool valid_col = j >= 0 && j < g->n;
  return valid_line && valid_col;
}

/* Takes col (j) and line (i) coords in grid g and returns
 * a uniq index for these coords in a 1d array
 *    - g must be a non-null grid
 *    - 0 <= i < n (n the size of g)
 *    - 0 <= j < n (n the size of g)
 */
size_t grid_coords_to_index(s_grid g, size_t i, size_t j) {
  return i * g->n + j;
}

// ===================


// ===== BASE FUNCTIONS =====

s_grid s_grid_create(size_t n) {
  if (!is_perfect_square(n) || n == 0 || n == 1) return NULL;

  s_grid g = (s_grid)malloc(sizeof(struct grid));
  if (!g) return NULL;

  size_t *grid = (size_t *)calloc(n * n, sizeof(size_t));
  if (!grid) {
    free(g);
    return NULL;
  }

  g->grid = grid;
  g->n = n;
  return g;
}

void s_grid_free(s_grid g) {
  free(g->grid);
  free(g);
}

// ==========================


// ===== GETTERS =====

size_t s_grid_size(s_grid g) {
  if (!g) return 0;
  return g->n;
}

int s_grid_get_cell_value(s_grid g, size_t i, size_t j) {
  if (!g || !valid_grid_coords(g, i, j)) return -1;

  int index = grid_coords_to_index(g, i, j);
  return g->grid[index];
}

// ===================


// ===== SETTERS =====

int s_grid_set_cell_value(s_grid g, size_t i, size_t j, size_t val) {
  if (!g || !valid_grid_coords(g, i, j)) return -1;
  bool valid_val = val >= 0 && val <= g->n;
  if (!valid_val) return -1;

  int index = grid_coords_to_index(g, i, j);
  g->grid[index] = val;
  return 0;
}

// ===================


// ===== UTILITY FUNCTIONS =====

s_grid s_grid_create_from_file(char *filename) {
  return NULL;
}

int s_grid_print(FILE *file, s_grid g) {
  if (!g || !file) return -1;

  

  return 0;
}

// =============================
