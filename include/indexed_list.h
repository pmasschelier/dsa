#ifndef INDEXED_LIST_H
#define INDEXED_LIST_H

#include <stddef.h>
#include <stdint.h>

/**
 * @file indexed_list.h
 * @brief Indexed lists definition
 * Defines functions to create, free and manipulate indexed-lists
 * @ingroup indexed_list
 */

#define indexed_list_foreach_index(list, index) \
    for(int index = (list).begin; index != -1; index = (list).next[index])

#define indexed_list_foreach_value(list, value, type)                   \
    for(int index = (list).begin; index != -1; index = -1)              \
    for(type* value = get_array_ref((list), (list).begin, type);        \
        index != -1 && (value = get_array_ref((list), index, type));    \
        index = (list).next[index])

#define indexed_list_foreach_index_rev(list, index) \
    for(int index = (list).end; index != -1; index = (list).prev[index])

#define indexed_list_foreach_value_rev(list, value, type)               \
    for(int index = (list).end; index != -1; index = -1)                \
    for(type* value = get_array_ref((list), (list).end, type);          \
        index != -1 && (value = get_array_ref((list), index, type));    \
        index = (list).prev[index])

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME

/**
 * @defgroup indexed_list Indexed lists
 * 
 * indexed_list are doubly-linked list with array_like memory layout.
 * ```
 * indexed_list_t array = INDEXED_LIST_INIT(int);
 * int value;
 * while(sizeof(int) == read(fd, &value, sizeof(int))) {
 *    if(NULL == indexed_list_insert(&array, array->end, &value))
 *       goto allocation_failed;
 * }
 * // Do something with the data...
 *
 * indexed_list_deinit(&array);
 * ```
 * @note the value given to indexed_list_insert() will be copied.<br>
 * You can use an indexed_list of pointers if the cost of copy is too high.
 * @{
 */

/**
 * @brief Foreach macro for indexed_list
 * 
 * Iterates over an indexed_list, by defining the value
 * pointer pointing to the current element.
 *
 * An implicit index variable is defined.
 *
 * **Example:** printing an array_list of int
 * ```
 * void print_array_int(const indexed_list list) {
 *     indexed_list_foreach(list, value, int) {
 *         printf("list[%d] = %d\n", index, *value);
 *     }
 * }
 * ```
 * @warning The index value is not the position of the element in the list
 * but the element offset in the backend array
 *
 * @param list List to iterate over
 * @param index index in the backend array
 * @param value Pointer to the element
 * @param type Type of the list elements
 */
#define indexed_list_foreach(...) \
	GET_MACRO(__VA_ARGS__, indexed_list_foreach_value, indexed_list_foreach_index, )(__VA_ARGS__)

/**
 * @brief Reverse Foreach macro for indexed_list
 * 
 * Iterates over an indexed_list in reverse order, by defining the value
 * pointer pointing to the current element.
 *
 * An implicit index variable is defined.
 *
 * @param list List to iterate over
 * @param index index in the backend array
 * @param value Pointer to the element
 * @param type Type of the list elements
 */
#define indexed_list_foreach_rev(...)                                             \
	GET_MACRO(__VA_ARGS__, indexed_list_foreach_value_rev, indexed_list_foreach_index_rev, ) \
	(__VA_ARGS__)

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
#define get_array_ref(array, index, type) \
	(type*)((array).data + (index) * (array).size_bytes)

/**
 * @typedef indexed_list_t
 * @brief Typedef for the indexed_list structure
 *
 */
typedef struct indexed_list indexed_list_t;

/**
 * @struct indexed_list
 * @brief Indexed list
 * @see indexed_list
 */
struct indexed_list {
	size_t size_bytes;
	/**< Size in bytes of an element referenced by the list */
	int size;
	/**< Element count */
	int capacity;
	/**< Allocated items count */
    int begin;
	/**< First item */
    int end;
	/**< Last item */
	uint8_t* data;
	/**< Pointer to data */
	int* prev;
	/**< Array of predecessors */
	int* next;
	/**< Array of successors */
};

/**
 * @brief indexed_list Compound literal
 * 
 * This is the most concise way to initialize an indexed_list.
 *
 * ```
 * indexed_list_t list = INDEXED_LIST_INIT(int);
 * // Do something...
 * indexed_list_deinit(&list);
 * ```
 * @see indexed_list_init
 */
#define INDEXED_LIST_INIT(type)             \
	(indexed_list_t) {                      \
		.size_bytes = sizeof(type),         \
        .size = 0,                          \
        .capacity = 0,                      \
        .begin = -1,                        \
        .end = -1,                          \
        .data = NULL,                       \
        .prev = NULL,                       \
        .next = NULL,                       \
	}

/**
 * @brief Create an empty list
 *
 * __Every list created with this function should be freed using
 * indexed_list_free()__
 *
 * _Complexity: O(1)_
 * @param[in] size Size of an element in bytes
 * @return A pointer to the newly created list
 * @see indexed_list_free()
 */
indexed_list_t* indexed_list_create(size_t size_byte);

/**
 * @brief Frees the list
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 */
void indexed_list_free(indexed_list_t* list);

/**
 * @brief Initialize an empty indexed list
 *
 * __Every indexed_list initialized with this function should be freed using
 * indexed_list_deinit()__
 *
 * This is equivalent to initializing with indexed_list_INIT() ie:
 * ```
 * indexed_list_t list;
 * indexed_list_init(&list, sizeof(int));
 *
 * // Use the array here
 *
 * indexed_list_deinit(&list);
 * ```
 * is equivalent to:
 * ```
 * indexed_list_t list = INDEXED_LIST_INIT(int);
 *
 * // Use the array here
 *
 * indexed_list_deinit(&list);
 * ```
 * This function is provided for symetry with indexed_list_deinit()
 *
 * _Complexity: O(1)_
 * @param[in] list A pointer to the uninitialized list
 * @param[in] size_bytes Size of an element
 * @see indexed_list_deinit()
 */
void indexed_list_init(indexed_list_t* list, size_t size_bytes);

/**
 * @brief Uninitialize the list.
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @see indexed_list_init()
 */
void indexed_list_deinit(indexed_list_t* array);

/**
 * @brief Create a list from an array
 *
 * call :
 * ```c
 * LIST* liste = list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
 * sizeof(char), 10);
 * ```
 * __The array content is copied__<br>
 * __Every list created with this function should be freed using indexed_list_free__
 *
 * _Complexity: O(n)_
 * @param[in] tab pointer to the array to copy
 * @param[in] size size (in bytes) of an array's element
 * @param[in] length number of elements in the array
 * @return A pointer on the newly created list
 * @see indexed_list_free()
 */
indexed_list_t* indexed_list_from_tab(void* tab, size_t size, unsigned length);

/**
 * @brief Fill an array with the content of a list
 *
 * _The array should be allocated with a sufficient size which can be determined
 * using the indexed_list_length() function._
 *
 * _Complexity O(n)_
 * @param[in] list list to copy element from
 * @param[out] tab pointer to the array to fill
 * @return tab
 * @see length_list()
 */
void* indexed_list_to_tab(indexed_list_t* list, void* tab);

/** @brief Returns the list length
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @return The length of the list
 */
unsigned indexed_list_length(indexed_list_t* list);

/**
 * @brief Insert an element into the list.
 *
 * The list will take ownership of the element and free it when the times come.
 * You can overwrite this behavior by setting list_ref::free_element to
 * NULL.<br>
 * __The data is not copied.__
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list
 * @param[in] prev index after which the insertion should occur. Passing -1
 * will cause it insert at the beginning of the list
 * @param[in] data pointer to the value the new node will reference
 * @return index of the newly created node
 */
int indexed_list_insert(indexed_list_t* list, int prev, const void* data);

/**
 * @brief Remove a node from the list
 *
 * If x is not NULL the data will be copied to *x.<br>
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] index index to remove
 * @param[out] data removed data (if NULL the data will be lost)
 * value.
 *
 */
int indexed_list_remove(indexed_list_t* list, int index, void* data);

/**
 * @brief Removes all the elements of the list
 *
 * __Frees the referenced data__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 */
void indexed_list_clean(indexed_list_t* list);

/**
 * @brief Swaps two nodes of the list
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] a First node
 * @param[in] b Second node
 */
int indexed_list_swap(indexed_list_t* list, int a, int b);

int indexed_list_move_after(indexed_list_t* list, int index, int prev);

/** @} */  // end of list_ref

#endif
