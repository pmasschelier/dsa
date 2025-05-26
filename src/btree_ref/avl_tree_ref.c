#include "btree_ref/avl_tree_ref.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/path.h"
#include "errors.h"
#include "structures.h"
#include "test_macros.h"

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
static node_situation_t compute_situation(node_btree_ref_t* node) {
	return ((node->ls == NULL) << 1) | (node->rs == NULL);
}

static BOOL update_subtree(node_btree_ref_t* node) {
	unsigned subtree = 1;
	switch (compute_situation(node)) {
	case BOTH_CHILDREN:
		subtree = 1 + MAX(node->ls->priv, node->rs->priv);
		break;
	case LEFT_CHILD:
		subtree = 1 + node->ls->priv;
		break;
	case RIGHT_CHILD:
		subtree = 1 + node->rs->priv;
		break;
	default:
		break;
	}
	if (subtree == node->priv)
		return FALSE;
	node->priv = subtree;
	return TRUE;
}

static int balance_factor(node_btree_ref_t* node) {
	int bf_ls = 0, bf_rs = 0;
	if (node->ls != NULL)
		bf_ls = node->ls->priv;
	if (node->rs != NULL)
		bf_rs = node->rs->priv;
	return bf_ls - bf_rs;
}

static void equilibrate(node_btree_ref_t** node) {
	int bf = balance_factor(*node);
	node_btree_ref_t** bsearch_node = (node_btree_ref_t**)node;
	if (bf == -2) {
		(*node)->priv -= 2;
		if (balance_factor((*node)->rs) == 1) {
			(*node)->rs->priv -= 1;
			(*node)->rs->ls->priv += 1;
			bsearch_tree_rotate_right(&(*bsearch_node)->rs);
		}
		if (balance_factor((*node)->rs) == -1)
			bsearch_tree_rotate_left(bsearch_node);
	} else if (bf == 2) {
		(*node)->priv -= 2;
		if (balance_factor((*node)->ls) == -1) {
			(*node)->ls->priv -= 1;
			(*node)->ls->rs->priv += 1;
			bsearch_tree_rotate_left(&(*bsearch_node)->ls);
		}
		if (balance_factor((*node)->ls) == 1)
			bsearch_tree_rotate_right(bsearch_node);
	}
}

static void equilibrate_leaf_path(bsearch_tree_ref_t* tree,
								  node_btree_ref_t* node,
								  btree_path_t path) {
	path.length -= 1;
	node_btree_ref_t** node_ptr = NULL;
	while (node->parent != NULL && node->parent->parent != NULL) {
		node_ptr = path_walk_backward(&path, &node->parent->parent->ls,
									  &node->parent->parent->rs);
		node = *node_ptr;
		if (update_subtree(node) == FALSE)
			return;
		equilibrate(node_ptr);
	}
	if (tree->root != NULL &&
		update_subtree((node_btree_ref_t*)tree->root) == TRUE)
		equilibrate((node_btree_ref_t**)&tree->root);
}

typedef node_btree_ref_t* (
	*create_bsearch_leaf_fn_t)(void* value, node_btree_ref_t* parent);

int bsearch_tree_insert_impl(bsearch_tree_ref_t* tree,
							 void* value,
							 node_btree_ref_t** found,
							 btree_path_t* path,
                             uintptr_t priv_init);

int avl_tree_insert(bsearch_tree_ref_t* tree,
					void* value,
					node_btree_ref_t** found) {
	btree_path_t path;
	node_btree_ref_t* node;
	int ret =
		bsearch_tree_insert_impl(tree, value, &node, &path, 1);
    if(found)
        *found = node;
    if (ret == -ERROR_KEY_ALREADY_EXISTS)
		return ret;
	equilibrate_leaf_path(tree, (node_btree_ref_t*)node, path);
	return -ERROR_NO_ERROR;
}

int avl_tree_insert_clone(bsearch_tree_ref_t* tree,
						  const void* value,
						  node_btree_ref_t** found) {
	void* copy = malloc(tree->size);
	when_null_ret(copy, -ERROR_ALLOCATION_FAILED);
	memcpy(copy, value, tree->size);
	int ret = avl_tree_insert(tree, copy, found);
	if (ret != -ERROR_NO_ERROR)
		free(copy);
	return ret;
}

node_btree_ref_t* bsearch_tree_remove_impl(bsearch_tree_ref_t* tree,
												  void* value,
												  btree_path_t* path);

BOOL avl_tree_remove(bsearch_tree_ref_t* tree, void* value) {
	btree_path_t path;
	node_btree_ref_t* node =
		(node_btree_ref_t*)bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	equilibrate_leaf_path(tree, node, path);
	free(node);
	return TRUE;
}

unsigned avl_tree_height(bsearch_tree_ref_t* tree) {
	node_btree_ref_t* root = (node_btree_ref_t*)tree->root;
	return root == NULL ? 0 : root->priv;
}
