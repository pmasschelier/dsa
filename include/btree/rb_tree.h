#ifndef RBTREE_H
#define RBTREE_H

#include <stddef.h>
#include "structures.h"

#include "bsearch_tree.h"

/**
 * @file rb_tree.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
 */

/**
 * @defgroup rb_tree Red-Black binary search trees
 * @ingroup btree
 *
 * Red-Black Balanced binary search tree uses color tags on each node to keep
 * the tree balanced on each insertion or deletion.
 *
 * @{
 */

enum rb_tree_colors {
    RB_BLACK,
    RB_RED
};

/**
 * @brief Inserts an element into the tree
 *
 * Insert an element into the tree. The tree will take ownership of the element
 * and free it (if bsearch_tree#free_element != NULL) when the tree is
 * freed.
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
int rb_tree_insert(bsearch_tree_t* tree,
				   void* value,
				   node_btree_t** found);

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
BOOL rb_tree_remove(bsearch_tree_t* tree, void* value);

BOOL rb_tree_is_node_red(node_btree_t* node);

#endif	// !RBTREE_H
