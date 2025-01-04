#ifndef AVLTREE_H
#define AVLTREE_H

#include <stddef.h>
#include "structures.h"

#include "bsearch_tree_ref.h"

/**
 * @file avl_tree_ref.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
 */

/**
 * @defgroup avl_tree_ref AVL binary search trees
 *
 * AVL Balanced binary search tree uses balance factors on each node to keep the
 * tree balanced on each insertion or deletion.
 *
 * @dotfile avl_tree_case1.dot
 *
 * @{
 */

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
int avl_tree_insert(bsearch_tree_ref_t* tree,
					void* value,
					node_bsearch_tree_ref_t** found);

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
BOOL avl_tree_remove(bsearch_tree_ref_t* tree, void* value);

/**
 * @brief Compute the height of an AVL tree
 *
 * The usual way to compute the height of a binary tree is a logarithmic
 * operation. However to equilibrate an AVL tree we keep the height of each node
 * across insertion and deletion. Therefore this function provide a very
 * efficient way (a simple read) to retrieve the height of the root of an AVL
 * tree.
 *
 * Complexity_: \f$O(1)\f$
 * @param tree Pointer to the tree
 * @return The height of the tree (0 if the tree is empty ie. has no root)
 */
unsigned avl_tree_height(bsearch_tree_ref_t* tree);

#endif	// !AVLTREE_H
