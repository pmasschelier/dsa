#include "btree_ref/bsearch_tree_ref.h"
#include <stdlib.h>
#include "btree_ref/path.h"
#include "errors.h"
#include "test_macros.h"

// TODO: Make bsearch tree order-statistic trees

bsearch_tree_ref_t* create_bsearch_tree(size_t size_bytes,
										compare_fn_t compare) {
	bsearch_tree_ref_t* ret = malloc(sizeof(bsearch_tree_ref_t));
	when_null_ret(ret, NULL);
	ret->compare = compare;
	ret->free_element = free;
	ret->size = size_bytes;
	ret->root = NULL;
	return ret;
}

node_bsearch_tree_ref_t** node_bsearch_tree_get_location(
	bsearch_tree_ref_t* tree,
	node_bsearch_tree_ref_t* node) {
	when_null_ret(tree, NULL);
	when_null_ret(node, NULL);
	if (node->father == NULL)
		return &tree->root;
	if (node->father->ls == node)
		return &node->father->ls;
	return &node->father->rs;
}

static node_bsearch_tree_ref_t* create_bsearch_leaf(
	void* value,
	node_bsearch_tree_ref_t* father) {
	node_bsearch_tree_ref_t* leaf = malloc(sizeof(node_bsearch_tree_ref_t));
	when_null_ret(leaf, NULL);
	leaf->p = value;
	leaf->ls = NULL;
	leaf->rs = NULL;
	leaf->father = father;
	return leaf;
}

BOOL bsearch_tree_is_left_son(node_bsearch_tree_ref_t* node) {
	return node->father->ls == node;
}

typedef node_bsearch_tree_ref_t* (
	*create_bsearch_leaf_fn_t)(void* value, node_bsearch_tree_ref_t* father);

int bsearch_tree_insert_impl(bsearch_tree_ref_t* tree,
							 void* value,
							 node_bsearch_tree_ref_t** found,
							 btree_path_t* path,
							 node_bsearch_tree_ref_t** created,
							 create_bsearch_leaf_fn_t create_leaf) {
	node_bsearch_tree_ref_t* father = NULL;
	*path = ROOT_PATH;
	if (tree->root == NULL) {
		*created = tree->root = create_leaf(value, father);
		return -ERROR_NO_ERROR;
	}

	node_bsearch_tree_ref_t** node = &tree->root;
	do {
		father = *node;
		int cmp = tree->compare(value, (*node)->p);
		if (cmp == -1) {
			path_lhs(path);
			node = &(*node)->ls;
		} else if (cmp == 1) {
			path_rhs(path);
			node = &(*node)->rs;
		} else {
			if (found != NULL)
				*found = *node;
			return -ERROR_KEY_ALREADY_EXISTS;
		}
	} while (*node != NULL);
	*node = create_leaf(value, father);
	when_null_ret(*node, -ERROR_ALLOCATION_FAILED);
	*created = *node;
	return -ERROR_NO_ERROR;
}

int bsearch_tree_insert(bsearch_tree_ref_t* tree,
						void* value,
						node_bsearch_tree_ref_t** found) {
	btree_path_t path;
	node_bsearch_tree_ref_t* node;
	return bsearch_tree_insert_impl(tree, value, found, &path, &node,
									create_bsearch_leaf);
}

node_bsearch_tree_ref_t* bsearch_tree_find(bsearch_tree_ref_t* tree,
										   void* value) {
	node_bsearch_tree_ref_t* node = tree->root;
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

node_bsearch_tree_ref_t** bsearch_tree_successor_location(
	bsearch_tree_ref_t* tree,
	node_bsearch_tree_ref_t* node,
	btree_path_t* path) {
	if (node->rs != NULL) {
		node_bsearch_tree_ref_t** ret = &node->rs;
		if (path != NULL)
			path_rhs(path);
		while ((*ret)->ls != NULL) {
			ret = &(*ret)->ls;
			if (path != NULL)
				path_lhs(path);
		}
		return ret;
	}
	while (node->father != NULL && node->father->rs == node) {
		node = node->father;
		if (path != NULL)
			path->length -= 1;
	}
	if (node->father == NULL)
		return NULL;
	if (path != NULL)
		path->length -= 1;
	return node_bsearch_tree_get_location(tree, node->father);
}

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
static node_situation_t compute_situation(node_bsearch_tree_ref_t* node) {
	return ((node->ls == NULL) << 1) | (node->rs == NULL);
}

static node_bsearch_tree_ref_t* resolve_remove(
	bsearch_tree_ref_t* tree,
	node_bsearch_tree_ref_t** node_ptr,
	btree_path_t* path) {
	node_bsearch_tree_ref_t* node = *node_ptr;
	node_bsearch_tree_ref_t** freed = NULL;
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
		freed = bsearch_tree_successor_location(tree, node, path);
		// exchange value of node and *succ
		void* node_p = node->p;
		node->p = (*freed)->p;
		(*freed)->p = node_p;
		// the only remaining reference on *freed will be node
		return resolve_remove(tree, freed, path);
	case NO_CHILDREN:
		*node_ptr = NULL;
	default:
		break;
	}
	if (node->p != NULL && tree->free_element != NULL)
		tree->free_element(node->p);
	return node;
}

node_bsearch_tree_ref_t* bsearch_tree_remove_impl(bsearch_tree_ref_t* tree,
												  void* value,
												  btree_path_t* path) {
	node_bsearch_tree_ref_t** node_ptr = &tree->root;
	*path = ROOT_PATH;
	while (*node_ptr != NULL) {
		int cmp = tree->compare(value, (*node_ptr)->p);
		if (cmp == -1) {
			path_lhs(path);
			node_ptr = &(*node_ptr)->ls;
		} else if (cmp == 1) {
			path_rhs(path);
			node_ptr = &(*node_ptr)->rs;
		} else
			break;
	}
	if (*node_ptr == NULL)
		return NULL;
	return resolve_remove(tree, node_ptr, path);
}

BOOL bsearch_tree_remove(bsearch_tree_ref_t* tree, void* value) {
	btree_path_t path;
	node_bsearch_tree_ref_t* node =
		bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	free(node);
	return TRUE;
}

node_bsearch_tree_ref_t* bsearch_tree_min(bsearch_tree_ref_t* tree) {
	node_bsearch_tree_ref_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->ls != NULL)
		node = node->ls;
	return node;
}

node_bsearch_tree_ref_t* bsearch_tree_max(bsearch_tree_ref_t* tree) {
	node_bsearch_tree_ref_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->rs != NULL)
		node = node->rs;
	return node;
}

void bsearch_tree_rotate_left(node_bsearch_tree_ref_t** node) {
	node_bsearch_tree_ref_t* root = *node;
	node_bsearch_tree_ref_t* right_son = (*node)->rs;

	*node = right_son;
	right_son->father = root->father;

	root->rs = right_son->ls;
	if (root->rs != NULL)
		root->rs->father = root;

	right_son->ls = root;
	root->father = right_son;
}

void bsearch_tree_rotate_right(node_bsearch_tree_ref_t** node) {
	node_bsearch_tree_ref_t* root = *node;
	node_bsearch_tree_ref_t* left_son = (*node)->ls;

	*node = left_son;
	left_son->father = root->father;

	root->ls = left_son->rs;
	if (root->ls != NULL)
		root->ls->father = root;

	left_son->rs = root;
	root->father = left_son;
}

// TODO: Implement join, split and union
