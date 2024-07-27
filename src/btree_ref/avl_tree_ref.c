#include "btree_ref/avl_tree_ref.h"
#include <stdint.h>
#include <stdlib.h>
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

static void rotate_left(node_avl_tree_ref_t** node) {
	node_avl_tree_ref_t* root = *node;
	node_avl_tree_ref_t* right_son = (*node)->rs;

	*node = right_son;
	right_son->father = root->father;

	root->rs = right_son->ls;
	if (root->rs != NULL)
		root->rs->father = root;

	right_son->ls = root;
	root->father = right_son;
}

static void rotate_right(node_avl_tree_ref_t** node) {
	node_avl_tree_ref_t* root = *node;
	node_avl_tree_ref_t* left_son = (*node)->ls;

	*node = left_son;
	left_son->father = root->father;

	root->ls = left_son->rs;
	if (root->ls != NULL)
		root->ls->father = root;

	left_son->rs = root;
	root->father = left_son;
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
	if (bf == -2) {
		(*node)->subtree -= 2;
		if (balance_factor((*node)->rs) == 1) {
			(*node)->rs->subtree -= 1;
			(*node)->rs->ls->subtree += 1;
			rotate_right(&(*node)->rs);
			rotate_left(node);
		}
		if (balance_factor((*node)->rs) == -1)
			rotate_left(node);
	} else if (bf == 2) {
		(*node)->subtree -= 2;
		if (balance_factor((*node)->ls) == -1) {
			(*node)->ls->subtree -= 1;
			(*node)->ls->rs->subtree += 1;
			rotate_left(&(*node)->ls);
			rotate_right(node);
		}
		if (balance_factor((*node)->ls) == 1)
			rotate_right(node);
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

static void equilibrate_leaf_path(avl_tree_ref_t* tree,
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
	if (tree->root != NULL && update_subtree(tree->root) == TRUE)
		equilibrate(&tree->root);
}

int avl_tree_insert(avl_tree_ref_t* tree,
					void* value,
					node_avl_tree_ref_t** found) {
	node_avl_tree_ref_t* father = NULL;
	if (tree->root == NULL) {
		tree->root = create_avl_leaf(value, father);
		return -ERROR_NO_ERROR;
	}

	node_avl_tree_ref_t** node = &tree->root;
	btree_path_t path = ROOT_PATH;
	do {
		father = *node;
		int cmp = tree->compare(value, (*node)->p);
		if (cmp == -1) {
			path_lhs(&path);
			node = &(*node)->ls;
		} else if (cmp == 1) {
			path_rhs(&path);
			node = &(*node)->rs;
		} else {
			if (found != NULL)
				*found = *node;
			return -ERROR_KEY_ALREADY_EXISTS;
		}
	} while (*node != NULL);
	*node = create_avl_leaf(value, father);
	when_null_ret(*node, -ERROR_ALLOCATION_FAILED);
	equilibrate_leaf_path(tree, *node, path);
	return -ERROR_NO_ERROR;
}

node_avl_tree_ref_t* avl_tree_find(avl_tree_ref_t* tree, void* value) {
	node_avl_tree_ref_t* node = tree->root;
	while (node != NULL) {
		int cmp = tree->compare(value, node->p);
		if (cmp == -1)
			node = node->ls;
		else if (cmp == 1)
			node = node->rs;
		else
			return node;
	}
	return NULL;
}

node_avl_tree_ref_t** successor_node(node_avl_tree_ref_t* node) {
	node_avl_tree_ref_t** ret = &node->rs;
	while ((*ret)->ls != NULL)
		ret = &(*ret)->ls;
	return ret;
}

static node_avl_tree_ref_t* resolve_remove(avl_tree_ref_t* tree,
										   node_avl_tree_ref_t** node_ptr) {
	node_avl_tree_ref_t* node = *node_ptr;
	node_avl_tree_ref_t** freed = node_ptr;
	int situation = compute_situation(node);
	switch (situation) {
	case LEFT_CHILD:
		*node_ptr = node->ls;
		(*node_ptr)->father = node->father;
		break;
	case RIGHT_CHILD:
		*node_ptr = node->rs;
		(*node_ptr)->father = node->father;
		break;
	case BOTH_CHILDREN:
		freed = successor_node(node);
		// exchange value of node and *succ
		void* node_p = node->p;
		node->p = (*freed)->p;
		(*freed)->p = node_p;
		// the only remaining reference on *freed will be node
		node = *freed;
		*freed = NULL;
		break;
	case NO_CHILDREN:
		*node_ptr = NULL;
	default:
		break;
	}
	if (node->p != NULL && tree->free_element != NULL)
		tree->free_element(node->p);
	return node;
}

BOOL avl_tree_remove(avl_tree_ref_t* tree, void* value) {
	node_avl_tree_ref_t** node_ptr = &tree->root;
	btree_path_t path = ROOT_PATH;
	while (*node_ptr != NULL) {
		int cmp = tree->compare(value, (*node_ptr)->p);
		if (cmp == -1) {
			path_lhs(&path);
			node_ptr = &(*node_ptr)->ls;
		} else if (cmp == 1) {
			path_rhs(&path);
			node_ptr = &(*node_ptr)->rs;
		} else
			break;
	}
	if (*node_ptr == NULL)
		return FALSE;
	node_avl_tree_ref_t* node = resolve_remove(tree, node_ptr);
	equilibrate_leaf_path(tree, node, path);
	free(node);
	return TRUE;
}

node_avl_tree_ref_t* avl_tree_min(avl_tree_ref_t* tree) {
	node_avl_tree_ref_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->ls != NULL)
		node = node->ls;
	return node;
}

node_avl_tree_ref_t* avl_tree_max(avl_tree_ref_t* tree) {
	node_avl_tree_ref_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->rs != NULL)
		node = node->rs;
	return node;
}

avl_tree_ref_t* create_avl_tree(size_t size_bytes, compare_fn_t compare) {
	avl_tree_ref_t* ret = malloc(sizeof(avl_tree_ref_t));
	when_null_ret(ret, NULL);
	ret->compare = compare;
	ret->free_element = free;
	ret->size = size_bytes;
	ret->root = NULL;
	return ret;
}
