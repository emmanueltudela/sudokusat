#ifndef CNF_H
#define CNF_H

#include <stdlib.h>
#include <stdbool.h>


// ===== STRUCTS =====

// Private cnf formula struct
typedef struct cnf *s_cnf;

// ===================

// ===== BASE FUNCTIONS =====

/* Creates an empty cnf formula and returns it
 */
s_cnf s_cnf_create();

/* Frees the whole struct of the cnf formula
 */
void s_cnf_free(s_cnf cn);

/* Returns a full copy of the cnf formula cn
 *    - cn must be a valid non-null cnf formula
 *
 * Returns NULL on error
 */
s_cnf s_cnf_copy(s_cnf cn);

/* Add a full clause to the cnf formula
 *    - cn must be a valid non-null cnf formula
 *    - litt must be a valid array of litterals of size length
 *      A positive litt such as 10 represents the litteral :
 *          x10
 *      A negative litt such as -5 represents the litteral :
 *         ¬x5
 *      if litt is NULL the array is considered empty and length must be 0
 *      a litt must be != 0
 *    - length must be a valid length >= 0
 *
 * This functions makes a copy of the array litt so you can
 * free it after
 *
 * This function returns a uniq id for the added clause or -1 on failure
 */
int s_cnf_add_clause(s_cnf cn, int *litt, size_t length);

/* Remove the c_id from the cnf formula cn
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *
 * Returns 0 on success and 1 on failure
 */
int s_cnf_remove_clause(s_cnf cn, size_t c_id);

/* Adds litt in the c_id in cnf formula cn
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *    - litt must be a valid litteral (!= 0)
 *
 * Will not add mutliple time the same litt to the same clause
 *    and will not fail when asked to do so
 *
 * Returns 0 on success and 1 on failure
 */
int s_cnf_clause_add_litt(s_cnf cn, size_t c_id, int litt);

/* Removes litt in the c_id in cnf formula cn
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *    - litt must be a valid litteral (!= 0)
 *
 *  Returns 0 on success and 1 on failure
 */
int s_cnf_clause_remove_litt(s_cnf cn, size_t c_id, int litt);

/* Returns whether the given clause c_id is empty (aka contains not litteral)
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *
 * Returns 0 on false, 1 on true and -1 on failure
 */
int s_cnf_clause_empty(s_cnf cn, size_t c_id);

/* Returns whether the given clause c_id is empty
 * (aka contains exactly one litt)
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *
 * Returns 0 on false 1 on true and -1 on failure
 */
int s_cnf_clause_unit(s_cnf cn, size_t c_id);

/* Returns whether or not the clause c_id contains litt
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *    - litt must be a valid litteral (!= 0)
 *
 * Returns 0 on false 1 on true and -1 on failure
 */
int s_cnf_clause_contains_litt(s_cnf cn, size_t c_id, int litt);

// ==========================


// ===== GETTERS =====

/* Returns a pointer to a list of clauses id
 * Stores the size of this array in n
 *      - cn must be a valid non-null cnf formula
 *      - n must be a valid non-null pointer
 *
 * Modifying the returned array doesn't modify the cnf formula.
 *
 * The returned array must be freed by the user
 *
 * This function returns NULL on empty lists
 * If invalid arguments are passed to this function
 * it will return the same output as for an empty
 * list
 */
size_t *s_cnf_get_clauses_ids(s_cnf cn, size_t *n);

/* Returns a pointer to an array of the litterals in clause c_id
 * Stores the size of this array in n
 *    - cn must be a valid non-null cnf formula
 *    - c_id must be the id of a clause in the formula
 *    - n must be a valid non-null pointer
 *
 * Modifying the returned array does't modify the cnf formula.
 *
 * The returned array must be freed by the user
 *
 * This function returns NULL on empty lists
 * If invalid arguments are passed to this function
 * it will return the same output as for an empty
 * list
 */
int *s_cnf_clause_get_litts(s_cnf cn, size_t c_id, size_t *n);

// ===================

// ===== UTILITY FUNCTIONS =====

/* Prints a beautify version of the cnf formula
 *    - cn must be a valid non-null cnf formula
 */
void s_cnf_print(s_cnf cn);

// =============================


#endif
