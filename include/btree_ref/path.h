#ifndef BTREE_REF_PATH_H
#define BTREE_REF_PATH_H

/**
 * @file path.h
 * @brief Binary tree path definition
 * Defines functions to manipulate a path in a binary tree.
 */

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
extern const btree_path_t ROOT_PATH;
/**
 * @brief A path leading to the left son of a btree_ref_t.
 *
 * Starting from a random node, it always leads to its left son.
 */
extern const btree_path_t LHS_PATH;
/**
 * @brief A path leading to the right son of the root of a btree_ref_t.
 *
 * Starting from a random node, it always leads to its right son.
 */
extern const btree_path_t RHS_PATH;

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

#endif
