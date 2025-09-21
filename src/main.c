#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sudoku.h"
#include "cnf.h"

void usage(char *exec) {
  printf("%s -h : displays this menu\n", exec);
  printf("%s -f filename : load sudoku using given file\n\t(see data/sudoku.txt for an example or read include/sudoku.h\n\tto see a description of the format)\n", exec);
}

int main(int argc, char **argv) {
  char *filename = NULL;

  // Handle arguments
  if (argc != 1) {
    if (strcmp(argv[1], "-h") == 0) {
      usage(argv[0]);
      exit(EXIT_SUCCESS);
    } else if (strcmp(argv[1], "-f") == 0) {
      if (argc != 3) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
      }
      filename = argv[2];
    }
  }

  s_sudoku sud = NULL;

  // Load grid from file or create a default empty grid
  if (filename) {
    sud = s_sudoku_read(filename);
    if (!sud) {
      fprintf(stderr, "Could not open %s or invalid file format\n", filename);
      exit(EXIT_FAILURE);
    }
  } else {
    sud = s_sudoku_create(9);
  }

  // Temporary tests

  s_sudoku_print(sud);

  s_cnf cn = s_cnf_create();
  int litt1[] = {1, 2, 3};
  int litt2[] = {-2, 3, -1};
  s_cnf_add_clause(cn, litt1, 3);
  s_cnf_add_clause(cn, litt2, 3);
  s_cnf_print(cn);

  s_sudoku_free(sud);
  s_cnf_free(cn);
  return EXIT_SUCCESS;
}
