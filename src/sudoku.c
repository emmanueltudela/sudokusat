#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "sudoku.h"
#include "cnf.h"

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

void s_rules_free(s_rules s_rul) {
  free(s_rul->cells);
  free(s_rul->rules);
  free(s_rul);
}

void s_sudoku_set_rules(s_sudoku sud, int *cells, int *rules, size_t n) {
  if (!sud) return;

  // Create a new rule struct
  s_rules s_rul = (s_rules)malloc(sizeof(struct rules));
  if (!s_rul) return;

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

  s_rul->cells = cel;
  s_rul->rules = rul;
  s_rul->len = n;

  if (sud->rules) s_rules_free(sud->rules);
  sud->rules = s_rul;
}

void s_sudoku_free(s_sudoku sud) {
  free(sud->arr);
  if (sud->rules) s_rules_free(sud->rules);
  free(sud);
}

size_t s_sudoku_coords_to_index(s_sudoku sud, size_t i, size_t j) {
  if (!sud) return -1;
  return i * sud->n + j;
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

      printf(" %d ", sud->arr[ind]);
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

s_cnf s_sudoku_to_cnf(s_sudoku sud) {
  // TODO
  return NULL;
}
