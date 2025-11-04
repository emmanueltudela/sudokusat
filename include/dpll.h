#ifndef DPLL_H
#define DPLL_H

#include <stdbool.h>

#include "cnf.h"


// ===== BASE FUNCTIONS =====

/* DPLL SAT SOLVER
 * ref: https://en.wikipedia.org/wiki/DPLL_algorithm#The_algorithm
 */
bool dpll(s_cnf cn);

/* DPLL SAT SOLVER
 * stores the positive valuations in valuations and the number
 * of positive valuations in valuations_length
 *
 * *valuations must be initialized to NULL
 * *valuations_length must be initialized to 0
 */
bool dpll_valuations(s_cnf cn, int **valuations, size_t *valuations_length);

// ==========================


#endif
