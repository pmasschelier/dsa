#include "btree_ref/avl_tree_ref.h"
#include <stdint.h>
#include <stdlib.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/path.h"
#include "errors.h"
#include "structures.h"
#include "test_macros.h"

typedef struct node_avl_tree_ref node_avl_tree_ref_t;

struct node_avl_tree_ref {
	void* p;					 /**< Pointer to data */
	node_avl_tree_ref_t* ls;	 /**< Pointer to its left son */
	node_avl_tree_ref_t* rs;	 /**< Pointer to its right son */
	node_avl_tree_ref_t* father; /**< Pointer to its father */
	unsigned subtree; /**< Heigh of the subtree whom this node is the root */
};

typedef enum node_situation {
	NO_CHILDREN = 3,
	LEFT_CHILD = 1,
	RIGHT_CHILD = 2,
	BOTH_CHILDREN = 0
} node_situation_t;

/**
 * @brief Returns an integer representing the situation of the node
 *
 * @param node A pointer to the node
 * @return Its encoded situation
 */
static node_situation_t compute_situation(node_avl_tree_ref_t* node) {
	return ((node->ls == NULL) << 1) | (node->rs == NULL);
}

static BOOL update_subtree(node_avl_tree_ref_t* node) {
	unsigned subtree = 1;
	switch (compute_situation(node)) {
	case BOTH_CHILDREN:
		subtree = 1 + MAX(node->ls->subtree, node->rs->subtree);
		break;
	case LEFT_CHILD:
		subtree = 1 + node->ls->subtree;
		break;
	case RIGHT_CHILD:
		subtree = 1 + node->rs->subtree;
		break;
	default:
		break;
	}
	if (subtree == node->subtree)
		return FALSE;
	node->subtree = subtree;
	return TRUE;
}

static int balance_factor(node_avl_tree_ref_t* node) {
	int bf_ls = 0, bf_rs = 0;
	if (node->ls != NULL)
		bf_ls = node->ls->subtree;
	if (node->rs != NULL)
		bf_rs = node->rs->subtree;
	return bf_ls - bf_rs;
}

static void equilibrate(node_avl_tree_ref_t** node) {
	int bf = balance_factor(*node);
	node_bsearch_tree_ref_t** bsearch_node = (node_bsearch_tree_ref_t**)node;
	if (bf == -2) {
		(*node)->subtree -= 2;
		if (balance_factor((*node)->rs) == 1) {
			(*node)->rs->subtree -= 1;
			(*node)->rs->ls->subtree += 1;
			bsearch_tree_rotate_right(&(*bsearch_node)->rs);
		}
		if (balance_factor((*node)->rs) == -1)
			bsearch_tree_rotate_left(bsearch_node);
	} else if (bf == 2) {
		(*node)->subtree -= 2;
		if (balance_factor((*node)->ls) == -1) {
			(*node)->ls->subtree -= 1;
			(*node)->ls->rs->subtree += 1;
			bsearch_tree_rotate_left(&(*bsearch_node)->ls);
		}
		if (balance_factor((*node)->ls) == 1)
			bsearch_tree_rotate_right(bsearch_node);
	}
}

static node_avl_tree_ref_t* create_avl_leaf(void* value,
											node_avl_tree_ref_t* father) {
	node_avl_tree_ref_t* leaf = malloc(sizeof(node_avl_tree_ref_t));
	when_null_ret(leaf, NULL);
	leaf->p = value;
	leaf->ls = NULL;
	leaf->rs = NULL;
	leaf->subtree = 1;
	leaf->father = father;
	return leaf;
}

static void equilibrate_leaf_path(bsearch_tree_ref_t* tree,
								  node_avl_tree_ref_t* node,
								  btree_path_t path) {
	path.length -= 1;
	node_avl_tree_ref_t** node_ptr = NULL;
	while (node->father != NULL && node->father->father != NULL) {
		node_ptr = path_walk_backward(&path, &node->father->father->ls,
									  &node->father->father->rs);
		node = *node_ptr;
		if (update_subtree(node) == FALSE)
			return;
		equilibrate(node_ptr);
	}
	if (tree->root != NULL &&
		update_subtree((node_avl_tree_ref_t*)tree->root) == TRUE)
		equilibrate((node_avl_tree_ref_t**)&tree->root);
}

typedef node_bsearch_tree_ref_t* (
	*create_bsearch_leaf_fn_t)(void* value, node_bsearch_tree_ref_t* father);

int bsearch_tree_insert_impl(bsearch_tree_ref_t* tree,
							 void* value,
							 node_bsearch_tree_ref_t** found,
							 btree_path_t* path,
							 node_bsearch_tree_ref_t** node,
							 create_bsearch_leaf_fn_t create_leaf);

int avl_tree_insert(bsearch_tree_ref_t* tree,
					void* value,
					node_bsearch_tree_ref_t** found) {
	btree_path_t path;
	node_bsearch_tree_ref_t* node;
	int ret =
		bsearch_tree_insert_impl(tree, value, found, &path, &node,
								 (create_bsearch_leaf_fn_t)create_avl_leaf);
	if (ret == -ERROR_KEY_ALREADY_EXISTS)
		return ret;
	equilibrate_leaf_path(tree, (node_avl_tree_ref_t*)node, path);
	return -ERROR_NO_ERROR;
}

node_bsearch_tree_ref_t* bsearch_tree_remove_impl(bsearch_tree_ref_t* tree,
												  void* value,
												  btree_path_t* path);

BOOL avl_tree_remove(bsearch_tree_ref_t* tree, void* value) {
	btree_path_t path;
	node_avl_tree_ref_t* node =
		(node_avl_tree_ref_t*)bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	equilibrate_leaf_path(tree, node, path);
	free(node);
	return TRUE;
}

unsigned avl_tree_height(bsearch_tree_ref_t* tree) {
	node_avl_tree_ref_t* root = (node_avl_tree_ref_t*)tree->root;
	return root == NULL ? 0 : root->subtree;
}
