#ifndef BSEARCHTREE_H
#define BSEARCHTREE_H

#include <stddef.h>
#include "compare.h"
#include "structures.h"

#include "btree.h"

#define free_bsearch_tree(tree) btree_free((btree_t*)(tree))
#define clean_bsearch_tree(tree) btree_clean((btree_t*)(tree))
#define bsearch_tree_height(tree) btree_height((btree_t*)(tree))
#define bsearch_tree_length(tree) btree_length((btree_t*)(tree))
#define bsearch_tree_traversal(tree, array) \
	btree_inorder_traversal((btree_t*)(tree), array)
#define bsearch_tree_clean(tree) btree_clean((btree_t*)(tree))

/**
 * @file bsearch_tree.h
 * @brief Unbanlanced binary search trees definition
 * Defines functions to create, free and manipulate unbalanced binary search
 * trees
 */

/**
 * @defgroup bsearch_tree Unbalanced binary search trees
 * @ingroup btree
 * @{
 */

/**
 * @typedef bsearch_tree_t
 * @brief Typedef for the bsearch_tree structure
 *
 */
typedef struct bsearch_tree bsearch_tree_t;

/**
 * @struct bsearch_tree
 * @brief Unbalanced binary searcch tree
 *
 * A binary tree struct contains the size of the elements referenced by the
 * nodes and a pointer to a function to free them.
 * A binary search also has a pointer to a comparison function ti sort its
 * content.
 * @see node_bsearch_tree
 */
struct bsearch_tree {
	/**
	 * @brief Root of the binary tree
	 *
	 * If root == NULL the binary tree is empty
	 */
	node_btree_t* root;
	/**
	 * @brief Size (in bytes) on a element
	 *
	 * This field should be equals to the size (in bytes) of the memory region
	 * referenced by node_bsearch_tree_t#p
	 *
	 * \note This field is not used by any function and is mainly here for
	 * information.
	 */
	size_t size;
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
 * @brief Create an empty unbalanced binary search tree.
 *
 * @param[in] size Size of an element (the size of the element pointed by
 * node_bsearch_tree#p )
 * @return A pointer to the newly created unbalanced binary search tree
 * @see bsearch_tree_free()
 */
bsearch_tree_t* create_bsearch_tree(size_t size_bytes,
										compare_fn_t compare);

/**
 * @brief Inserts an element into the tree
 *
 * Insert an element into the tree. The tree will take ownership of the element
 * and free it (if bsearch_tree#free_element != NULL) when the tree is
 * freed.
 *
 * _Complexity_: \f$O(ln(n))\f$ if the tree is balanced, \f$O(n)\f$ otherwise
 * @param tree Pointer to the tree
 * @param value Pointer to the element to insert
 * @param found If the element was already in the tree and found != NULL then
 * after the call *found points to the node that contains the element equal to
 * value
 * @return ERROR_KEY_ALREADY_EXISTS if value was found in the tree and
 * ERROR_NO_ERROR otherwise
 */
int bsearch_tree_insert(bsearch_tree_t* tree,
						void* value,
						node_btree_t** found);

/**
 * @brief Clone and inserts an element into the tree
 *
 * Clone and insert an element into the tree.
 *
 * _Complexity_: \f$O(ln(n))\f$ if the tree is balanced, \f$O(n)\f$ otherwise
 * @param tree Pointer to the tree
 * @param value Pointer to the element to clone
 * @param found If the element was already in the tree and found != NULL then
 * after the call *found points to the node that contains the element equal to
 * value
 * @return ERROR_KEY_ALREADY_EXISTS if value was found in the tree and
 * ERROR_NO_ERROR otherwise
 */
int bsearch_tree_insert_clone(bsearch_tree_t* tree,
							  const void* value,
							  node_btree_t** found);

/**
 * @brief Finds if the element is present in the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @param value Pointer to the element to find
 * @return If value was found, returns the pointer to the node containing value,
 * otherwise returns NULL
 */
node_btree_t* bsearch_tree_find(bsearch_tree_t* tree,
										   void* value);

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
BOOL bsearch_tree_remove(bsearch_tree_t* tree, void* value);

/**
 * @brief Finds the minimum element of the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @return Returns NULL if the tree is empty and the node containing the minimum
 * element otherwise
 */
node_btree_t* bsearch_tree_min(bsearch_tree_t* tree);

/**
 * @brief Finds the maximum element of the tree
 *
 * _Complexity_: \f$O(ln(n))\f$
 * @param tree Pointer to the tree
 * @return Returns NULL if the tree is empty and the node containing the maximum
 * element otherwise
 */
node_btree_t* bsearch_tree_max(bsearch_tree_t* tree);

/**
 * @brief Apply left rotation on a tree node
 *
 * Before applying the left rotation on the "a" node
 * @dot
 * digraph before {
 *      graph [ordering="out"]
 *      node [style=filled]
 *      a [fillcolor=lightblue]
 *      b [fillcolor=lightskyblue]
 *      c [fillcolor=mediumpurple]
 *      null0 [shape=point]
 *      null1 [shape=point]
 *      null2 [shape=point]
 *      null3 [shape=point]
 *      a -> null0
 *      a -> b -> null1
 *      b -> c -> null2
 *      c -> null3
 * }
 * @enddot
 * After applying the left rotation on the "a" node
 * @dot
 * digraph after {
 *      graph [ordering="out"]
 *      node [style=filled]
 *      a [fillcolor=lightblue]
 *      b [fillcolor=lightskyblue]
 *      c [fillcolor=mediumpurple]
 *      null0 [shape=point]
 *      null1 [shape=point]
 *      null2 [shape=point]
 *      null3 [shape=point]
 *      b -> a -> null2
 *      a -> null3
 *      b -> c -> null0
 *      c -> null1
 * }
 * @enddot
 *
 * @param node Pointer to a node_bsearch_tree#ls, node_bsearch_tree#rs
 * or bsearch_tree#root attribute
 *
 */
void bsearch_tree_rotate_left(node_btree_t** node);

/**
 * @brief Apply right rotation on a tree node
 *
 * Before applying the right rotation on the "c" node
 * @dot
 * digraph before {
 *      node [style=filled]
 *      a [fillcolor=lightblue]
 *      b [fillcolor=lightskyblue]
 *      c [fillcolor=mediumpurple]
 *      null0 [shape=point]
 *      null1 [shape=point]
 *      null2 [shape=point]
 *      null3 [shape=point]
 *      c -> b -> a -> null0
 *      a -> null1
 *      b -> null2
 *      c -> null3
 * }
 * @enddot
 * After applying the right rotation on the "c" node
 * @dot
 * digraph after {
 *      node [style=filled]
 *      a [fillcolor=lightblue]
 *      b [fillcolor=lightskyblue]
 *      c [fillcolor=mediumpurple]
 *      null0 [shape=point]
 *      null1 [shape=point]
 *      null2 [shape=point]
 *      null3 [shape=point]
 *      b -> a -> null0
 *      a -> null1
 *      b -> c -> null2
 *      c -> null3
 * }
 * @enddot
 *
 * @param node Pointer to a node_bsearch_tree#ls, node_bsearch_tree#rs
 * or bsearch_tree#root attribute
 *
 */
void bsearch_tree_rotate_right(node_btree_t** node);

#endif	// !BSEARCHTREE_H
