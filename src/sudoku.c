#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

#include "sudoku.h"
#include "math.h"


// ===== STRUCTS =====

typedef struct sudoku {
  size_t *grid;
  size_t n;
} *s_sudoku;

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
bool valid_grid_coords(s_sudoku g, size_t i, size_t j) {
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
size_t grid_coords_to_index(s_sudoku g, size_t i, size_t j) {
  return i * g->n + j;
}

/* Returns the size n of the grid from the given file
 * that must contain a grid description
 *    - filename must be non-null and represent a valid file
 *
 * Returns -1 on error
 */
int get_grid_size_from_file(char *filename) {
  if (!filename) return -1;

  FILE *file = fopen(filename, "r");
  if (!file) return -1;

  char *buffer = NULL;
  size_t buffer_size = 0;
  int r = 0;

  // Read the first line of the file
  r = getline(&buffer, &buffer_size, file);
  if (r == 0 || buffer_size <= 0) {
    fclose(file);
    free(buffer);
    return -1;
  }

  // Initialize strtok
  int size = 0;
  char *token = strtok(buffer, ";");
  if (!token) {
    fclose(file);
    free(buffer);
    return -1;
  };

  // For every token increase the size of the grid
  do {
    size++;
  } while ((token = strtok(NULL, ";")));

  fclose(file);
  free(buffer);
  return size;
}

// ===================


// ===== BASE FUNCTIONS =====

s_sudoku s_sudoku_create(size_t n) {
  if (!is_perfect_square(n) || n == 0 || n == 1) return NULL;

  s_sudoku g = (s_sudoku)malloc(sizeof(struct sudoku));
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

void s_sudoku_free(s_sudoku g) {
  free(g->grid);
  free(g);
}

// ==========================


// ===== GETTERS =====

size_t s_sudoku_size(s_sudoku g) {
  if (!g) return 0;
  return g->n;
}

int s_sudoku_get_cell_value(s_sudoku g, size_t i, size_t j) {
  if (!g || !valid_grid_coords(g, i, j)) return -1;

  int index = grid_coords_to_index(g, i, j);
  return g->grid[index];
}

// ===================


// ===== SETTERS =====

int s_sudoku_set_cell_value(s_sudoku g, size_t i, size_t j, size_t val) {
  if (!g || !valid_grid_coords(g, i, j)) return -1;
  bool valid_val = val >= 0 && val <= g->n;
  if (!valid_val) return -1;

  int index = grid_coords_to_index(g, i, j);
  g->grid[index] = val;
  return 0;
}

// ===================


// ===== UTILITY FUNCTIONS =====

s_sudoku s_sudoku_create_from_file(char *filename) {
  if (!filename) return NULL;

  // Open file
  FILE *file = fopen(filename, "r");
  if (!file) return NULL;

  // Init grid
  int n = get_grid_size_from_file(filename);
  if (n == -1) {
    fclose(file);
    return NULL;
  }

  s_sudoku g = s_sudoku_create(n);

  char *buffer = NULL;
  size_t buffer_size = 0;
  int r = 0;

  // Those are user to keep count
  // of the cells position in the grid description
  size_t i = 0; // Current grid line
  size_t j = 0; // Current gird col

  // For each line of the file
  while ((r = getline(&buffer, &buffer_size, file)) > 0) {
    char *token = strtok(buffer, ";");
    if (!token) continue;

    int line_size = 0;

    // For every token in the line
    do {
      // Get cell value
      int val = atol(token);
      if (val < 0 || val > n) {
        fclose(file);
        s_sudoku_free(g);
        free(buffer);
        return NULL;
      }

      line_size++;

      // If there is more cols than the size of the grid
      // there is an error
      if (line_size > n) {
        fclose(file);
        s_sudoku_free(g);
        s_sudoku_free(g);
        free(buffer);
        return NULL;
      }

      // Update grid
      int index = grid_coords_to_index(g, i, j);
      g->grid[index] = val;

      j++; // Next col
    } while ((token = strtok(NULL, ";")));

    j = 0; // Reset col
    i++;   // Next line

    // If there is too much line in the file
    // Or not enough columns in the line
    // Error
    if (i > n || line_size != n) {
      fclose(file);
      s_sudoku_free(g);
      free(buffer);
      return NULL;
    }
  }

  // If there is not enough lines in the file
  // Error
  if (i != n) {
    fclose(file);
    s_sudoku_free(g);
    free(buffer);
    return NULL;
  }

  fclose(file);
  free(buffer);
  return g;
}

int s_sudoku_print(FILE *file, s_sudoku g) {
  if (!g || !file) return -1;

  for (int i = 0; i < g->n; i++) {
    for (int j = 0; j < g->n; j++) {
      int index = grid_coords_to_index(g, i, j);
      int val = g->grid[index];

      if (val != 0) {
        if (fprintf(file, " %d ", val) < 0) return -1;
      } else {
        if (fprintf(file, "   ") < 0) return -1;
      }

      // Show hard separation every sqrt(n) cells
      // On a n=9 grid it is every 3 cells
      bool verticalSeparation = (j + 1) % (int)sqrt(g->n) == 0;
      bool isLastCol = j == g->n - 1;

      if (verticalSeparation && !isLastCol) {
        if (fprintf(file, "┃") < 0) return -1;
      } else if (!isLastCol) {
        if (fprintf(file, "┊") < 0) return -1;
      }
    }

    if (fprintf(file, "\n") < 0) return -1;

    // Show hard separation every sqrt(n) cells
    // On a n=9 grid it is every 3 cells
    bool horizontalSeparation = (i + 1) % (int)sqrt(g->n) == 0;
    bool isLastLine = i == g->n - 1;

    for (int k = 0; k < g->n; k++) {
      if (horizontalSeparation && !isLastLine) {
        if (fprintf(file, "━━━━") < 0) return -1;
      } else if (!isLastLine) {
        if (fprintf(file, "┈┈┈┈") < 0) return -1;
      }
    }

    if (!isLastLine)
      if (fprintf(file, "\n") < 0) return -1;

  }

  return 0;
}

// =============================
