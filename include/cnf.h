#ifndef __CNF_H__
#define __CNF_H__

#include <stdlib.h>

#define DEFAULT_CNF_CLAUSES_SIZE 10

typedef struct clause {
  int *litt;  // Litterals in the clause (+1 is positiv litt, -1 is negation)
  size_t len; // Number of litterals
} *s_clause;

typedef struct cnf {
  s_clause *clauses; // Set of clauses
  size_t len;        // Number of clauses
  size_t size;       // Size of the arr (for realloc)
} *s_cnf;


// Base functions

// Allocate a cnf struct
s_cnf s_cnf_create();

// Add a clause to the cnf (Return EXIT_FAILURE if failed EXIT_SUCCESS otherwise)
int s_cnf_add_clause(s_cnf cn, int *litt, size_t len);

// Frees the cnf struct
void s_cnf_free(s_cnf cn);


// Utility functions

// Print cnf formula
void s_cnf_print(s_cnf cn);

#endif
