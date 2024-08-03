#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>
#include "compare.h"
#include "ptr.h"
#include "structures.h"

#include "btree_ref.h"

#define free_avl_tree_ref(tree) btree_free((btree_ref_t*)(tree))

/**
 * @file avl_tree_ref.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
 */

/**
 * @defgroup avl_tree_ref AVL binary search trees
 * @{
 */

/**
 * @typedef node_avl_tree_ref_t
 * @brief Typedef for the node_avl_tree_ref structure
 *
 */
typedef struct node_avl_tree_ref node_avl_tree_ref_t;

/**
 * @struct node_avl_tree_ref
 * @brief A binary-tree node.
 *
 * A tree node holds a pointer to its data and a pointer to its left son (ls)
 * (NULL if there is no left son) and a pointer to its right son (NULL if it is
 * there is no right son).
 */
struct node_avl_tree_ref {
	void* p;					 /**< Pointer to data */
	node_avl_tree_ref_t* ls;	 /**< Pointer to its left son */
	node_avl_tree_ref_t* rs;	 /**< Pointer to its right son */
	node_avl_tree_ref_t* father; /**< Pointer to its father */
	unsigned subtree; /**< Heigh of the subtree whom this node is the root */
};

/**
 * @typedef avl_tree_ref_t
 * @brief Typedef for the avl_tree_ref structure
 *
 */
typedef struct avl_tree_ref avl_tree_ref_t;

/**
 * @struct avl_tree_ref
 * @brief Binary tree
 *
 * A binary tree struct contains the size of the elements referenced by the
 * nodes and a pointer to a function to free them.
 * @see node_avl_tree_ref
 */
struct avl_tree_ref {
	/**
	 * @brief Root of the binary tree
	 *
	 * If root == NULL the binary tree is empty
	 */
	node_avl_tree_ref_t* root;
	/**
	 * @brief Size (in bytes) on a element
	 *
	 * This field should be equals to the size (in bytes) of the memory region
	 * referenced by node_avl_tree_ref_t#p
	 *
	 * \note This field is not used by any function and is mainly here for
	 * information.
	 */
	size_t size;
	/**
	 * @brief Function used to free the elements
	 *
	 * This field should be a pointer to a function that will be called to free
	 * the memory region referenced by node_avl_tree_ref_t#p. If the btree was
	 * created using the API it will default to free from libc. However you can
	 * turn a btree in a simple view on the data by setting this pointer to
	 * NULL.
	 */
	free_element_fn_t free_element;
	/**
	 * @brief Function used to compare to elements in the tree
	 *
	 * This field should be a pointer to a function that will be called to
	 * compare to elements of the tree, it should take two pointers as
	 * arguments.
	 * @see compare_fn_t
	 */
	compare_fn_t compare;
};

/**
 * @brief Create an empty avl binary tree.
 *
 * @param[in] size Size of an element (the size of the element pointed by
 * node_avl_tree_ref#p )
 * @return A pointer to the newly created avl tree
 * @see avl_tree_free()
 */
avl_tree_ref_t* create_avl_tree(size_t size_bytes, compare_fn_t compare);

/**
 * @brief Inserts an element into the tree
 *
 * Insert an element into the tree. The tree will take ownership of the element
 * and free it (if avl_tree_ref#free_element != NULL) when the tree is freed.
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @param value Pointer to the element to compare
 * @param found If the element was already in the tree and found != NULL then
 * after the call *found points to the node that contains the element equal to
 * value
 * @return ERROR_KEY_ALREADY_EXISTS if value was found in the tree and
 * ERROR_NO_ERROR otherwise
 */
int avl_tree_insert(avl_tree_ref_t* tree,
					void* value,
					node_avl_tree_ref_t** found);

/**
 * @brief Finds if the element is present in the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @param value Pointer to the element to find
 * @return If value was found, returns the pointer to the node containing value,
 * otherwise returns NULL
 */
node_avl_tree_ref_t* avl_tree_find(avl_tree_ref_t* tree, void* value);

/**
 * @brief Removes an element from the tree
 *
 * This function will find an element equal to value in the tree, an if one such
 * element is found, it will be freed.
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @param value Pointer to the element to find and remove
 * @return TRUE if an element equal to value was found in the tree, FALSE
 * otherwise
 */
BOOL avl_tree_remove(avl_tree_ref_t* tree, void* value);

/**
 * @brief Finds the minimum element of the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @return Returns NULL if the tree is empty and the node containing the minimum
 * element otherwise
 */
node_avl_tree_ref_t* avl_tree_min(avl_tree_ref_t* tree);

/**
 * @brief Finds the maximum element of the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @return Returns NULL if the tree is empty and the node containing the maximum
 * element otherwise
 */
node_avl_tree_ref_t* avl_tree_max(avl_tree_ref_t* tree);

#endif	// !AVLTREE_H
