#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "structures.h"

/**
 * @file array_list.h
 * @brief Dynamic arrays definition
 * Defines functions to create, free and manipulate dynamic arrays
 * @ingroup array_list
 */

#define ARRAY_LIST_MIN_CAPACITY 8

#define get_array_list_ref(array, index, type) \
	(type*)((array)->data + (index) * (array)->size_bytes)

/**
 * @defgroup array_list Dynamic arrays
 * @{
 */

/**
 * @typedef array_list_t
 * @brief Typedef for the array_list structure
 *
 */
typedef struct array_list array_list_t;

/**
 * @struct circular_buffer
 * @brief A circular buffer
 *
 * A dynamic array olds a pointer to its data, the size (in bytes) of an
 * element of the array, the current capacity of the buffer, the actual
 * number of elements in the array
 */
struct array_list {
	char* data; /**< Pointer to the allocated memory for the dynamic array */
	unsigned size_bytes; /**< Size (in bytes) of an element of the array */
	unsigned capacity;	 /**< The current capacity of the array */
	unsigned size;		 /**< The number of elements in the array */
};

/**
 * @brief Create an empty dynamic array
 *
 * __Every array_list created with this function should be freed using
 * free_array_list()_
 *
 * _Complexity: O(1)_
 * @param[in] size Size of an element
 * @return A pointer to the newly created array_list
 * @see free_list()
 */
array_list_t* create_array_list(unsigned size_bytes);

/**
 * @brief Free the array
 *
 * _Complexity: O(n)_
 * @param[in] array pointer to the array
 */
void free_array_list(array_list_t* array);

/**
 * @brief Test if the array is empty
 *
 * What it really does : `return array->size == 0;`
 *
 * _Complexity: O(1)_
 * @param[in] array pointer on the array to test
 * @return TRUE iif the array is empty
 */
BOOL array_list_empty(array_list_t* array);

/**
 * @brief Copy an element at the beginning of the array
 *
 * The array **won't** take ownership of the element but extend the array buffer
 * and copy the element into it.
 *
 * _Complexity: O(n)_
 * @param array Pointer to the array to extend
 * @param p Pointer to the data to copy at the beginning of the array
 * @return Pointer to the newly created element (NULL in case of failure)
 */
void* array_list_push_front(array_list_t* array, void* value);

/**
 * @brief Copy an element at the end of the array
 *
 * The array **won't** take ownership of the element but extend the array buffer
 * and copy the element into it.
 *
 * _Complexity: O(1)_
 * @param array Pointer to the array to extend
 * @param p Pointer to the data to copy at the end of the array
 * @return Pointer to the newly created element (NULL in case of failure)
 */
void* array_list_push_back(array_list_t* array, void* value);

/**
 * @brief Remove the first element of the array
 *
 * If value is NULL the removed data will be lost, otherwise the data is copied
 * to the memory region pointed by value which should have been allocated with
 * array_list::size_bytes bytes.<br>
 *
 * _Complexity: O(n)_
 * @param[in] array pointer to the array
 * @param[out] value pointer to the remove data (if NULL the data will be lost)
 * @return FALSE iif the array was empty
 */
BOOL array_list_pop_front(array_list_t* array, void* value);

/**
 * @brief Remove the last element of the array
 *
 * If value is NULL the removed data will be lost, otherwise the data is copied
 * to the memory region pointed by value which should have been allocated with
 * array_list::size_bytes bytes.<br>
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @param[out] value pointer to the remove data (if NULL the data will be freed)
 * @return FALSE iif the array was empty
 */
BOOL array_list_pop_back(array_list_t* array, void* value);

/**
 * @brief Swap two elements of the array
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @param[in] a First index
 * @param[in] b Second index
 * @return -ERROR_NO_ERROR is the indices could be swapped
 */
int array_list_swap(array_list_t* array, unsigned a, unsigned b);

/** @} */

#endif	// !ARRAY_LIST_H
