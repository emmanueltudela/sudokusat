#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <assert.h>

#include "cnf.h"
#include "../src/dpll.c"

void test_get_unit_clause_litt() {
  s_cnf cn = s_cnf_create();

  int litt[] = {1, 2, 3};

  s_cnf_add_clause(cn, litt, 3);

  assert(get_unit_clause_litt(cn) == 0);

  int litt2[] = {1};

  s_cnf_add_clause(cn, litt2, 1);

  assert(get_unit_clause_litt(cn) == 1);

  s_cnf_free(cn);
}

void test_get_pure_litteral() {
  s_cnf cn = s_cnf_create();

  int litt[] = {1, 2, 3};
  int litt2[] = {1, -3, -2};
  int litt3[] = {-1};
  int litt4[] = {4, 1, 2};

  s_cnf_add_clause(cn, litt, 3);
  s_cnf_add_clause(cn, litt2, 3);

  assert(get_pure_litteral(cn) == 1);

  s_cnf_add_clause(cn, litt3, 1);

  assert(get_pure_litteral(cn) == 0);

  s_cnf_add_clause(cn, litt4, 3);

  assert(get_pure_litteral(cn) == 4);

  s_cnf_free(cn);
}

void test_cnf_is_empty() {
  s_cnf cn = s_cnf_create();

  assert(cnf_is_empty(cn));

  size_t c_id = s_cnf_add_clause(cn, NULL, 0);

  assert(!cnf_is_empty(cn));

  s_cnf_remove_clause(cn, c_id);

  assert(cnf_is_empty(cn));

  s_cnf_free(cn);
}

void test_cnf_contains_empty_clause() {
  s_cnf cn = s_cnf_create();

  assert(!cnf_contains_empty_clause(cn));

  size_t c_id = s_cnf_add_clause(cn, NULL, 0);

  assert(cnf_contains_empty_clause(cn));

  s_cnf_clause_add_litt(cn, c_id, 1);

  assert(!cnf_contains_empty_clause(cn));

  int litt = 3;
  s_cnf_add_clause(cn, &litt, 1);

  s_cnf_clause_remove_litt(cn, c_id, 1);

  assert(cnf_contains_empty_clause(cn));

  s_cnf_free(cn);
}

void test_cnf_choose_litteral() {
  s_cnf cn = s_cnf_create();

  int litt[] = {1, 2, 3};

  int c_id = s_cnf_add_clause(cn, litt, 3);

  int chosen_litteral = cnf_choose_litteral(cn);
  assert(s_cnf_clause_contains_litt(cn, c_id, chosen_litteral));

  s_cnf_free(cn);
}

void test_unit_propagate() {
  s_cnf cn = s_cnf_create();

  int litt[] = {1, 2, 3};
  int litt2[] = {1};

  s_cnf_add_clause(cn, litt, 3);
  s_cnf_add_clause(cn, litt2, 2);

  unit_propagate(cn, 1);

  assert(cnf_is_empty(cn));

  s_cnf_add_clause(cn, litt, 3);
  s_cnf_add_clause(cn, litt2, 1);

  int litt3[] = {-1, 2};

  s_cnf_add_clause(cn, litt3, 2);

  unit_propagate(cn, 1);

  assert(!cnf_is_empty(cn));

  size_t number_clauses = 0;
  size_t *clauses = s_cnf_get_clauses_ids(cn, &number_clauses);

  assert(number_clauses == 1);
  assert(s_cnf_clause_unit(cn, clauses[0]));
  assert(s_cnf_clause_contains_litt(cn, clauses[0], 2));

  free(clauses);

  s_cnf_free(cn);
}

void test_pure_litteral_assign() {
  s_cnf cn = s_cnf_create();

  int litt[] = {1, 2, 3};
  int litt2[] = {1, -3, -2};
  int litt3[] = {-1};

  s_cnf_add_clause(cn, litt, 3);
  s_cnf_add_clause(cn, litt2, 3);

  pure_litteral_assign(cn, 1);

  assert(cnf_is_empty(cn));

  s_cnf_add_clause(cn, litt, 3);
  s_cnf_add_clause(cn, litt2, 3);
  s_cnf_add_clause(cn, litt3, 1);

  pure_litteral_assign(cn, 1);

  assert(!cnf_is_empty(cn));

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

  if (strcmp(argv[1], "test_get_unit_clause_litt") == 0 || execute_all) {
    test_get_unit_clause_litt();
  }
  if (strcmp(argv[1], "test_get_pure_litteral") == 0 || execute_all) {
    test_get_pure_litteral();
  }
  if (strcmp(argv[1], "test_cnf_is_empty") == 0 || execute_all) {
    test_cnf_is_empty();
  }
  if (strcmp(argv[1], "test_cnf_contains_empty_clause") == 0 || execute_all) {
    test_cnf_contains_empty_clause();
  }
  if (strcmp(argv[1], "test_cnf_choose_litteral") == 0 || execute_all) {
    test_cnf_choose_litteral();
  }
  if (strcmp(argv[1], "test_unit_propagate") == 0 || execute_all) {
    test_unit_propagate();
  }
  if (strcmp(argv[1], "test_pure_litteral_assign") == 0 || execute_all) {
    test_pure_litteral_assign();
  }

  return EXIT_SUCCESS;
}
