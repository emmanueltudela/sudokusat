#ifndef __CNF_H__
#define __CNF_H__

#include <stdlib.h>

#define DEFAULT_CNF_CLAUSES_SIZE 10


// Structs

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

// Frees the cnf struct
void s_cnf_free(s_cnf cn);

// Add a clause to the cnf (Return -1 if failed 0 otherwise)
int s_cnf_add_clause(s_cnf cn, int *litt, size_t len);


// Utility functions

// Print pretty cnf formula
void s_cnf_print(s_cnf cn);

#endif
