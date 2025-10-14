#ifndef DPLL_H
#define DPLL_H

#include <stdbool.h>

#include "cnf.h"


// ===== BASE FUNCTIONS =====

/* DPLL SAT SOLVER
 * ref: https://en.wikipedia.org/wiki/DPLL_algorithm#The_algorithm
 */
bool dpll(s_cnf cn);

// ==========================


#endif
