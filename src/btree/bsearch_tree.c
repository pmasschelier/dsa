#include "btree/bsearch_tree.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "btree/btree.h"
#include "btree/path.h"
#include "errors.h"
#include "test_macros.h"

// TODO: Make bsearch tree order-statistic trees

node_btree_t* create_btree_leaf(
	void* value,
	node_btree_t* parent,
    uintptr_t priv_init,
    size_t size_bytes);

bsearch_tree_t* create_bsearch_tree(size_t size_bytes,
										compare_fn_t compare) {
	bsearch_tree_t* ret = malloc(sizeof(bsearch_tree_t));
	when_null_ret(ret, NULL);
	ret->compare = compare;
	ret->size_bytes = size_bytes;
	ret->root = NULL;
	return ret;
}

node_btree_t** node_bsearch_tree_get_location(
	bsearch_tree_t* tree,
	node_btree_t* node) {
	when_null_ret(tree, NULL);
	when_null_ret(node, NULL);
	if (node->parent == NULL)
		return &tree->root;
	if (node->parent->ls == node)
		return &node->parent->ls;
	return &node->parent->rs;
}

BOOL bsearch_tree_is_left_son(node_btree_t* node) {
	return node->parent->ls == node;
}

int bsearch_tree_insert_impl(bsearch_tree_t* tree,
							 void* value,
							 node_btree_t** found,
							 btree_path_t* path,
                             uintptr_t priv_init) {
	node_btree_t* parent = NULL;
	node_btree_t** node = &tree->root;
    *path = ROOT_PATH;

	while(*node != NULL) {
		parent = *node;
		int cmp = tree->compare(value, (*node)->data);
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
	}
	*node = create_btree_leaf(value, parent, priv_init, tree->size_bytes);
	when_null_ret(*node, -ERROR_ALLOCATION_FAILED);
    if(found != NULL)
        *found = *node;
    return -ERROR_NO_ERROR;
}

int bsearch_tree_insert(bsearch_tree_t* tree,
						void* value,
						node_btree_t** found) {
	btree_path_t path;
	return bsearch_tree_insert_impl(tree, value, found, &path, 0);
}

int bsearch_tree_insert_clone(bsearch_tree_t* tree,
							  const void* value,
							  node_btree_t** found) {
	void* copy = malloc(tree->size_bytes);
	when_null_ret(copy, -ERROR_ALLOCATION_FAILED);
	memcpy(copy, value, tree->size_bytes);
	int ret = bsearch_tree_insert(tree, copy, found);
	if (ret != -ERROR_NO_ERROR)
		free(copy);
	return ret;
}

node_btree_t* bsearch_tree_find(bsearch_tree_t* tree,
										   void* value) {
	node_btree_t* node = tree->root;
	while (node != NULL) {
		int cmp = tree->compare(value, node->data);
		if (cmp == -1)
			node = node->ls;
		else if (cmp == 1)
			node = node->rs;
		else
			return node;
	}
	return NULL;
}

node_btree_t** bsearch_tree_successor_location(
	bsearch_tree_t* tree,
	node_btree_t* node,
	btree_path_t* path) {
	if (node->rs != NULL) {
		node_btree_t** ret = &node->rs;
		if (path != NULL)
			path_rhs(path);
		while ((*ret)->ls != NULL) {
			ret = &(*ret)->ls;
			if (path != NULL)
				path_lhs(path);
		}
		return ret;
	}
	while (node->parent != NULL && node->parent->rs == node) {
		node = node->parent;
		if (path != NULL)
			path->length -= 1;
	}
	if (node->parent == NULL)
		return NULL;
	if (path != NULL)
		path->length -= 1;
	return node_bsearch_tree_get_location(tree, node->parent);
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
static node_situation_t compute_situation(node_btree_t* node) {
	return ((node->ls == NULL) << 1) | (node->rs == NULL);
}

static node_btree_t* resolve_remove(
	bsearch_tree_t* tree,
	node_btree_t** node_ptr,
	btree_path_t* path)
{
	node_btree_t* node = *node_ptr;
	node_btree_t** freed = NULL;
	int situation = compute_situation(node);
	switch (situation) {
	case LEFT_CHILD:
		*node_ptr = node->ls;
		(*node_ptr)->parent = node->parent;
		break;
	case RIGHT_CHILD:
		*node_ptr = node->rs;
		(*node_ptr)->parent = node->parent;
		break;
	case BOTH_CHILDREN:
		freed = bsearch_tree_successor_location(tree, node, path);

		// exchange value of node and *succ
        btree_swap_node(&freed, &node_ptr);
		// the only remaining reference on *freed will be node
		return resolve_remove(tree, freed, path);
	case NO_CHILDREN:
		*node_ptr = NULL;
	default:
		break;
	}
	return node;
}

node_btree_t* bsearch_tree_remove_impl(bsearch_tree_t* tree,
												  void* value,
												  btree_path_t* path) {
	node_btree_t** node_ptr = &tree->root;
	*path = ROOT_PATH;
	while (*node_ptr != NULL) {
		int cmp = tree->compare(value, (*node_ptr)->data);
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

BOOL bsearch_tree_remove(bsearch_tree_t* tree, void* value) {
	btree_path_t path;
	node_btree_t* node =
		bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	free(node);
	return TRUE;
}

node_btree_t* bsearch_tree_min(bsearch_tree_t* tree) {
	node_btree_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->ls != NULL)
		node = node->ls;
	return node;
}

node_btree_t* bsearch_tree_max(bsearch_tree_t* tree) {
	node_btree_t* node = tree->root;
	if (node == NULL)
		return NULL;
	while (node->rs != NULL)
		node = node->rs;
	return node;
}

void bsearch_tree_rotate_left(node_btree_t** node) {
	node_btree_t* root = *node;
	node_btree_t* right_son = (*node)->rs;

	*node = right_son;
	right_son->parent = root->parent;

	root->rs = right_son->ls;
	if (root->rs != NULL)
		root->rs->parent = root;

	right_son->ls = root;
	root->parent = right_son;
}

void bsearch_tree_rotate_right(node_btree_t** node) {
	node_btree_t* root = *node;
	node_btree_t* left_son = (*node)->ls;

	*node = left_son;
	left_son->parent = root->parent;

	root->ls = left_son->rs;
	if (root->ls != NULL)
		root->ls->parent = root;

	left_son->rs = root;
	root->parent = left_son;
}

// TODO: Implement join, split and union
