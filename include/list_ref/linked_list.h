#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// TODO: Write a linked list implementation where node and node data are
// allocated altogather and data is systematically copied, eventually define a
// threshold for switching between copy and reference semantics

#include <stddef.h>
#include <stdint.h>
#include "compare.h"
#include "structures.h"

/**
 * @file linked_list.h
 * @brief Doubly linked lists definition
 * Defines functions to create, free and manipulate doubly-linked lists
 * @ingroup list_ref
 */

#define get_node_ref(node, type) ((type*)&(node)->data)

#define linked_list_foreach_node(list, node)                    \
	for (linked_list_node_t* node = (list).begin; node != NULL; \
		 node = node->next)

#define linked_list_foreach_value(list, value, type)                         \
	for (linked_list_node_t* node = (list).begin; node != NULL; node = NULL) \
		for (type* value = (type*)node->data;                                \
			 node != NULL && (value = (type*)node->data); node = node->next)

#define linked_list_foreach_node_rev(list, node) \
	for (linked_list_node_t* node = (list).end; node != NULL; node = node->prev)

#define linked_list_foreach_value_rev(list, value, type)                   \
	for (linked_list_node_t* node = (list).end; node != NULL; node = NULL) \
		for (type* value = (type*)node->data;                              \
			 node != NULL && (value = (type*)node->data); node = node->prev)

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME

/**
 * @defgroup list_ref Doubly-linked lists
 *
 * linked_list are doubly linked lists.
 *
 * @dot
 * digraph after {
 *      rankdir = "LR"
 *      bgcolor = "transparent"
 *      node [style=filled]
 *      edge [color=grey50]
 *      a, e [fillcolor=lightblue]
 *      a -> b -> c -> d -> e
 *      e -> d -> c -> b -> a [style=dashed,constraint=false]
 * }
 * @enddot
 *
 * ```
 * linked_list_t list = LINKED_LIST_INIT(int);
 * int value;
 * while(sizeof(int) == read(fd, &value, sizeof(int))) {
 *    if(NULL == linked_list_push_back(&list, &value))
 *       goto allocation_failed;
 * }
 * // Do something with the data...
 *
 * linked_list_deinit(&array);
 * ```
 * @note the value given to linked_list_push_back() will be copied.<br>
 * You can use a linked_list of pointers if the cost of copy is too high.
 * @{
 */

/**
 * @brief Foreach macro for linked_list
 *
 * Iterates over an linked_list, by defining the value
 * pointer pointing to the current element.
 *
 * An implicit node variable is defined.
 *
 * **Example:** printing a linked_list of int
 * ```
 * void print_list_int(const linked_list list) {
 *     linked_list_foreach(list, value, int) {
 *         printf("%d\n", *value);
 *     }
 * }
 * ```
 *
 * @param list List to iterate over
 * @param node Current node
 * @param value Pointer to the element
 * @param type Type of the array elements
 */
#define linked_list_foreach(...)                      \
	GET_MACRO(__VA_ARGS__, linked_list_foreach_value, \
			  linked_list_foreach_node, )(__VA_ARGS__)

/**
 * @brief Reverse Foreach macro for linked_list
 *
 * Iterates over a linked_list in reverse order, by defining the value
 * pointer pointing to the curmultiplerent element.
 *
 * An implicit node variable is defined.
 *
 * @param array Array to iterate over
 * @param node Current node
 * @param value Pointer to the element
 * @param type Type of the array elements
 */
#define linked_list_foreach_rev(...)                      \
	GET_MACRO(__VA_ARGS__, linked_list_foreach_value_rev, \
			  linked_list_foreach_node_rev)               \
	(__VA_ARGS__)

/**
 * @typedef node_list_ref_t
 * @brief Typedef for the node_list_ref structure
 *
 */
typedef struct linked_list_node linked_list_node_t;

/**
 * @struct node_list_ref
 * @brief A doubly-linked list node.
 *
 * A list node holds a pointer to its data and a pointer to its predecessor
 * (NULL if it is the first node) and a pointer to its successor (NULL if it is
 * the last node)
 */
struct linked_list_node {
	linked_list_node_t* prev; /**< Pointer to its predecessor */
	linked_list_node_t* next; /**< Pointer to its successor */
	uint8_t data[];			  /**< Node data */
};

/**
 * @typedef list_ref_t
 * @brief Typedef for the list_ref structure
 *
 */
typedef struct linked_list linked_list_t;

/**
 * @struct list_ref
 * @brief Doubly linked list
 *
 * A linked lists olds pointers to its first and last node and the size (in
 * bytes) of an element of the list.
 *
 * The prefered way to iterate over a linked_list is using linked_list_foreach():
 * ```
 * int i;
 * linked_list_foreach(&array, value, int) {
 *    printf("%d\n", *value);
 *    // ...
 * }
 * ```
 * @see list_node_ref
 */
struct linked_list {
	size_t size_bytes;
	/**< Size in bytes of an element referenced by the list */
	linked_list_node_t* begin;
	/**< Pointer to the first element of the list (NULL if list is empty) */
	linked_list_node_t* end;
	/**< Pointer to the last element of the list */
};

/**
 * @brief array_list Compound literal
 *
 * This is the most concise way to initialize a linked_list.
 *
 * ```
 * linked_list_t list = LINKED_LIST_INIT(int);
 * // Do something...
 * linked_list_deinit(&list);
 * ```
 * @see linked_list_init
 */
#define LINKED_LIST_INIT(type)                                 \
	(linked_list_t) {                                          \
		.size_bytes = sizeof(type), .begin = NULL, .end = NULL \
	}

/**
 * @brief Create an empty list
 *
 * __Every linked_list created with this function should be freed using
 * linked_list_free()__
 *
 * ```
 * linked_list_t* list = linked_list_create(sizeof(int));
 * if(list == NULL)
 *     goto error;
 *
 * // Use the array here
 *
 * linked_list_free(list);
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] size_bytes Size of an element (the size of the element pointed by
 * @ref node_list_ref::p )
 * @return A pointer to the newly created list
 * @see linked_list_free()
 */
linked_list_t* linked_list_create(size_t size_bytes);

/**
 * @brief Frees a list created with linked_list_create()
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 * @see linked_list_create()
 */
void linked_list_free(linked_list_t* list);

/**
 * @brief Initialize an empty linked  list
 *
 * __Every linked_list initialized with this function should be freed using
 * linked_list_deinit()__
 *
 * This is equivalent to initializing with LINKED_LIST_INIT() ie:
 * ```
 * linked_list_t list;
 * linked_list_init(&list, sizeof(int));
 *
 * // Use the array here
 *
 * linked_list_deinit(&list);
 * ```
 * is equivalent to:
 * ```
 * linked_list_t list = LINKED_LIST_INIT(int);
 *
 * // Use the array here
 *
 * linked_list_deinit(&list);
 * ```
 * This function is provided for symetry with linked_list_deinit()
 *
 * _Complexity: O(1)_
 * @param[in] array A pointer to the uninitialized array
 * @param[in] size_bytes Size of an element
 * @see linked_list_deinit()
 */
void linked_list_init(linked_list_t* array, size_t size_bytes);

/**
 * @brief Uninitialize the list.
 *
 * _Complexity: O(1)_
 * @param[in] array pointer to the array
 * @see linked_list_init()
 */
void linked_list_deinit(linked_list_t* array);

/**
 * @brief Test if the list is empty
 *
 * What it really does : `return list->begin == NULL;`
 *
 * _Complexity: O(1)_
 * @param[in] list pointer on the list to test
 * @return TRUE iif the list is empty
 */
BOOL linked_list_empty(const linked_list_t* list);

/**
 * @brief Create a linked_list from an array
 *
 * Exemple:
 * ```c
 * linked_list_t* list = linked_list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
 * sizeof(char), 10);
 * if(list == NULL)
 *    goto allocation_failed;
 *
 * // Do someting
 * linked_list_free(list);
 * ```
 * __The array content is copied__<br>
 * __Every list created with this function should be freed using
 * linked_list_free__
 *
 * _Complexity: O(n)_
 * @param[in] tab pointer to the array to copy
 * @param[in] size_bytes size (in bytes) of an array's element
 * @param[in] length number of elements in the array
 * @return A pointer on the newly created list
 * @see linked_list_free()
 */
linked_list_t* linked_list_from_tab(void* tab,
									size_t size_bytes,
									unsigned length);

/**
 * @brief Fill an array with the content of a list
 *
 * _The array should be allocated with a sufficient size which can be determined
 * using the linked_list_length() function._
 *
 * _Complexity O(n)_
 * @param[in] list list to copy element from
 * @param[out] tab pointer to the array to fill
 * @return tab
 * @see linked_list_length()
 */
void* linked_list_to_tab(linked_list_t* list, void* tab);

/** @brief Compute a list length
 *
 * Iterates over the list from begin to end to compute its length.
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 * @return The length of the list
 */
unsigned linked_list_length(linked_list_t* list);

/**
 * @brief Insert an existing node into the list
 *
 * The list will take ownership on the node and it doesn't need to be freed
 * anymore.<br>
 * __A node should only be owned by one list otherwise cleaning the lists
 * will cause a segfault because of double free.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] prev node after which the insertion should occur. Pass NULL
 * to insert at the beginning of the list
 * @param[in] node pointer to the node to insert
 * @see linked_list_extract
 */
void linked_list_insert_node(linked_list_t* list,
							 linked_list_node_t* prev,
							 linked_list_node_t* node);

/**
 * @brief Insert an element into the list.
 *
 * The data will be copied.
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list
 * @param[in] prev node after which the insertion should occur. Pass NULL
 * to insert at the beginning of the list
 * @param[in] p pointer to the value to be copied into the list
 * @return pointer to the newly created node
 */
linked_list_node_t* linked_list_insert(linked_list_t* list,
									   linked_list_node_t* prev,
									   const void* p);

/**
 * @brief Append an element to the beginning of the list
 *
 * The data will be copied
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list to extend
 * @param[in] x pointer to data we want to add to the list
 * @return pointer to the newly created node (NULL in case of failure)
 */
linked_list_node_t* linked_list_push_front(linked_list_t* list, const void* x);

/**
 * @brief Append an element to the end of the list
 *
 * The data will be copied
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list to extend
 * @param[in] x pointer to the data we want to add to the list
 * @return pointer to the newly created node (NULL in case of failure)
 */
linked_list_node_t* linked_list_push_back(linked_list_t* list, const void* x);

/**
 * @brief Remove the first element of the list
 *
 * If x is not NULL the node data will be copied to *x.<br>
 * __Frees the removed node.__
 *
 * Example: Level-order traversal of a binary tree
 * ```
 * linked_list_t forest = LINKED_LIST_INIT(node_btree_ref_t*);
 * linked_list_node_t* ret = linked_list_push_back(&forest, &tree->root);
 * when_null_ret(ret, -ERROR_ALLOCATION_FAILED);
 *
 * node_btree_ref_t* t;
 * while (linked_list_pop_front(&forest, &t)) {
 *     if (t->ls != NULL)
 *         linked_list_push_back(&forest, &t->ls);
 *     if (t->rs != NULL)
 *         linked_list_push_back(&forest, &t->rs);
 *     // ...
 *     // Do something with t
 *     // ...
 * }
 * linked_list_deinit(&forest);
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] x removed data (if NULL the data will be lost)
 * @return TRUE iif the list wasn't empty
 * @see linked_list_pop_back
 */
BOOL linked_list_pop_front(linked_list_t* list, void* x);

/**
 * @brief Remove the last element of the list
 *
 * If x is not NULL the node data will be copied to *x.
 *
 * __Frees the removed node.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] x pointer on a pointer the removed data (if NULL the data will be
 * freed)
 * @see linked_list_pop_front
 */
BOOL linked_list_pop_back(linked_list_t* list, void* x);

/**
 * @brief Extract a node from the list
 *
 * This removes a node from the list without freeing anything.
 * It can be useful to transfer a node from one list to another for instance.
 *
 * **Example:** Insertion sort
 * ```
 * linked_list_node_t* next;
 * for(linked_list_node_t* node = list.begin; node != NULL; node = next) {
 *     next = node->next;
 *     int value = *get_node_ref(node, int);
 *     // Find the first predecessor of node with a smaller value
 *     linked_list_node_t* prev;
 *     for(prev = node; prev != NULL; prev = prev->prev) {
 *         int x = *get_node_ref(prev, int);
 *         if(x < value)
 *             break;
 *     }
 *     // Extract node from the list
 *     linked_list_extract(&list, node);
 *     // Insert node after prev
 *     linked_list_insert_node(&list, prev, node);
 * }
 * ```
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] node pointer to the extracted element
 * @see linked_list_insert_node
 * @see linked_list_remove
 */
void linked_list_extract(linked_list_t* list, linked_list_node_t* node);

/**
 * @brief Remove a node from the list
 *
 * If x is not NULL the data will be copied to *x.<br>
 * __Frees the removed node.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] node node to remove
 * @param[out] x removed data (if NULL the data will be lost)
 * @see linked_list_extract
 * @see linked_list_pop_front
 * @see linked_list_pop_back
 */
void linked_list_remove(linked_list_t* list, linked_list_node_t* node, void* x);

/**
 * @brief Removes all the elements of the list
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 */
void linked_list_clean(linked_list_t* list);

/**
 * @brief Finds an element of the list
 *
 * Find the node whose data is equals to *value
 * according to the predicate equals.
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 * @param[in] value value to find
 * @param[in] equals predicate
 * @see equals_fn_t
 */
linked_list_node_t* linked_list_find_equals(linked_list_t* list,
											void* value,
											equals_fn_t equals);

/**
 * @brief Swaps two nodes of the list
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] a First node
 * @param[in] b Second node
 */
void linked_list_swap(linked_list_t* list,
					  linked_list_node_t** a,
					  linked_list_node_t** b);

/** @} */  // end of list_ref

#endif
