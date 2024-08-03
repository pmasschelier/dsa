#ifndef COMPARE_H
#define COMPARE_H

/**
 * @file compare.h
 * @brief Comparison function type definition
 * Defines compare_fn_t type function
 * @ingroup avl_tree_ref heap_view
 */

/**
 * @typedef compare_fn_t
 * @brief Typedef for pointer to a comparison function
 *
 * A comparison function takes to pointers as arguments, it should return -1 if
 * the first element is smaller than the second, 1 if the first element is
 * greater and 0 if they are equals. Any other value returned may lead to
 * Undefined Behavior.
 */
typedef int (*compare_fn_t)(void*, void*);

/**
 * @brief Defines a function to compare scalar values
 * @param type The type of the values we want to compare
 * For instance calling this macro with `DEFINE_COMPARE_SCALAR(int)` will define
 * the function compare_int which will be the accurate function to compare two
 * ints.
 */
#define DEFINE_COMPARE_SCALAR(type)                               \
	static inline int compare_##type(void* a, void* b) {          \
		return (*(type*)a > *(type*)b) - (*(type*)a < *(type*)b); \
	}

/**
 * @brief Defines a function to compare scalar values
 * @param type The type of the values we want to compare
 * For instance calling this macro with `DEFINE_COMPARE_SCALAR(int)` will define
 * the function compare_int which will be the accurate function to compare two
 * ints.
 */
#define DEFINE_COMPARE_MIN_SCALAR(type)                           \
	static inline int compare_min_##type(void* a, void* b) {      \
		return (*(type*)a < *(type*)b) - (*(type*)a > *(type*)b); \
	}

#endif	// !COMPARE_H
