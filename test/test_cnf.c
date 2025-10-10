#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <assert.h>

#include "cnf.h"

void test_s_cnf_create() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  s_cnf_free(cn);
}

void test_s_cnf_free() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  s_cnf_free(cn);
  assert(!cn);
}

void test_s_cnf_copy() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2, -3};
  size_t c_id = s_cnf_add_clause(cn, litt, 3);

  s_cnf cnc = s_cnf_copy(cn);       // Valid call
  assert(cnc);

  s_cnf_free(cn);

  // Check equality
  assert(s_cnf_clause_contains_litt(cnc, c_id, 1) == 1);
  assert(s_cnf_clause_contains_litt(cnc, c_id, 2) == 1);
  assert(s_cnf_clause_contains_litt(cnc, c_id, -3) == 1);

  assert(s_cnf_copy(NULL) == NULL); // Invalid formula

  s_cnf_free(cnc);
}

void test_s_cnf_add_clause() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2, -3};
  int c_id1 = s_cnf_add_clause(cn, litt, 3);
  int c_id2 = s_cnf_add_clause(cn, NULL, 0);
  assert(c_id1 != c_id2);
  assert(c_id1 != -1);
  assert(c_id2 != -1);

  assert(!s_cnf_clause_empty(cn, c_id1));            // clause1 is not empty
  assert(s_cnf_clause_contains_litt(cn, c_id1, 2));  // clause1 contains x2
  assert(!s_cnf_clause_contains_litt(cn, c_id1, 4)); // clause1 does not contain x4
  assert(s_cnf_clause_empty(cn, c_id2));             // clause2 is empty

  assert(s_cnf_add_clause(NULL, NULL, 0) != -1);     // Can add empty clause
  assert(s_cnf_add_clause(NULL, NULL, 1) == -1);     // Invalid empty array of size 1

  int litt2[] = {0, -1};
  assert(s_cnf_add_clause(cn, litt2, 2) == -1);      // litteral array contains invalid litteral (0)

  s_cnf_free(cn);
}

void test_s_cnf_remove_clause() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  s_cnf_remove_clause(cn, 0);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);
  assert(s_cnf_remove_clause(cn, c_id) == 0); // Valid remove

  size_t nb_clauses = 0;
  size_t *clauses = s_cnf_get_clauses(cn, &nb_clauses);
  assert(nb_clauses == 0);

  assert(s_cnf_remove_clause(cn, c_id + 1) == 1); // Invalid call

  free(clauses);

  s_cnf_free(cn);
}

void test_s_cnf_clause_add_litt() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  size_t c_id = s_cnf_add_clause(cn, NULL, 0);

  assert(!s_cnf_clause_contains_litt(cn, c_id, 1));
  assert(s_cnf_clause_add_litt(cn, c_id, 1) == 0);    // Valid add
  assert(s_cnf_clause_contains_litt(cn, c_id, 1));

  assert(s_cnf_clause_add_litt(cn, c_id + 1, 1) == 1);    // Invalid c_id
  assert(s_cnf_clause_add_litt(cn, c_id, 0) == 1);   // Invalid litt

  s_cnf_free(cn);
}

void test_s_cnf_clause_remove_litt() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);

  assert(s_cnf_clause_contains_litt(cn, c_id, 1));
  assert(s_cnf_clause_contains_litt(cn, c_id, 2));
  assert(s_cnf_clause_remove_litt(cn, c_id, 1) == 0); // Valid remove
  assert(!s_cnf_clause_contains_litt(cn, c_id, 1));
  assert(s_cnf_clause_contains_litt(cn, c_id, 2));

  assert(s_cnf_clause_remove_litt(cn, c_id + 1, 1) == 1); // Invalid c_id
  assert(s_cnf_clause_remove_litt(cn, c_id, 0) == 1);     // Invalid litt

  s_cnf_free(cn);
}

void test_s_cnf_clause_contains_litt() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);

  assert(s_cnf_clause_contains_litt(cn, c_id, 1) == 1);       // Valid call
  assert(s_cnf_clause_contains_litt(cn, c_id, 2) == 1);       // Valid call
  s_cnf_clause_remove_litt(cn, c_id, 1);
  assert(s_cnf_clause_contains_litt(cn, c_id, 1) == 0);       // Valid call
  assert(s_cnf_clause_contains_litt(cn, c_id, 2) == 1);       // Valid call

  assert(s_cnf_clause_contains_litt(cn, c_id, 0) == -1);      // Invalid litt
  assert(s_cnf_clause_contains_litt(cn, c_id + 1, 1) == -1);  // Invalid c_id

  s_cnf_free(cn);
}

void test_s_cnf_clause_empty() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  size_t c_id = s_cnf_add_clause(cn, NULL, 0);

  assert(s_cnf_clause_empty(cn, c_id) == 1);        // Valid call

  s_cnf_clause_add_litt(cn, c_id, 1);

  assert(s_cnf_clause_empty(cn, c_id) == 0);        // Valid call

  assert(s_cnf_clause_empty(cn, c_id + 1) == -1);   // Invalid c_id

  s_cnf_free(cn);
}

void test_s_cnf_clause_unit() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);

  assert(s_cnf_clause_unit(cn, c_id) == 0);       // Valid call

  s_cnf_clause_remove_litt(cn, c_id, 2);

  assert(s_cnf_clause_unit(cn, c_id) == 1);       // Valid call

  assert(s_cnf_clause_unit(cn, c_id + 1) == -1);  // Invalid c_id

  s_cnf_free(cn);
}

void test_s_cnf_get_clauses() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt1[] = {1, 2};
  size_t c_id1 = s_cnf_add_clause(cn, litt1, 2);
  int litt2[] = {1};
  size_t c_id2 = s_cnf_add_clause(cn, litt2, 1);

  size_t clauses_length = 0;
  size_t *clauses = s_cnf_get_clauses(cn, &clauses_length);
  assert(clauses);                          // Valid call
  assert(clauses_length == 2);

  assert(s_cnf_clause_contains_litt(cn, c_id1, 1) == 1);
  assert(s_cnf_clause_contains_litt(cn, c_id2, 1) == 1);
  assert(s_cnf_clause_contains_litt(cn, c_id1, 2) == 1);
  assert(s_cnf_clause_contains_litt(cn, c_id2, 2) == 0);

  assert(!s_cnf_get_clauses(cn, NULL));     // Invalid call

  free(clauses);
  s_cnf_free(cn);
}

void test_s_cnf_clause_get_litts() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);

  size_t litts_length = 0;
  int *litts = s_cnf_clause_get_litts(cn, c_id, &litts_length);
  assert(litts);                          // Valid call
  assert(litts_length == 2);

  // Check integrity
  for (int i = 0; i < litts_length; i++) {
    assert(litts[i] == litt[i]);
  }

  assert(!s_cnf_clause_get_litts(cn, c_id + 1, &litts_length));     // Invalid call
  assert(!s_cnf_clause_get_litts(cn, c_id, NULL));                  // Invalid call

  free(litts);
  s_cnf_free(cn);
}

void test_s_cnf_print() {
  s_cnf cn = s_cnf_create();
  assert(cn);

  s_cnf_print(cn);

  int litt[] = {1, 2};
  size_t c_id = s_cnf_add_clause(cn, litt, 2);

  s_cnf_print(cn);

  s_cnf_free(cn);
}

void usage(char *exec) {
  printf("%s testname     -> Execute the given testname\n", exec);
  printf("%s all    -> Execute every tests\n", exec);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  bool execute_all = strcmp(argv[1], "all") == 0;

  if (strcmp(argv[1], "test_s_cnf_create") == 0 || execute_all) {
    test_s_cnf_create();
  }
  if (strcmp(argv[1], "test_s_cnf_free") == 0 || execute_all) {
    test_s_cnf_free();
  }
  if (strcmp(argv[1], "test_s_cnf_copy") == 0 || execute_all) {
    test_s_cnf_copy();
  }
  if (strcmp(argv[1], "test_s_cnf_add_clause") == 0 || execute_all) {
    test_s_cnf_add_clause();
  }
  if (strcmp(argv[1], "test_s_cnf_remove_clause") == 0 || execute_all) {
    test_s_cnf_remove_clause();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_add_litt") == 0 || execute_all) {
    test_s_cnf_clause_add_litt();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_remove_litt") == 0 || execute_all) {
    test_s_cnf_clause_remove_litt();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_empty") == 0 || execute_all) {
    test_s_cnf_clause_empty();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_unit") == 0 || execute_all) {
    test_s_cnf_clause_unit();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_remove_litt") == 0 || execute_all) {
    test_s_cnf_clause_remove_litt();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_contains_litt") == 0 || execute_all) {
    test_s_cnf_clause_contains_litt();
  }
  if (strcmp(argv[1], "test_s_cnf_get_clauses") == 0 || execute_all) {
    test_s_cnf_get_clauses();
  }
  if (strcmp(argv[1], "test_s_cnf_clause_get_litts") == 0 || execute_all) {
    test_s_cnf_clause_get_litts();
  }
  if (strcmp(argv[1], "test_s_cnf_print") == 0 || execute_all) {
    test_s_cnf_print();
  }

  return EXIT_SUCCESS;
}
