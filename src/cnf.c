#include <stdlib.h>
#include <stdio.h>

#include <sys/queue.h>

#include "cnf.h"


// ===== STRUCTS =====

// List of litterals
struct litterals {
  int val;
  SLIST_ENTRY(litterals) litterals;
};

SLIST_HEAD(litterals_head, litterals);

// List of clauses (wich contains litterals)
struct clauses {
  size_t id;
  struct litterals_head *litterals;
  SLIST_ENTRY(clauses) clauses;
};

SLIST_HEAD(clauses_head, clauses);

// Cnf formula is a list of clauses
// We hide the slist behind a curtain
typedef struct cnf {
  int clause_id;
  struct clauses_head *clauses;
} *s_cnf;

// ===================


// ===== PRIVATE =====

/* Frees the whole slist of litterals
 */
void free_litterals_slist(struct litterals_head *litterals) {
  while (!SLIST_EMPTY(litterals)) {
    struct litterals *lit = SLIST_FIRST(litterals); // Get first slist elt (head)
    SLIST_REMOVE_HEAD(litterals, litterals);        // Remove head from slist
    free(lit);                                      // Free it
  }
  free(litterals);                                  // Free the slist
}

/* Frees the whole slist of clauses and its
 * lists of litterals
 */
void free_clauses_slist(struct clauses_head *clauses) {
  while (!SLIST_EMPTY(clauses)) {
    struct clauses *cl = SLIST_FIRST(clauses);      // Get first slist elt (head)
    SLIST_REMOVE_HEAD(clauses, clauses);            // Remove head from slist
    free_litterals_slist(cl->litterals);            // Free litterals slist
    free(cl);                                       // Free the clause struct
  }
  free(clauses);                                    // Free the main slist
}

/* Returns a pointer to the given clause by its id
 *
 * Returns NULL when not found
 */
struct clauses *get_clause_by_id(struct clauses_head *clauses, size_t c_id) {
  struct clauses *cl;
  SLIST_FOREACH(cl, clauses, clauses) {
    if (cl->id == c_id) return cl;
  }
  return NULL;
}

/* Return the size of a single clause (aka the number of
 * litterals it contains)
 */
int get_clause_length(struct clauses *cl) {
  int length = 0;
  struct litterals *lit;
  SLIST_FOREACH(lit, cl->litterals, litterals) length++;
  return length;
}

/* Returns the number of clauses in the given cn formula
 */
int get_nb_clauses(s_cnf cn) {
  int nb = 0;
  struct clauses *cl;
  SLIST_FOREACH(cl, cn->clauses, clauses) nb++;
  return nb;
}

// ===================


// ===== BASE FUNCTIONS =====

s_cnf s_cnf_create() {
  s_cnf cn = malloc(sizeof(struct cnf));
  if (!cn) return NULL;

  struct clauses_head *clauses = malloc(sizeof(struct clauses_head));
  if (!clauses) {
    free(cn);
    return NULL;
  }

  SLIST_INIT(clauses);

  cn->clause_id = 0;
  cn->clauses = clauses;
  return cn;
}

void s_cnf_free(s_cnf cn) {
  free_clauses_slist(cn->clauses); // Free every clauses and every litterals
  free(cn);
}

s_cnf s_cnf_copy(s_cnf cn) {
  if (!cn) return NULL;

  s_cnf new_cn = s_cnf_create();
  if (!new_cn) return NULL;

  // Create a copy of each clause and append it to
  // the new list of clauses
  struct clauses *clause;
  SLIST_FOREACH(clause, cn->clauses, clauses) {
    struct clauses *clause_copy = malloc(sizeof(struct clauses));
    if (!clause_copy) {
      s_cnf_free(new_cn);
      return NULL;
    }

    clause_copy->id = clause->id;

    // Create the litterals list for this clause
    clause_copy->litterals = malloc(sizeof(struct litterals_head));
    if (!clause_copy->litterals) {
      s_cnf_free(new_cn);
      return NULL;
    }
    SLIST_INIT(clause_copy->litterals);

    // Create a copy of each litteral and append it
    // to the new list of litterals
    struct litterals *litteral;
    SLIST_FOREACH(litteral, clause->litterals, litterals) {
      struct litterals *litteral_copy = malloc(sizeof(struct litterals));
      if (!litteral_copy) {
        s_cnf_free(new_cn);
        return NULL;
      }

      litteral_copy->val = litteral->val;
      SLIST_INSERT_HEAD(clause_copy->litterals, litteral_copy, litterals);
    }

    SLIST_INSERT_HEAD(new_cn->clauses, clause_copy, clauses);
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
  struct clauses *clause = malloc(sizeof(struct clauses));
  if (!clause) return -1;

  clause->id = cn->clause_id++;

  SLIST_INSERT_HEAD(cn->clauses, clause, clauses);

  // Create a new list of litterals and add it to this clause
  struct litterals_head *litterals = malloc(sizeof(struct litterals_head));
  if (!litterals) {
    free(clause);
    return -1;
  }

  SLIST_INIT(litterals);
  clause->litterals = litterals;

  // If the given list of litterals is empty return directly
  // Nothing to do anymore
  if (length == 0 && litt == NULL) return clause->id;

  // Append every given litterals to the list of litterals
  for (int i = 0; i < length; i++) {
    struct litterals *lit = malloc(sizeof(struct litterals));
    if (!lit) {
      SLIST_REMOVE_HEAD(cn->clauses, clauses); // Remove just inserted clause
      free_litterals_slist(litterals);         // Free the litterals added
      free(clause);                            // Free the allocated clause
      return -1;
    }

    lit->val = litt[i];
    SLIST_INSERT_HEAD(litterals, lit, litterals);
  }

  return clause->id;
}

int s_cnf_remove_clause(s_cnf cn, size_t c_id) {
  if (!cn) return 1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  // Remove the given clause
  SLIST_REMOVE(cn->clauses, cl, clauses, clauses);

  // Free the clause
  free_litterals_slist(cl->litterals);
  free(cl);

  return 0;
}

int s_cnf_clause_add_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return 1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  // If already contains litt don't add it again
  if (s_cnf_clause_contains_litt(cn, c_id, litt) == 1) return 0;

  // Create a new litteral
  struct litterals *lit = malloc(sizeof(struct litterals));
  if (!lit) return 1;

  // Insert the new litteral
  lit->val = litt;
  SLIST_INSERT_HEAD(cl->litterals, lit, litterals);

  return 0;
}

int s_cnf_clause_remove_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return 1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return 1;

  struct litterals *lit;
  SLIST_FOREACH(lit, cl->litterals, litterals) {
    // Delete the matching litt
    if (lit->val == litt) {
      SLIST_REMOVE(cl->litterals, lit, litterals, litterals);
      free(lit);
      break;
    }
  }

  return 0;
}

int s_cnf_clause_empty(s_cnf cn, size_t c_id) {
  if (!cn) return -1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  return SLIST_EMPTY(cl->litterals);
}

int s_cnf_clause_unit(s_cnf cn, size_t c_id) {
  if (!cn) return -1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  return get_clause_length(cl) == 1;
}

int s_cnf_clause_contains_litt(s_cnf cn, size_t c_id, int litt) {
  if (!cn || litt == 0) return -1;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return -1;

  // Search if the given litteral is in the given clause
  struct litterals *lit;
  SLIST_FOREACH(lit, cl->litterals, litterals) {
    if (lit->val == litt) return 1;
  }

  return 0;
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
  struct clauses *cl;
  SLIST_FOREACH(cl, cn->clauses, clauses) {
    clauses_ids[i] = cl->id;
    i++;
  }

  return clauses_ids;
}

int *s_cnf_clause_get_litts(s_cnf cn, size_t c_id, size_t *n) {
  if (!cn || !n) return NULL;

  struct clauses *cl = get_clause_by_id(cn->clauses, c_id);
  if (!cl) return NULL;

  // Size of the array
  *n = get_clause_length(cl);

  // Allocate the array
  int *litts = malloc(sizeof(int) * (*n));
  if (!litts) return NULL;

  int i = 0;
  struct litterals *lit;
  SLIST_FOREACH(lit, cl->litterals, litterals) {
    litts[i] = lit->val;
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
  struct clauses *cl;
  SLIST_FOREACH(cl, cn->clauses, clauses) {

    printf("(");

    // Print every litterals in the clause
    int litt_i = 0;
    int litterals_length = get_clause_length(cl);
    struct litterals *lit;
    SLIST_FOREACH(lit, cl->litterals, litterals) {
      printf(" ");

      if (lit->val < 0) printf("¬");
      printf("x%d ", abs(lit->val));

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
