#include <stdbool.h>

#include "cnf.h"


// ===== UTILITY FUNCTIONS =====

/* Returns the unique litteral of the first unit clause it finds.
 *
 * A unit clause is a clause with only one litteral.
 *
 * If no unit clause found returns 0
 */
int get_unit_clause_litt(s_cnf cn) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  for (int i = 0; i < number_clauses; i++) {
    size_t clause = clauses[i];
    if (s_cnf_clause_unit(cn, clause)) {
      size_t count_litts = 0;
      int *litts = s_cnf_clause_get_litts(cn, clause, &count_litts);

      // Get the litteral of the unit clause
      int litt = litts[0];

      free(litts);
      free(clauses);
      return litt;
    }
  }

  free(clauses);
  return 0;
}

/* Returns the first pure litteral it finds
 *
 * A pure litteral is a litteral that only is only positive or only negative in
 * the formula
 *
 * If none found returns 0
 */
int get_pure_litteral(s_cnf cn) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  // For every clauses
  for (int i = 0; i < number_clauses; i++) {
    size_t clause = clauses[i];

    size_t number_litts = 0;
    int *litts = s_cnf_clause_get_litts(cn, clause, &number_litts);

    // For every litteral in this clause
    for (int j = 0; j < number_litts; j++) {
      int current_litt = litts[j];

      // Check if you can find it's opposite in another clause
      for (int k = 0; k < number_clauses; k++) {
        if (i == k) continue; // Don't check current clause
        int clause_check = clauses[k];
        // If you can find it's opposite in another clause continue searching
        if (s_cnf_clause_contains_litt(cn, clause_check, current_litt * -1)) break;
        // If the current litteral is pure return it
        if (k == number_clauses - 1) {
          free(litts);
          free(clauses);
          return current_litt;
        }
      }

    }

    free(litts);
  }

  free(clauses);
  return 0;
}

/* Returns whether the cnf formula is empty (no clauses) or not
 */
bool cnf_is_empty(s_cnf cn) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  free(clauses);
  return number_clauses == 0;
}

/* Returns whether or not the cnf formula contains an empty clause (no
 * litteral) or not
 */
bool cnf_contains_empty_clause(s_cnf cn) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  for (int i = 0; i < number_clauses; i++) {
    size_t clause = clauses[i];

    if (s_cnf_clause_empty(cn, clause)) {
      free(clauses);
      return true;
    }
  }

  free(clauses);
  return false;
}

/* Returns the first litteral it finds in the formula
 */
int cnf_choose_litteral(s_cnf cn) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  size_t clause = clauses[0];

  size_t number_litts = 0;
  int *litts = s_cnf_clause_get_litts(cn, clause, &number_litts);

  int litt = litts[0];

  free(litts);
  free(clauses);
  return litt;
}

// =============================

// ==== DPLL UTILITY FUNCTIONS =====

// Check the wiki for DPLL

void unit_propagate(s_cnf cn, int litt) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  for (int i = 0; i < number_clauses; i++) {
    size_t clause = clauses[i];
    if (s_cnf_clause_contains_litt(cn, clause, litt)) {
      // If we find a clause that contains that litt we can remove it
      // because it is satisfied
      s_cnf_remove_clause(cn, clause);
    } else if (s_cnf_clause_contains_litt(cn, clause, litt * -1)) {
      // If we find the complement litteral in this clause we can remove
      // the litteral because we will never be able to satisfy it because
      // we already satisfied its complement
      s_cnf_clause_remove_litt(cn, clause, litt * -1);
    }
  }
}

void pure_litteral_assign(s_cnf cn, int litt) {
  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  for (int i = 0; i < number_clauses; i++) {
    size_t clause = clauses[i];
    if (s_cnf_clause_contains_litt(cn, clause, litt)) {
      // If we find a clause that contains that litt we can remove it
      // because every occurence of the pure litteral litt can be
      // satisfied
      s_cnf_remove_clause(cn, clause);
    }
  }
}

// =================================


// ===== MAIN FUNCTIONS =====

bool dpll_internal(s_cnf cn) {
  // Unit propagation
  int unit_litt = 0;
  while ((unit_litt = get_unit_clause_litt(cn)) != 0) {
    unit_propagate(cn, unit_litt);
  }
  // Pure literal elimination
  int pure_litt = 0;
  while ((pure_litt = get_pure_litteral(cn))) {
    pure_litteral_assign(cn, pure_litt);
  }
  // Stopping conditions
  if (cnf_is_empty(cn)) {
    return true;
  }
  if (cnf_contains_empty_clause(cn)) {
    return false;
  }
  // DPLL procedure
  int litt = cnf_choose_litteral(cn);
  s_cnf cn_2 = s_cnf_copy(cn);

  int litt1 = litt;
  int litt2 = -1 * litt;

  s_cnf_add_clause(cn, &litt1, 1);
  s_cnf_add_clause(cn_2, &litt2, 1);

  return dpll_internal(cn) || dpll_internal(cn_2);
}

// ==========================


// ==== BASE FUNCTIONS =====

bool dpll(s_cnf cn) {
  s_cnf cn_copy = s_cnf_copy(cn);
  return dpll_internal(cn_copy);
}

// =========================
