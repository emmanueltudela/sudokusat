#include <stdlib.h>
#include <stdio.h>

#include <sys/queue.h>

#include "cnf.h"
#include "uthash/uthash.h"


// ===== STRUCTS =====

struct litteral {
  int litt;
  UT_hash_handle hh;
};

// Clause (wich contains litterals)
struct clause {
  size_t id;
  struct litteral *litterals;
  UT_hash_handle hh;
};

// Cnf formula is a list of clauses
// We hide the hashtable behind the curtain
typedef struct cnf {
  int current_clause_id;
  struct clause *clauses;
} *s_cnf;

// ===================


// ===== PRIVATE =====

/* Frees the whole hashmap of litterals and its
 * lists of litterals
 */
void free_litterals(struct litteral *litterals) {
  struct litteral *current_litteral, *tmp;

  HASH_ITER(hh, litterals, current_litteral, tmp) {
    HASH_DEL(litterals, current_litteral);
    free(current_litteral);
  }

  free(litterals);
}

/* Frees the whole hashmap of clauses and its
 * lists of litterals
 */
void free_clauses(struct clause *clauses) {
  struct clause *current_clause, *tmp;

  HASH_ITER(hh, clauses, current_clause, tmp) {
    HASH_DEL(clauses, current_clause);
    free_litterals(current_clause->litterals);
    free(current_clause);
  }

  free(clauses);
}

/* Returns a pointer to the given clause by its id
 *
 * Returns NULL when not found
 */
struct clause *get_clause_by_id(struct clause *clauses, size_t c_id) {
  struct clause *clause;

  HASH_FIND_INT(clauses, &c_id, clause);
  if (!clause) return NULL;
  return clause;
}

/* Return the size of a single clause (aka the number of
 * litterals it contains)
 */
int get_clause_length(struct clause *cl) {
  return HASH_COUNT(cl->litterals);
}

/* Returns the number of clauses in the given cn formula
 */
int get_nb_clauses(s_cnf cn) {
  return HASH_COUNT(cn->clauses);
}

// ===================


// ===== BASE FUNCTIONS =====

s_cnf s_cnf_create() {
  s_cnf cn = malloc(sizeof(struct cnf));
  if (!cn) return NULL;

  cn->current_clause_id = 0;
  cn->clauses = NULL;
  return cn;
}

void s_cnf_free(s_cnf cn) {
  free_clauses(cn->clauses);
  free(cn);
}

s_cnf s_cnf_copy(s_cnf cn) {
  if (!cn) return NULL;

  s_cnf new_cn = s_cnf_create();

  // Create a copy of each clause and append it to
  // the new hashmap of clauses
  struct clause *current_clause, *tmp;
  HASH_ITER(hh, cn->clauses, current_clause, tmp) {
    struct clause *clause_copy = malloc(sizeof(struct clause));
    if (!clause_copy) {
      s_cnf_free(new_cn);
      return NULL;
    }

    clause_copy->id = current_clause->id;
    clause_copy->litterals = NULL;

    // Create a copy of each litteral and append it
    // to the new hashmap of litterals
    struct litteral *current_litteral, *tmp;
    HASH_ITER(hh, current_clause->litterals, current_litteral, tmp) {
      struct litteral *litteral_copy = malloc(sizeof(struct litteral));
      if (!litteral_copy) {
        s_cnf_free(new_cn);
        return NULL;
      }

      litteral_copy->litt = current_litteral->litt;
      HASH_ADD_INT(clause_copy->litterals, litt, litteral_copy);
    }

    HASH_ADD_INT(new_cn->clauses, id, clause_copy);
  }

  return new_cn;
}

int s_cnf_add_clause(s_cnf cn, int *litt, size_t length) {
  // Return -1 on invalid parameters
  if (!cn || (!litt && length > 0) || length < 0) return -1;

  // Check that litterals are valid
  for (int i = 0; i < length; i++)
    if (litt[i] == 0) return -1;

  // Create a new clause and add it to the list of clauses
  struct clause *clause = malloc(sizeof(struct clause));
  if (!clause) return -1;

  clause->id = cn->current_clause_id++;
  clause->litterals = NULL;

  HASH_ADD_INT(cn->clauses, id, clause);

  // If the given list of litterals is empty return directly
  // Nothing to do anymore
  if (length == 0 && litt == NULL) return clause->id;

  // Append every given litterals to the list of litterals
  for (int i = 0; i < length; i++) {
    struct litteral *litteral = malloc(sizeof(struct litteral));
    if (!litteral) {
      HASH_DEL(cn->clauses, clause);      // Remove just inserted clause
      free_litterals(clause->litterals);  // Free the litterals
      free(clause);                       // Free the allocated clause
      return -1;
    }

    litteral->litt = litt[i];
    HASH_ADD_INT(clause->litterals, litt, litteral);
  }

  return clause->id;
}

int s_cnf_remove_clause(s_cnf cn, size_t c_id) {
  if (!cn) return 1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  // Remove the given clause
  HASH_DEL(cn->clauses, cl);

  // Free the clause
  free_litterals(cl->litterals);
  free(cl);

  return 0;
}

int s_cnf_clause_add_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return 1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  // If already contains litt don't add it again
  if (s_cnf_clause_contains_litt(cn, c_id, litt) == 1) return 0;

  // Create a new litteral
  struct litteral *litteral = malloc(sizeof(struct litteral));
  if (!litteral) return 1;

  // Insert the new litteral
  litteral->litt = litt;
  HASH_ADD_INT(cl->litterals, litt, litteral);

  return 0;
}

int s_cnf_clause_remove_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return 1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  if (s_cnf_clause_contains_litt(cn, c_id, litt) == 1) {
    struct litteral *litteral;
    HASH_FIND_INT(cl->litterals, &litt, litteral);
    HASH_DEL(cl->litterals, litteral);
  }

  return 0;
}

int s_cnf_clause_empty(s_cnf cn, size_t c_id) {
  if (!cn) return -1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  if (!cl->litterals)
    return true;
  else
    return HASH_COUNT(cl->litterals) == 0;
}

int s_cnf_clause_unit(s_cnf cn, size_t c_id) {
  if (!cn) return -1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  return HASH_COUNT(cl->litterals) == 1;
}

int s_cnf_clause_contains_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return -1;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  struct litteral *litteral;
  HASH_FIND_INT(cl->litterals, &litt, litteral);

  return litteral != NULL;
}

// ==========================


// ===== GETTERS =====

size_t *s_cnf_get_clauses_ids(s_cnf cn, size_t *n) {
  if (!cn || !n) return NULL;

  // Size of the array
  *n = get_nb_clauses(cn);

  // Allocate array
  size_t *clauses_ids = malloc(sizeof(size_t) * (*n));
  if (!clauses_ids) return NULL;

  int i = 0;
  struct clause *current_clause, *tmp;
  HASH_ITER(hh, cn->clauses, current_clause, tmp) {
    clauses_ids[i] = current_clause->id;
    i++;
  }

  return clauses_ids;
}

int *s_cnf_clause_get_litts(s_cnf cn, size_t c_id, size_t *n) {
  if (!cn || !n) return NULL;

  struct clause *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return NULL;

  // Size of the array
  *n = get_clause_length(cl);

  // Allocate the array
  int *litts = malloc(sizeof(int) * (*n));
  if (!litts) return NULL;

  int i = 0;
  struct litteral *current_litteral, *tmp;
  HASH_ITER(hh, cl->litterals, current_litteral, tmp) {
    litts[i] = current_litteral->litt;
    i++;
  }

  return litts;
}

// ===================


// ===== UTILITY FUNCTIONS =====

void s_cnf_print(s_cnf cn) {
  if (!cn) return;

  // Print every clause
  int nb_clauses = get_nb_clauses(cn);
  int clause_i = 0;

  struct clause *current_clause, *tmp;
  HASH_ITER(hh, cn->clauses, current_clause, tmp) {

    printf("(");

    // Print every litterals in the clause
    int litt_i = 0;
    int litterals_length = get_clause_length(current_clause);

    struct litteral *current_litteral, *tmp;
    HASH_ITER(hh, current_clause->litterals, current_litteral, tmp) {
      printf(" ");

      if (current_litteral->litt < 0) printf("¬");
      printf("x%d ", abs(current_litteral->litt));

      if (litt_i < litterals_length - 1) printf("∨");

      litt_i++;
    }

    printf(")");

    if (clause_i < nb_clauses - 1) printf(" ∧ ");

    clause_i++;
  }

  printf("\n");
}

// =============================
