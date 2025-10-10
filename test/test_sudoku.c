#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "sudoku.h"

void test_s_grid_create() {
  s_grid g = s_grid_create(4);
  assert(g);
  assert(s_grid_size(g) == 4);
  assert(s_grid_get_cell_value(g, 1, 1) == 0); // Check 0 initialized
  s_grid_free(g);

  g = s_grid_create(9);
  assert(g);
  assert(s_grid_size(g) == 9);
  assert(s_grid_get_cell_value(g, 1, 1) == 0); // Check 0 initialized
  s_grid_free(g);

  g = s_grid_create(-1);    // Invalid size
  assert(!g);

  g = s_grid_create(1);     // Invalid size
  assert(!g);

  g = s_grid_create(5);     // Invalid size
  assert(!g);
}

void test_s_grid_free() {
  s_grid g = s_grid_create(4);
  assert(g);
  s_grid_free(g);
}

void test_s_grid_size() {
  s_grid g = s_grid_create(4);
  assert(g);
  assert(s_grid_size(g) == 4);      // Expected behavior
  s_grid_free(g);

  g = s_grid_create(9);
  assert(g);
  assert(s_grid_size(g) == 9);      // Expected behavior
  s_grid_free(g);

  assert(s_grid_size(NULL) == 0);   // Invalid grid
}

void test_s_grid_get_cell_value() {
  s_grid g = s_grid_create(4);
  assert(g);

  s_grid_set_cell_value(g, 0, 0, 1);
  assert(s_grid_get_cell_value(g, 0, 0) == 1);   // Expected behavior
  assert(s_grid_get_cell_value(g, 0, 1) == 0);   // Expected behavior

  assert(s_grid_get_cell_value(g, -1, 0) == -1); // Invalid line
  assert(s_grid_get_cell_value(g, 0, 5) == -1);  // Invalid col

  s_grid_free(g);
}

void test_s_grid_set_cell_value() {
  s_grid g = s_grid_create(4);
  assert(g);

  assert(s_grid_set_cell_value(g, 0, 0, 1) == 0);   // Expected behavior
  assert(s_grid_get_cell_value(g, 0, 0) == 1);
  assert(s_grid_set_cell_value(g, 1, 1, 2) == 0);   // Expected behavior
  assert(s_grid_get_cell_value(g, 1, 1) == 2);
  assert(s_grid_set_cell_value(g, 0, 0, 0) == 0);   // Expected behavior (empty cell)
  assert(s_grid_set_cell_value(g, 0, 0, -1) == -1); // Invalid val
  assert(s_grid_set_cell_value(g, 0, 0, 5) == -1);  // Invalid val
  assert(s_grid_set_cell_value(g, 0, -1, 0) == -1); // Invalid col
  assert(s_grid_set_cell_value(g, 18, 0, 0) == -1); // Invalid line

  s_grid_free(g);
}

void test_s_grid_create_from_file() {
  s_grid g = s_grid_create_from_file("../data/test.txt");
  assert(g);

  assert(s_grid_size(g) == 4);
  assert(s_grid_get_cell_value(g, 2, 1) == 3);
  assert(s_grid_get_cell_value(g, 0, 0) == 0);

  s_grid_free(g);

  g = s_grid_create_from_file("../data/invalid_sudoku.txt");
  assert(!g);

  g = s_grid_create_from_file("non-existing-file");
  assert(!g);
}

void test_s_grid_print() {
  s_grid g = s_grid_create(4);
  assert(g);

  assert(s_grid_print(stdout, g) == 0);
  assert(s_grid_print(NULL, g) == -1);

  s_grid_free(g);
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

  if (strcmp(argv[1], "test_s_grid_create") == 0 || execute_all) {
   test_s_grid_create();
  }
  if (strcmp(argv[1], "test_s_grid_free") == 0 || execute_all) {
    test_s_grid_free();
  }
  if (strcmp(argv[1], "test_s_grid_size") == 0 || execute_all) {
    test_s_grid_size();
  }
  if (strcmp(argv[1], "test_s_grid_get_cell_value") == 0 || execute_all) {
    test_s_grid_get_cell_value();
  }
  if (strcmp(argv[1], "test_s_grid_set_cell_value") == 0 || execute_all) {
    test_s_grid_set_cell_value();
  }
  if (strcmp(argv[1], "test_s_grid_create_from_file") == 0 || execute_all) {
    test_s_grid_create_from_file();
  }
  if (strcmp(argv[1], "test_s_grid_print") == 0 || execute_all) {
    test_s_grid_print();
  }
  return EXIT_SUCCESS;
}
