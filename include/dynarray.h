#ifndef DYNARRAY_H
#define DYNARRAY_H

#include "structures.h"

/**
 * @file dynarray.h
 * @brief Dynamic arrays definition
 * Defines functions to create, free and manipulate dynamic arrays
 * @ingroup dynarray
 */

#define DYNARRAY_MIN_CAPACITY 8

#define get_dynarray_ref(array, index, type) \
	(type*)((array)->data + (index) * (array)->size_bytes)

/**
 * @defgroup dynarray Dynamic arrays
 * @{
 */

/**
 * @typedef dynarray_t
 * @brief Typedef for the dynarray structure
 *
 */
typedef struct dynarray dynarray_t;

/**
 * @struct circular_buffer
 * @brief A circular buffer
 *
 * A dynamic array olds a pointer to its data, the size (in bytes) of an
 * element of the array, the current capacity of the buffer, the actual
 * number of elements in the array
 */
struct dynarray {
	char* data; /**< Pointer to the allocated memory for the dynamic array */
	unsigned size_bytes; /**< Size (in bytes) of an element of the array */
	unsigned capacity;	 /**< The current capacity of the array */
	unsigned size;		 /**< The number of elements in the array */
};

dynarray_t* create_dynarray(unsigned size_bytes);

void free_dynarray(dynarray_t* array);

BOOL dynarray_empty(dynarray_t* array);

void* dynarray_push_front(dynarray_t* array, void* value);

void* dynarray_push_back(dynarray_t* array, void* value);

BOOL dynarray_pop_front(dynarray_t* array, void* value);

BOOL dynarray_pop_back(dynarray_t* array, void* value);

#endif	// !DYNARRAY_H
