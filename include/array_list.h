#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>
#include "structures.h"

/**
 * @file array_list.h
 * @brief Dynamic arrays definition
 * Defines functions to create, free and manipulate dynamic arrays
 * @ingroup array_list
 */

/**
 * @defgroup array_list Dynamic arrays
 * 
 * array_list are basic dynamic array with size and capacity.
 * ```
 * array_list_t array = ARRAY_LIST_INIT(int);
 * int value;
 * while(sizeof(int) == read(fd, &value, sizeof(int))) {
 *    if(NULL == array_list_push_back(&array, &value))
 *       goto allocation_failed;
 * }
 * // Do something with the data...
 *
 * array_list_deinit(&array);
 * ```
 * @note the value given to array_list_push_back() will be copied.<br>
 * You can use an array_list of pointers if the cost of copy is too high.
 * @{
 */

/**
 * @typedef array_list_t
 * @brief Typedef for the array_list structure
 *
 */
typedef struct array_list array_list_t;

/**
 * @struct array_list
 * @brief A dynamic array
 *
 * A dynamic array olds a pointer to its data, the size (in bytes) of an
 * element of the array, the current capacity of the buffer, the actual
 * number of elements in the array
 * 
 * The prefered way to iterate over an array_list is using get_array_list_ref():
 * ```
 * int* value;
 * int i;
 * for(int i = 0; i < array.size; i++) {
 *    value = get_array_list_ref(array, i, int);
 *    printf("%d\n", *value);
 *    // ...
 * }
 * ```
 */
struct array_list {
	char* data; /**< Pointer to the allocated memory for the dynamic array */
	unsigned size_bytes; /**< Size (in bytes) of an element of the array */
	unsigned capacity;	 /**< The current capacity of the array */
	unsigned size;		 /**< The number of elements in the array */
};

/**
 * @brief Minimal capacity of a non-empty array
 * 
 * This is typically the capacity that will be allocated after a first element
 * was pushed into the array.
 *
 * This value defaults to 8 and can be modified by the user.
 */
extern size_t array_list_min_capacity;

/**
 * @brief array_list Compound literal
 * 
 * This is the most concise way to initialize an array_list.
 *
 * ```
 * array_list_t array = ARRAY_LIST_INIT(int);
 * // Do something...
 * array_list_deinit(&array);
 * ```
 * @see array_list_init
 */
#define ARRAY_LIST_INIT(type)                                               \
	(array_list_t) {                                                        \
		.data = NULL, .size_bytes = sizeof(type), .capacity = 0, .size = 0, \
	}

/**
 * @brief Helper macro for random access
 * 
 * @warning The access are not checked. Be careful with out-of-bounds accesses.
 *
 * @param array The array to index
 * @param index The index of the target element
 * @param type The array type
 * @return A pointer to the i-th element
 */
#define get_array_list_ref(array, index, type) \
	(type*)((array).data + (index) * (array).size_bytes)


/**
 * @brief Create an empty dynamic array
 *
 * __Every array_list created with this function should be freed using
 * array_list_free()__
 *
 * ```
 * array_list_t* array = array_list_create(sizeof(int));
 * if(array == NULL)
 *     goto error;
 * 
 * // Use the array here
 *
 * array_list_free(array);
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] size_bytes Size of an element
 * @return A pointer to the newly created array_list or NULL if the allocation failed
 * @see array_list_free()
 */
array_list_t* array_list_create(unsigned size_bytes);

/**
 * @brief Free an array created with array_list_create()
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @see array_list_create()
 */
void array_list_free(array_list_t* array);

/**
 * @brief Initialize an empty dynamic array
 *
 * __Every array_list initialized with this function should be freed using
 * array_list_deinit()__
 *
 * This is equivalent to initializing with ARRAY_LIST_INIT() ie:
 * ```
 * array_list_t array;
 * array_list_init(&array, sizeof(int));
 *
 * // Use the array here
 *
 * array_list_deinit(&array);
 * ```
 * is equivalent to:
 * ```
 * array_list_t array = ARRAY_LIST_INIT(int);
 *
 * // Use the array here
 *
 * array_list_deinit(&array);
 * ```
 * This function is provided for symetry with array_list_deinit()
 *
 * _Complexity: O(1)_
 * @param[in] array A pointer to the uninitialized array
 * @param[in] size_bytes Size of an element
 * @see array_list_deinit()
 */
void array_list_init(array_list_t* array, unsigned size_bytes);

/**
 * @brief Uninitialize the array
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @see array_list_init()
 */
void array_list_deinit(array_list_t* array);

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
 * @param value Pointer to the data to copy at the beginning of the array
 * @return Pointer to the newly created element (NULL in case of failure)
 * @see array_list_push_back()
 */
void* array_list_push_front(array_list_t* array, const void* value);

/**
 * @brief Copy an element at the end of the array
 *
 * The array **won't** take ownership of the element but extend the array buffer
 * and copy the element into it. If you allocated the memory region pointed
 * by value you are responsible for freeing it
 *
 * ```
 * array_list_t array = ARRAY_LIST_INIT(unsigned);
 * 
 * // Appends the numbers 0 to 9 to the array
 * for(unsigned i = 0; i < 10; i++)
 *  array_list_push_back(&array, &i);
 *
 * array_list_deinit(&array);
 * ```
 *
 * If the objects you are manipulating are too large or have varying size you
 * can store pointers in the array, but remember that you are responsible for
 * freeing the memory.
 *
 * _Complexity: O(1)_
 * @param array Pointer to the array to extend
 * @param value Pointer to the data to copy at the end of the array
 * @return Pointer to the newly created element (NULL in case of failure)
 */
void* array_list_push_back(array_list_t* array, const void* value);

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
 * @see array_list_pop_back()
 */
BOOL array_list_pop_front(array_list_t* array, void* value);

/**
 * @brief Remove the last element of the array
 *
 * If value is NULL the removed data will be lost, otherwise the data is copied
 * to the memory region pointed by value which should have been allocated with
 * array_list::size_bytes bytes.<br>
 * 
 * ```
 * int compute_sum(array_list_t* array) {
 *    int value, sum = 0;
 *    while(array_list_pop_back(array, &value))
 *       sum += value;
 *    return sum;
 * }
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @param[out] value pointer to the removed data (if NULL the data will be lost)
 * @return FALSE iif the array was empty
 */
BOOL array_list_pop_back(array_list_t* array, void* value);

/**
 * @brief Swap two elements of the array
 *
 * Example: Selection sort
 * ```
 * for (unsigned i = 0; i < ARRAY_LEN; i++) {
 *     int min = *get_array_list_ref(array, i, int);
 *     unsigned j_min = i;
 *     for (unsigned j = i + 1; j < ARRAY_LEN; j++) {
 *         int x = *get_array_list_ref(array, j, int);
 *         if(x < min) {
 *             min = x;
 *             j_min = j;
 *         }
 *     }
 *     array_list_swap(&array, i, j_min);
 * }
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @param[in] a First index
 * @param[in] b Second index
 * @return -ERROR_NO_ERROR is the indices could be swapped
 */
int array_list_swap(array_list_t* array, unsigned a, unsigned b);

/**
 * @brief Swap the i-th element with the last one and pop back
 *
 * This function is very useful to remove an arbitrary element
 * of the array in constant time when you don't care about the order.
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @param[in] i Element index to remove
 * @param[out] value pointer to the remove data (if NULL the data will be lost)
 * @return TRUE iif the indices could be swapped
 */
int array_list_swap_and_pop_back(array_list_t* array,
								  unsigned int i,
								  void* value);

/** @} */

#endif	// !ARRAY_LIST_H
