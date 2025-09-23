#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "sudoku.h"
#include "cnf.h"


// ===== PRIVATE =====

// Frees a set of rules
void s_rules_free(s_rules s_rul) {
  free(s_rul->cells);
  free(s_rul->rules);
  free(s_rul);
}

// Set every grid cell to 0
void s_sudoku_reset(s_sudoku sud) {
  if (!sud) return;

  for (int i = 0; i < sud->n * sud->n; i++) {
    sud->arr[i] = 0;
  }
}

// Update the sudoku grid with it's set of rules
void s_sudoku_apply_rules(s_sudoku sud) {
  if (!sud || !sud->rules) return;

  s_sudoku_reset(sud);

  for (int i = 0; i < sud->rules->len; i++) {
    int cell = sud->rules->cells[i];
    int rule = sud->rules->rules[i];
    sud->arr[cell] = rule;
  }
}

// ===================


// Base functions

s_sudoku s_sudoku_create(size_t n) {
  // Must be divisible by 3
  if (n % 3 != 0) return NULL;

  s_sudoku g = (s_sudoku)malloc(sizeof(struct sudoku));
  if (!g) return NULL;

  int *arr = malloc(sizeof(int) * n*n);
  if (!arr) {
    free(g);
    return NULL;
  }

  g->arr = arr;
  g->rules = NULL;
  g->n = n;
  return g;
}

s_sudoku s_sudoku_read(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) return NULL;

  char *line = NULL;
  size_t r = 0;

  // Size of grid
  getline(&line, &r, file);
  if (r <= 0) return NULL;
  size_t n = atoi(line);

  // Nb of rules
  getline(&line, &r, file);
  if (r <= 0) return NULL;
  size_t number_of_rules = atoi(line);

  s_sudoku sud = s_sudoku_create(n);

  int *rules = malloc(sizeof(int) * number_of_rules);
  int *cells = malloc(sizeof(int) * number_of_rules);

  for (int rule_i = 0; rule_i < number_of_rules; rule_i++) {
    int rule[3]; // {i, j, v}

  // Get all three values on the three following lines
  for (int i = 0; i < 3; i++) {
    if (getline(&line, &r, file) <= 0) {
      free(sud);
      return NULL;
    }
    rule[i] = atoi(line);
  }

  rules[rule_i] = rule[2]; // For this rule the cell cells[rule_i] must contain v
  cells[rule_i] = s_sudoku_coords_to_index(sud, rule[0], rule[1]); // The cell targeted by the rule is stored
  }

  s_sudoku_set_rules(sud, cells, rules, number_of_rules);
  free(rules);
  free(cells);
  free(line);
  fclose(file);
  return sud;
}

void s_sudoku_free(s_sudoku sud) {
  free(sud->arr);
  if (sud->rules) s_rules_free(sud->rules);
  free(sud);
}

void s_sudoku_set_rules(s_sudoku sud, int *cells, int *rules, size_t n) {
  if (!sud) return;

  // Create a new rule struct
  s_rules s_rul = (s_rules)malloc(sizeof(struct rules));
  if (!s_rul) return;

  // Copy the given cells and rules
  int *cel = (int *)malloc(sizeof(int)*n);
  if (!cel) {
    free(s_rul);
    return;
  }

  int *rul = (int *)malloc(sizeof(int)*n);
  if (!rul) {
    free(s_rul);
    free(cel);
    return;
  }

  memcpy(cel, cells, n * sizeof(int));
  memcpy(rul, rules, n * sizeof(int));

  // Set the new rules struct
  s_rul->cells = cel;
  s_rul->rules = rul;
  s_rul->len = n;

  // Change the set of rules and frees the last if needed
  if (sud->rules) s_rules_free(sud->rules);
  sud->rules = s_rul;

  // Apply the rules to the grid (Only to be able to display the grid with the rules)
  s_sudoku_apply_rules(sud);
}


// Utility functions

size_t s_sudoku_coords_to_index(s_sudoku sud, size_t i, size_t j) {
  if (!sud) return -1;
  return i * sud->n + j;
}

int s_sudoku_index_to_coords(s_sudoku sud, size_t index, size_t *i, size_t *j) {
  if (!i || !j || index >= sud->n * sud->n) return -1;
  *j = index % sud->n;
  *i = (index - *j) / sud->n;
  return 0;
}

void s_sudoku_print(s_sudoku sud) {
  if (!sud) return;

  printf("\n");

  for (int i = 0; i < sud->n; i++) {
    for (int j = 0; j < sud->n; j++) {
      size_t ind = s_sudoku_coords_to_index(sud, i, j);
      if (ind == -1) return;

      // Vertical separation
      if (j % 3 == 0 && j != 0) {
        printf("┃");
      } else if (j != sud->n) {
        printf("┊");
      }

      if (sud->arr[ind] != 0)
        printf(" %d ", sud->arr[ind]);
      else
        printf("   ");
    }

    // Horizontal separation

    printf("\n");

    for (int j = 0; j < sud->n; j++) {
      if ((i + 1) % 3 == 0 && i != 0 && i != sud->n - 1)
        printf("━━━━");
      else if (i != sud->n - 1)
        printf("┈┈┈┈");
    }

    printf("\n");

  }
}


// SAT functions

// ===== PRIVATE =====

// Takes the coords of a cell in a sudoku grid and a value
// This function will return a unique number for this combination
//    The goal is to represent the var x(ij, v) which represents
//    the variable in a sat formula.
int s_sudoku_sat_encode_litt(s_sudoku sud, size_t i, size_t j, size_t v) {
  int index = s_sudoku_coords_to_index(sud, i, j);
  return index * sud->n + v;
}

// Takes a var number in sat formula and returns the matching cell and value
// by putting them in the matching pointers
//    The pointers must be valid
//    The function returns 0 on success and -1 on failure
int s_sudoku_sat_decode_litt(s_sudoku sud, int litt, size_t *i, size_t *j, size_t *v) {
  if (!i || !j || !v) return -1;
  *v = litt % sud->n;
  int ind = (litt - *v) / sud->n;
  s_sudoku_index_to_coords(sud, ind, i, j);
  return 0;
}


// ===================

s_cnf s_sudoku_to_cnf(s_sudoku sud) {
  s_cnf cn = s_cnf_create();
  if (!cn) return NULL;

  // Sud constraints are respected
  for (int i = 0; i < sud->rules->len; i++) {
    int cell = sud->rules->cells[i];
    int v = sud->rules->rules[i];
    size_t i, j = 0;
    if (s_sudoku_index_to_coords(sud, cell, &i, &j) == -1) {
      s_cnf_free(cn);
      return NULL;
    }
    int *litt = malloc(sizeof(int));
    if (!litt) {
      s_cnf_free(cn);
      return NULL;
    }
    *litt = s_sudoku_sat_encode_litt(sud, i, j, v);
    if (s_cnf_add_clause(cn, litt, 1) == -1) {
      s_cnf_free(cn);
      return NULL;
    }
  }

  // Every cell must have a value
  for (int i = 0; i < sud->n * sud->n; i++) {
    int *litt = malloc(sizeof(int) * sud->n);
    if (!litt) {
      s_cnf_free(cn);
      return NULL;
    }
    for (int j = 0; j < sud->n; j++) {
      int cell = i;
      size_t cell_i, cell_j = 0;
      if (s_sudoku_index_to_coords(sud, cell, &cell_i, &cell_j) == -1) {
        free(litt);
        s_cnf_free(cn);
        return NULL;
      }
      int v = j + 1;
      litt[j] = s_sudoku_sat_encode_litt(sud, cell_i, cell_j, v);
    }
    if (s_cnf_add_clause(cn, litt, sud->n) == -1) {
      free(litt);
      s_cnf_free(cn);
      return NULL;
    }
  }

  // One cell cannot have two values
  for (int i = 0; i < sud->n * sud->n; i++) {
    for (int j = 0; j < sud->n; j++) {
      for (int k = 0; k < sud->n; k++) {
        if (j == k) continue;
        int *litt = malloc(sizeof(int) * 2);
        if (!litt) {
          s_cnf_free(cn);
          return NULL;
        }
        int cell = i;
        size_t cell_i, cell_j = 0;
        if (s_sudoku_index_to_coords(sud, cell, &cell_i, &cell_j) == -1) {
          free(litt);
          s_cnf_free(cn);
          return NULL;
        }
        int v = j + 1;
        int vp = k + 1;
        litt[0] = -s_sudoku_sat_encode_litt(sud, cell_i, cell_j, v);
        litt[1] = -s_sudoku_sat_encode_litt(sud, cell_i, cell_j, vp);
        if (s_cnf_add_clause(cn, litt, 2) == -1) {
          free(litt);
          s_cnf_free(cn);
          return NULL;
        }
      }

    }
  }

  return cn;
}
