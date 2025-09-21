#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "cnf.h"


// ==== PRIVATE ====

// Create a new clause
s_clause s_clause_create(int *litt, size_t len) {
  s_clause cl = malloc(sizeof(struct clause));
  if (!cl) return NULL;

  cl->len = len;
  cl->litt = litt;
  return cl;
}

// Frees a clause
void s_clause_free(s_clause cl) {
  free(cl->litt);
  free(cl);
}

// =================


// Base functions

s_cnf s_cnf_create() {
  s_cnf cn = malloc(sizeof(struct cnf));
  if (!cn) return NULL;
  cn->clauses = NULL;
  cn->len = 0;
  cn->size = 0;
  return cn;
}

void s_cnf_free(s_cnf cn) {
  for (int i = 0; i < cn->len; i++) {
    s_clause_free(cn->clauses[i]);
  }
  free(cn->clauses);
  free(cn);
}

int s_cnf_add_clause(s_cnf cn, int *litt, size_t len) {
  if (!cn || !litt) return -1;

  // Realloc the array of clauses if needed
  if (cn->len >= cn->size) {
    // Special case if the current size is zero (not allocated yet)
    if (cn->size == 0)
      cn->size = DEFAULT_CNF_CLAUSES_SIZE;
    else
      cn->size *= 2;

    cn->clauses = realloc(cn->clauses, cn->size * sizeof(s_clause));
    if (!cn->clauses) return -1;
  }

  // Make a copy of the given litterals
  int *l = malloc(sizeof(int) * len);
  if (!l) return 1;
  memcpy(l, litt, len * sizeof(int));

  // Create a new clause
  s_clause cl = s_clause_create(l, len);
  if (!cl) return -1;

  // Append
  cn->clauses[cn->len] = cl;
  cn->len += 1;
  return 0;
}


// Utility functions

void s_cnf_print(s_cnf cn) {
  for (int i = 0; i < cn->len; i++) {
    printf("(");
    for (int j = 0; j < cn->clauses[i]->len; j++) {
      printf(" ");
      if (cn->clauses[i]->litt[j] < 0) printf("¬");
      printf("x%d ", abs(cn->clauses[i]->litt[j]));
      if (j != cn->clauses[i]->len - 1) printf("∨");
    }
    printf(")");
    if (i != cn->len - 1) printf(" ∧ ");
  }
  printf("\n");
}
