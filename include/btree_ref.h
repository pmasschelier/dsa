#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stddef.h>
#include "ptr.h"

/**
 * @file btree_ref.h
 * @brief Binary trees definition
 * Defines functions to create, free and manipulate binary trees
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
	size_t size;
	free_element_fn_t free_element;
	node_btree_ref_t* root;
};

typedef struct btree_path btree_path_t;
/**
 * @class btree_path
 * @brief Path to a node in a binary tree
 *
 * A path in a binary tree is downward starting from a given node
 * (by default the root node). Each bit, starting from the 0th bit (LSB)
 * and up to the length-th bit (MSB), describe which branch should be taken.
 * A 0 bit means "left" and a 1 bit means "right".
 *
 * \see ROOT_PATH LHS_PATH RHS_PATH
 *
 */
struct btree_path {
	unsigned char length;
	long unsigned int path;
};

/**
 * @brief A path leading to root of a btree_ref_t.
 *
 * Starting from a random node, it always leads to the node itself.
 */
const btree_path_t ROOT_PATH = {0, 0};
/**
 * @brief A path leading to the left son of a btree_ref_t.
 *
 * Starting from a random node, it always leads to its left son.
 */
const btree_path_t LHS_PATH = {1, 0};
/**
 * @brief A path leading to the right son of the root of a btree_ref_t.
 *
 * Starting from a random node, it always leads to its right son.
 */
const btree_path_t RHS_PATH = {1, 1};

/**
 * @brief Add a left branch to the path
 *
 * The path is extended of 1 branch by turning left.
 *
 * @param[inout] path_from_root the path to extend
 * @return a pointer to the extended path (allow to chain calls to this
 * function)
 */
btree_path_t* path_lhs(btree_path_t* path_from_root);

/**
 * @brief Add a right branch to the path
 *
 * The path is extended of 1 branch by turning right
 *
 * @param[inout] path_from_root the path to extend
 * @return a pointer to the extended path (allow to chain calls to this
 * function)
 */
btree_path_t* path_rhs(btree_path_t* path_from_root);

/**
 * @brief Concatenate two paths
 *
 * The path path_from_root is extended with next branch.
 *
 * @param path_from_root the path to extend
 * @param next_branch the branch to append
 *
 * @see path_lhs() path_rhs()
 */
void path_append_branch(btree_path_t* path_from_root, btree_path_t next_branch);

/**
 * @brief Returns the path corresponding to the numbering of a perfect binary
 * tree (canonical heap numbering).
 *
 * @param[in] pos number of the node for which we want the path
 * @return the path to the node
 */
btree_path_t btree_node_to_path(long unsigned int pos);

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
 * @param tree pointer to the tree
 * @param path path alongside we want to place the values
 * @param values the pointers to data to be written into the node
 * @param length length of the values array
 * @param offset first node in the path which should be be modified with a value
 */
void btree_emplace_path(btree_ref_t* tree,
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
 * @param tab pointer to the array to copy
 * @param size size (in bytes) of the elements in the array
 * @param length length of the array
 * @return a perfect binary tree referencing the copied values
 * @see btree_free()
 */
btree_ref_t* btree_perfect_tree_from_tab(void* tab,
										 size_t size,
										 unsigned length);

/* \brief Parcours respectiverment en ordre prefixe, suffixe et infixe l'arbre
 * binaire et écrit les éléments dans le tableau
 * \param Arbre à parcourir
 * \param Tableau à remplir, il doit pointer vers une zone allouée de taille
 * suffisante, par exemple en ayant mesuré l'arbre au préalable avec
 * size_BT(tree) \return Nombre de noeuds de l'arbre.
 */
/**
 * @brief Preorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order node -> left son ->
 * right son.<br>
 * For each node the value of node_btree_ref#p is written in the tab array in
 * the order of the traversal.
 *
 * @param tree pointer to the tree
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 */
unsigned btree_preorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Postorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order: left son -> right son
 * -> node.<br>
 * For each node the value of node_btree_ref#p is written in the tab array in
 * the order of the traversal.
 *
 * @param tree pointer to the tree.
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 */
unsigned btree_postorder_traversal(btree_ref_t* tree, void* tab[]);

/**
 * @brief Inorder traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree in the order: left son -> node ->
 * right son.<br>
 * For each node the value of node_btree_ref#p is written in the
 * tab array in the order of the traversal.
 *
 * @param tree pointer to the tree.
 * @param tab array to fill, it should be allocated with a sufficient size, the
 * count of node can be get with the btree_length() function.
 */
unsigned btree_inorder_traversal(btree_ref_t* tree, void* tab[]);

/* \brief Parcours en largeur de l'arbre binaire
 * \param Arbre binaire à parcourir
 * \param Pointeur vers le tableau à remplir dans l'ordre du parcours
 * Ce dernier doit avoir été alloué avec une taille suffisante
 * (cf. size(tree))
 * \return 0 si tout c'est bien passé et -1 en cas d'erreur
 */
/**
 * @brief Level-order traversal of the binary tree.
 *
 * Iterate over the nodes of a binary tree level by level (from left to
 * right).<br>
 * For each node the value of node_btree_ref#p is written in the tab
 * array in the order of the traversal.
 *
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
 * @param tree pointer to the tree
 */
void btree_free(btree_ref_t* tree);

#endif
