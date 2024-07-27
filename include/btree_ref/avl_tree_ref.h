#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>
#include "btree_ref.h"
#include "compare.h"
#include "ptr.h"
#include "structures.h"

#define free_avl_tree_ref(tree) btree_free((btree_ref_t*)(tree))

/**
 * @file avl_tree_ref.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
 */

/**
 * @defgroup avl_tree_ref Binary trees
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
	compare_fn_t compare;
};

avl_tree_ref_t* create_avl_tree(size_t size_bytes, compare_fn_t compare);

int avl_tree_insert(avl_tree_ref_t* tree,
					void* value,
					node_avl_tree_ref_t** found);

node_avl_tree_ref_t* avl_tree_find(avl_tree_ref_t* tree, void* value);

BOOL avl_tree_remove(avl_tree_ref_t* tree, void* value);

node_avl_tree_ref_t* avl_tree_min(avl_tree_ref_t* tree);

node_avl_tree_ref_t* avl_tree_max(avl_tree_ref_t* tree);

#endif	// !AVLTREE_H
