#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stddef.h>
#include "btree_ref/path.h"
#include "ptr.h"

/**
 * @file btree_ref.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
 */

/**
 * @defgroup btree_ref Binary trees
 * @{
 */

/**
 * @typedef node_btree_ref_t
 * @brief Typedef for the node_btree_ref structure
 *
 */
typedef struct node_btree_ref node_btree_ref_t;

/**
 * @struct node_btree_ref
 * @brief A binary-tree node.
 *
 * A tree node holds a pointer to its data and a pointer to its left son (ls)
 * (NULL if there is no left son) and a pointer to its right son (NULL if it is
 * there is no right son).
 */
struct node_btree_ref {
	void* p;			  /**< Pointer to data */
	node_btree_ref_t* ls; /**< Pointer to its left son */
	node_btree_ref_t* rs; /**< Pointer to its right son */
};

/**
 * @typedef btree_ref_t
 * @brief Typedef for the btree_ref structure
 *
 */
typedef struct btree_ref btree_ref_t;

/**
 * @struct btree_ref
 * @brief Binary tree
 *
 * A binary tree struct contains the size of the elements referenced by the
 * nodes and a pointer to a function to free them.
 * @see node_btree_ref
 */
struct btree_ref {
	/**
	 * @brief Size (in bytes) on a element
	 *
	 * This field should be equals to the size (in bytes) of the memory region
	 * referenced by node_btree_ref_t#p
	 */
	size_t size;
	/**
	 * @brief Function used to free the elements
	 *
	 * This field should be a pointer to a function that will be called to free
	 * the memory region referenced by node_btree_ref_t#p. If the btree was
	 * created using the API it will default to free from libc. However you can
	 * turn a btree in a simple view on the data by setting this pointer to
	 * NULL.
	 */
	free_element_fn_t free_element;
	/**
	 * @brief Root of the binary tree
	 *
	 * If root == NULL the binary tree is empty
	 */
	node_btree_ref_t* root;
};

/**
 * @brief Create an empty binary tree.
 *
 * @param[in] size Size of an element (the size of the element pointed by
 * node_btree_ref#p )
 * @return A pointer to the newly created btree
 * @see btree_free()
 */
btree_ref_t* create_btree(size_t size);

/**
 * @brief Return the binary tree height
 *
 * @param[in] tree pointer to the tree
 * @return height of the tree, 0 if tree == NULL, 1 if there is only the root
 */
unsigned btree_height(btree_ref_t* tree);

/**
 * @brief Return the count of nodes in the tree
 *
 * @param[in] tree pointer to the tree
 */
unsigned btree_length(btree_ref_t* tree);

/**
 * @brief Return the son of the tree node at the end of the branch indicated by
 * the first bit of the given path
 *
 * This function will return a pointer to the node_btree_ref#ls or
 * node_btree_ref#rs depending on the first bit of the path. The path will be
 * modified to remove the first branch so that next call with the same path and
 * the returned node will return the following node in the path.
 *
 * _Complexity:_ \f$O(1)\f$
 * @param[in] tree pointer to the tree
 * @param[inout] p pointer to the path to the next node.
 * @return a pointer to node_btree_ref#ls or node_btree_ref#rs
 */
node_btree_ref_t** btree_next_node(node_btree_ref_t* tree, btree_path_t* p);

/**
 * @brief Emplace a node in a binary tree at a given path
 *
 * This function write a value in a binary tree at the position given by a path.
 * If a node already exists at this position, node_btree_ref#p will be
 * overwritten by the p parameter. If a node doesn't exists at this position but
 * the parent node exists a node will be created. Otherwise nothing is modified
 * and the value NULL is returned.
 *
 * _Complexity:_ \f$O(path.length)\f$
 * @param[out] tree pointer to the tree
 * @param[in] path path indicating where the value should be written
 * @param[in] p value to be referenced by the node
 * @return a reference to the node where the reference has be written (NULL if
 * the node couldn't be created)
 */
node_btree_ref_t* btree_emplace_at(btree_ref_t* tree,
								   btree_path_t path,
								   void* p);

/**
 * @brief Emplace an array of node following a path in a binary tree.
 *
 * This function write values in a binary tree alongside a given path.
 * It will starts from the root and and as soon as the number of the node in the
 * path is greater than offset the pointer in the value array are emplaced one
 * by one.<br>
 * All the nodes (numbered with i) on the path are created:
 *  - with a NULL value if i is smaller than offset
 *  - with a NULL value if i if greater than offset + length
 *  - with values[i - offset] otherwise
 *
 * _Complexity:_ \f$O(path.length)\f$
 * @param tree pointer to the tree
 * @param path path alongside we want to place the values
 * @param values the pointers to data to be written into the node
 * @param length length of the values array
 * @param offset first node in the path which should be be modified with a value
 * @return The number of element from values written into the binary tree in
 * case of succes. A negative error otherwise.
 */
int btree_emplace_path(btree_ref_t* tree,
					   btree_path_t path,
					   void* values[],
					   size_t length,
					   size_t offset);

/**
 * @brief Create a perfect binary tree from an array
 *
 * The bijection array <-> btree is done following the canonical numbering of
 * the leaves of a perfect binary tree.<br>
 * **The created binary tree should be freed using the btree_free() function**
 *
 * _Complexity:_ \f$O(size)\f$
 * @param tab pointer to the array to copy
 * @param size size (in bytes) of the elements in the array
 * @param length length of the array
 * @return a perfect binary tree referencing the copied values
 * @see btree_free()
 */
btree_ref_t* btree_perfect_tree_from_tab(void* tab,
										 size_t size,
										 unsigned length);

/**
 * @brief Preorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order node -> left son ->
 * right son.<br>
 * For each node the value of node_btree_ref#p is written in the tab array in
 * the order of the traversal.
 *
 * _Complexity:_ \f$O(n)\f$
 * @param tree pointer to the tree
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 * @return The number of node in the tree in case of success, a negative error
 * otherwise.
 */
int btree_preorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Postorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order: left son -> right son
 * -> node.<br>
 * For each node the value of node_btree_ref#p is written in the tab array in
 * the order of the traversal.
 *
 * _Complexity:_ \f$O(n)\f$
 * @param tree pointer to the tree.
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 * @return The number of node in the tree in case of success, a negative error
 * otherwise.
 */
int btree_postorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Inorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order: left son -> node ->
 * right son.<br>
 * For each node the value of node_btree_ref#p is written in the
 * tab array in the order of the traversal.
 *
 * _Complexity:_ \f$O(n)\f$
 * @param tree pointer to the tree.
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 * @return The number of node in the tree in case of success, a negative error
 * otherwise.
 */
int btree_inorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Level-order traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree level by level (from left to
 * right).<br>
 * For each node the value of node_btree_ref#p is written in the tab
 * array in the order of the traversal.
 *
 * _Complexity:_ \f$O(n)\f$
 * @param tree pointer to the tree.
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 */
int btree_levelorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Frees the binary tree and the owned elements.
 *
 * Frees the data referenced by the nodes of the binary tree with the
 * btree_ref_t#free_element if its not NULL.
 *
 * _Complexity:_ \f$O(n)\f$
 * @param tree pointer to the tree
 */
void btree_free(btree_ref_t* tree);

/** @} */  // end of btree_ref

#endif
