#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "ptr.h"
#include "structures.h"

/**
 * @file list_ref.h
 * @brief Doubly linked lists definition
 * Defines functions to create, free and manipulate doubly-linked lists
 * @ingroup list_ref
 */

#define get_node_ref(node, type) ((type*)(node)->p)

#define foreach_node_node(list, node) \
	for (node_list_ref_t* node = list->begin; node != NULL; node = node->next)

#define foreach_node_value(list, value, type)                    \
	type* value = (list->begin != NULL) ? list->begin->p : NULL; \
	for (node_list_ref_t* node = list->begin; node != NULL;      \
		 node = node->next, value = (node != NULL) ? node->p : NULL)

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define foreach_node(...) \
	GET_MACRO(__VA_ARGS__, foreach_node_value, foreach_node_node)(__VA_ARGS__)

/**
 * @defgroup list_ref Doubly-linked lists
 * @{
 */

/**
 * @typedef node_list_ref_t
 * @brief Typedef for the node_list_ref structure
 *
 */
typedef struct node_list_ref node_list_ref_t;

/**
 * @struct node_list_ref
 * @brief A doubly-linked list node.
 *
 * A list node holds a pointer to its data and a pointer to its predecessor
 * (NULL if it is the first node) and a pointer to its successor (NULL if it is
 * the last node)
 */
struct node_list_ref {
	void* p;			   /**< Pointer to data */
	node_list_ref_t* prev; /**< Pointer to its predecessor */
	node_list_ref_t* next; /**< Pointer to its successor */
};

/**
 * @typedef list_ref_t
 * @brief Typedef for the list_ref structure
 *
 */
typedef struct list_ref list_ref_t;

/**
 * @struct list_ref
 * @brief Doubly linked list
 * @see list_node_ref
 */
struct list_ref {
	size_t size;
	/**< Size in bytes of an element referenced by the list */
	free_element_fn_t free_element;
	/**< Function called on a list member to free its memory */
	node_list_ref_t* begin;
	/**< Pointer to the first element of the list (NULL if list is empty) */
	node_list_ref_t* end;
	/**< Pointer to the last element of the list */
};

/**
 * @brief Create an empty list
 *
 * __Every list created with this function should be freed using free_list__
 *
 * _Complexity: O(1)_
 * @param[in] size Size of an element (the size of the element pointed by
 * @ref node_list_ref::p )
 * @return A pointer to the newly created list
 * @see free_list()
 */
list_ref_t* create_list(size_t size);

/**
 * @brief Test if the list is empty
 *
 * What it really does : `return list->begin == NULL;`
 *
 * _Complexity: O(1)_
 * @param[in] list pointer on the list to test
 * @return TRUE iif the list is empty
 */
BOOL empty_list(list_ref_t* list);

/**
 * @brief Create a list from an array
 *
 * call :
 * ```c
 * LIST* liste = list_from_tab((char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
 * sizeof(char), 10);
 * ```
 * __The array content is copied__<br>
 * __Every list created with this function should be freed using free_list__
 *
 * _Complexity: O(n)_
 * @param[in] tab pointer to the array to copy
 * @param[in] size size (in bytes) of an array's element
 * @param[in] length number of elements in the array
 * @return A pointer on the newly created list
 * @see free_list()
 */
list_ref_t* list_from_tab(void* tab, size_t size, unsigned length);

/**
 * @brief Fill an array with the content of a list
 *
 * _The array should be allocated with a sufficient size which can be determined
 * using the length_list function._
 *
 * _Complexity O(n)_
 * @param[in] list list to copy element from
 * @param[out] tab pointer to the array to fill
 * @return tab
 * @see length_list()
 */
void* tab_from_list(list_ref_t* list, void* tab);

/** @brief Compute a list length
 *
 * Iterates over the list from begin to end to compute its length.
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 * @return The length of the list
 */
unsigned length_list(list_ref_t* list);

/**
 * @brief Insert an existing node into the list
 *
 * The list will take ownership on the node and it doesn't need to be freed
 * anymore.<br>
 * __A node should only be owned by one list otherwise it will segfault because
 * of double free.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] prev node after which the insertion should occur. Passing NULL
 * will cause it insert at the beginning of the list
 * @param[in] node pointer to the node to insert
 */
void insert_list_node(list_ref_t* list,
					  node_list_ref_t* prev,
					  node_list_ref_t* node);

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
 * @param[in] prev node after which the insertion should occur. Passing NULL
 * will cause it insert at the beginning of the list
 * @param[in] p pointer to the value the new node will reference
 * @return pointer to the newly created node
 */
node_list_ref_t* insert_list(list_ref_t* list, node_list_ref_t* prev, void* p);

/**
 * @brief Append an element to the beginning of the list
 *
 * The list will take ownership of the element and free it when the times come.
 * You can overwrite this behavior by setting list_ref::free_element to
 * NULL.<br>
 * __The data is not copied.__
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list to extend
 * @param[in] x pointer to data we want to add to the list
 * @return pointer to the newly created node (NULL in case of failure)
 */
node_list_ref_t* push_front_list(list_ref_t* list, void* x);

/**
 * @brief Copy an element to the beginning of the list
 *
 * The list **won't** take ownership of the element but allocate a new memory
 * region and copy the element.
 * @warning If you use this function your list_ref#free_element shouldn't be
 * NULL, otherwise the allocated region will never be freed.
 * Under the hood this function allocate a new memory region, copy the data into
 * it and call push_front_list() with the pointer to the memory region.
 *
 * _Complexity: O(1)_
 * @param list Pointer to the list to extend
 * @param p Pointer to the data to copy at the beginning of the list
 * @return Pointer to the newly created node (NULL in case of failure)
 */
node_list_ref_t* append_front_list(list_ref_t* list, void* p);

/**
 * @brief Append an element to the end of the list
 *
 * The list will take ownership of the element and free it when the times come.
 * You can overwrite this behavior by setting list_ref::free_element to
 * NULL.<br>
 * __The data is not copied.__
 *
 * _Complexity O(1)_
 * @param[in] list pointer to the list to extend
 * @param[in] x pointer to the data we want to add to the list
 * @return pointer to the newly created node (NULL in case of failure)
 */
node_list_ref_t* push_back_list(list_ref_t* list, void* x);

/**
 * @brief Copy an element to the end of the list
 *
 * The list **won't** take ownership of the element but allocate a new memory
 * region and copy the element.
 * @warning If you use this function your list_ref#free_element shouldn't be
 * NULL, otherwise the allocated region will never be freed.
 * Under the hood this function allocate a new memory region, copy the data into
 * it and call push_back_list() with the pointer to the memory region.
 *
 * _Complexity: O(1)_
 * @param list Pointer to the list to extend
 * @param p Pointer to the data to copy at the beginning of the list
 * @return Pointer to the newly created node (NULL in case of failure)
 */
node_list_ref_t* append_back_list(list_ref_t* list, void* p);

/**
 * @brief Remove the first element of the list
 *
 * If x is NULL the data referenced by the first node will be freed using
 * list_ref::free_element otherwise *x will point to the data and will need to
 * be freed later.<br>
 * __Frees the removed node.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] x pointer on a pointer to the remove data (if NULL the data will
 * be freed)
 */
void pop_front_list(list_ref_t* list, void** x);

/**
 * @brief Remove the last element of the list
 *
 * If x is NULL the data referenced by the first node will be freed using
 * list_ref::free_element otherwise *x will point to the data and will need to
 * be freed later.<br>
 * __Frees the removed node.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] x pointer on a pointer the removed data (if NULL the data will be
 * freed)
 */
void pop_back_list(list_ref_t* list, void** x);

/**
 * @brief Extract a node from the list
 *
 * This removes a node from the list without freeing anything.
 * It can be useful to transfer a node from one list to another for instance.
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[out] node pointer to the extracted element
 */
void extract_list(list_ref_t* list, node_list_ref_t* node);

/**
 * @brief Remove a node from the list
 *
 * If x is NULL the data referenced by the first node will be freed using
 * list_ref::free_element otherwise *x will point to the data and will need to
 * be freed later.<br>
 * __Frees the removed node.__
 *
 * _Complexity: O(1)_
 * @param[in] list pointer to the list
 * @param[in] node node to remove
 * @param[out] x reference to a pointer which will contain the removed node
 * value.
 *
 */
void remove_list(list_ref_t* list, node_list_ref_t* node, void** x);

/**
 * @brief Removes all the element of the list
 *
 * __Frees the referenced data__
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 */
void clean_list(list_ref_t* list);

/**
 * @brief Frees the list
 *
 * _Complexity: O(n)_
 * @param[in] list pointer to the list
 */
void free_list(list_ref_t* list);

/** @} */  // end of list_ref

#endif
