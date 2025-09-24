#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <string.h>

#include "../src/cnf.c"
#include "../src/sudoku.c"

int s_sudoku_reset_test() {
  s_sudoku sud1 = s_sudoku_create(4);
  if (!sud1) return -1;

  sud1->arr[0] = 9;
  sud1->arr[15] = 9;

  s_sudoku_reset(sud1);

  assert(sud1->arr[0] == 0);
  assert(sud1->arr[15] == 0);

  s_sudoku sud2 = s_sudoku_create(9);
  if (!sud2) return -1;

  sud2->arr[0] = 9;
  sud2->arr[80] = 9;

  s_sudoku_reset(sud2);

  assert(sud2->arr[0] == 0);
  assert(sud2->arr[81] == 0);

  s_sudoku_free(sud1);
  s_sudoku_free(sud2);
  return 0;
}

int s_sudoku_apply_rules_test() {
  s_sudoku sud = s_sudoku_create(4);
  if (!sud) return -1;

  int cells[] = {0, 3};
  int rules[] = {3, 7};
  s_sudoku_set_rules(sud, cells, rules, 2);

  s_sudoku_apply_rules(sud);

  assert(sud->arr[0] == 3);
  assert(sud->arr[3] == 7);

  return 0;
}

int s_sudoku_read_test() {
  s_sudoku sud = s_sudoku_read("../data/sudoku.txt");
  if (!sud) return -1;

  assert(sud->arr[1] == 8);
  assert(sud->arr[32] == 9);
  assert(sud->arr[80] == 5);

  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_set_rules_test() {
  s_sudoku sud = s_sudoku_create(4);
  if (!sud) return -1;

  int cells[] = {0, 3};
  int rules[] = {3, 7};
  s_sudoku_set_rules(sud, cells, rules, 2);

  s_sudoku_apply_rules(sud);

  assert(sud->arr[0] == 3);
  assert(sud->arr[3] == 7);

  return 0;
  return 0;
}

int s_sudoku_coords_to_index_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  assert(s_sudoku_coords_to_index(sud, 0, 0) == 0);
  assert(s_sudoku_coords_to_index(sud, 8, 8) == 80);

  s_sudoku_free(sud);

  sud = s_sudoku_create(4);
  if (!sud) return -1;

  assert(s_sudoku_coords_to_index(sud, 0, 0) == 0);
  assert(s_sudoku_coords_to_index(sud, 3, 3) == 15);

  s_sudoku_free(sud);

  return 0;
}

int s_sudoku_index_to_coords_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  size_t i, j = 0;
  assert(s_sudoku_index_to_coords(sud, 0, &i, &j) == 0);
  assert(i == 0 && j == 0);

  assert(s_sudoku_index_to_coords(sud, 80, &i, &j) == 0);
  assert(i == 8 && j == 8);

  s_sudoku_free(sud);

  sud = s_sudoku_create(4);
  if (!sud) return -1;

  assert(s_sudoku_index_to_coords(sud, 1, &i, &j) == 0);
  assert(i == 0 && j == 1);

  assert(s_sudoku_index_to_coords(sud, 15, &i, &j) == 0);
  assert(i == 3 && j == 3);

  s_sudoku_free(sud);

  return 0;
}

int s_sudoku_sat_encode_litt_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  assert(s_sudoku_sat_encode_litt(sud, 0, 0, 0) == -1);
  assert(s_sudoku_sat_encode_litt(sud, 0, -1, 1) == -1);
  assert(s_sudoku_sat_encode_litt(sud, -1, 1, 1) == -1);
  assert(s_sudoku_sat_encode_litt(sud, 10, 1, 1) == -1);
  assert(s_sudoku_sat_encode_litt(sud, 1, 1, 10) == -1);
  assert(s_sudoku_sat_encode_litt(sud, 1, 10, 1) == -1);

  assert(s_sudoku_sat_encode_litt(sud, 0, 0, 1) == 1);
  assert(s_sudoku_sat_encode_litt(sud, 0, 0, 2) == 2);
  assert(s_sudoku_sat_encode_litt(sud, 0, 1, 1) == 10);

  s_sudoku_free(sud);

  sud = s_sudoku_create(4);
  if (!sud) return -1;

  assert(s_sudoku_sat_encode_litt(sud, 0, 0, 1) == 1);
  assert(s_sudoku_sat_encode_litt(sud, 0, 0, 2) == 2);
  assert(s_sudoku_sat_encode_litt(sud, 0, 1, 1) == 5);

  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_sat_decode_litt_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  size_t i, j, v = 0;

  assert(s_sudoku_sat_decode_litt(sud, 1, &i, &j, &v) == 0);
  assert(i == 0 && j == 0 && v == 1);

  assert(s_sudoku_sat_decode_litt(sud, 2, &i, &j, &v) == 0);
  assert(i == 0 && j == 0 && v == 2);

  assert(s_sudoku_sat_decode_litt(sud, 10, &i, &j, &v) == 0);
  assert(i == 0 && j == 1 && v == 1);

  s_sudoku_free(sud);

  sud = s_sudoku_create(4);
  if (!sud) return -1;

  assert(s_sudoku_sat_decode_litt(sud, 1, &i, &j, &v) == 0);
  assert(i == 0 && j == 0 && v == 1);

  assert(s_sudoku_sat_decode_litt(sud, 2, &i, &j, &v) == 0);
  assert(i == 0 && j == 0 && v == 2);

  assert(s_sudoku_sat_decode_litt(sud, 5, &i, &j, &v) == 0);
  assert(i == 0 && j == 1 && v == 1);

  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_get_line_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  int *line = s_sudoku_get_line(sud, 0);
  if (!line) return -1;

  int expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  for (int i = 0; i < sud->n; i++) {
    assert(line[i] == expected[i]);
  }

  free(line);

  line = s_sudoku_get_line(sud, 1);
  if (!line) return -1;

  int expected2[] = {9, 10, 11, 12, 13, 14, 15, 16, 17};
  for (int i = 0; i < sud->n; i++) {
    assert(line[i] == expected2[i]);
  }

  free(line);
  s_sudoku_free(sud);

  sud = s_sudoku_create(4);

  line = s_sudoku_get_line(sud, 1);
  if (!line) return -1;

  int expected3[] = {4, 5, 6, 7};
  for (int i = 0; i < sud->n; i++) {
    assert(line[i] == expected3[i]);
  }

  free(line);
  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_get_col_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  int *col = s_sudoku_get_col(sud, 0);
  if (!col) return -1;

  int expected[] = {0, 9, 18, 27, 36, 45, 54, 63, 72};
  for (int i = 0; i < sud->n; i++) {
    assert(col[i] == expected[i]);
  }

  free(col);

  col = s_sudoku_get_col(sud, 1);
  if (!col) return -1;

  int expected2[] = {1, 10, 19, 28, 37, 46, 55, 64, 73};
  for (int i = 0; i < sud->n; i++) {
    assert(col[i] == expected2[i]);
  }

  free(col);
  s_sudoku_free(sud);

  sud = s_sudoku_create(4);

  col = s_sudoku_get_col(sud, 1);
  if (!col) return -1;

  int expected3[] = {1, 5, 9, 13};
  for (int i = 0; i < sud->n; i++) {
    assert(col[i] == expected3[i]);
  }

  free(col);
  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_get_block_test() {
  s_sudoku sud = s_sudoku_create(9);
  if (!sud) return -1;

  int *block = s_sudoku_get_block(sud, 0);
  if (!block) return -1;

  int expected[] = {0, 1, 2, 9, 10, 11, 18, 19, 20};
  for (int i = 0; i < sud->n; i++) {
    assert(block[i] == expected[i]);
  }

  free(block);

  block = s_sudoku_get_block(sud, 4);
  if (!block) return -1;

  int expected2[] = {30, 31, 32, 39, 40, 41, 48, 49, 50};
  for (int i = 0; i < sud->n; i++) {
    assert(block[i] == expected2[i]);
  }

  free(block);
  s_sudoku_free(sud);

  sud = s_sudoku_create(4);

  block = s_sudoku_get_block(sud, 1);
  if (!block) return -1;

  int expected3[] = {2, 3, 6, 7};
  for (int i = 0; i < sud->n; i++) {
    assert(block[i] == expected3[i]);
  }

  free(block);
  s_sudoku_free(sud);
  return 0;
}

int s_sudoku_cnf_constraints_test() {
  return 0;
}

int s_sudoku_cnf_values_test() {
  return 0;
}

int s_sudoku_cnf_uniq_test() {
  return 0;
}

int s_sudoku_cnf_set_complete_test() {
  return 0;
}

int s_sudoku_cnf_set_uniq_test() {
  return 0;
}

int s_sudoku_cnf_line_complete_test() {
  return 0;
}

int s_sudoku_cnf_line_uniq_test() {
  return 0;
}

int s_sudoku_cnf_col_complete_test() {
  return 0;
}

int s_sudoku_cnf_col_uniq_test() {
  return 0;
}

int s_sudoku_cnf_cel_complete_test() {
  return 0;
}

int s_sudoku_cnf_cel_uniq_test() {
  return 0;
}

// int s_sudoku_to_cnf_test() {}

void usage(char *exec) {
  printf("%s test_name : Execute given test the program will fail if the test fails\n", exec);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  char *testname = argv[1];
  if (strcmp(testname, "s_sudoku_reset") == 0) {
    assert(s_sudoku_reset_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_apply_rules") == 0) {
    assert(s_sudoku_apply_rules_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_read") == 0) {
    assert(s_sudoku_read_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_set_rules") == 0) {
    assert(s_sudoku_set_rules_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_coords_to_index") == 0) {
    assert(s_sudoku_coords_to_index_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_index_to_coords") == 0) {
    assert(s_sudoku_index_to_coords_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_sat_encode_litt") == 0) {
    assert(s_sudoku_sat_encode_litt_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_sat_decode_litt") == 0) {
    assert(s_sudoku_sat_decode_litt_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_get_line") == 0) {
    assert(s_sudoku_get_line_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_get_col") == 0) {
    assert(s_sudoku_get_col_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_get_block") == 0) {
    assert(s_sudoku_get_block_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_constraints") == 0) {
    assert(s_sudoku_cnf_constraints_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_values") == 0) {
    assert(s_sudoku_cnf_values_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_uniq") == 0) {
    assert(s_sudoku_cnf_uniq_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_set_complete") == 0) {
    assert(s_sudoku_cnf_set_complete_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_set_uniq") == 0) {
    assert(s_sudoku_cnf_set_uniq_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_line_complete") == 0) {
    assert(s_sudoku_cnf_line_complete_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_line_uniq") == 0) {
    assert(s_sudoku_cnf_line_uniq_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_col_complete") == 0) {
    assert(s_sudoku_cnf_col_complete_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_col_uniq") == 0) {
    assert(s_sudoku_cnf_col_uniq_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_cel_complete") == 0) {
    assert(s_sudoku_cnf_cel_complete_test() == 0);
  }
  if (strcmp(testname, "s_sudoku_cnf_cel_uniq") == 0) {
    assert(s_sudoku_cnf_cel_uniq_test() == 0);
  }
}
