#include "btree/rb_tree.h"
#include <stdint.h>
#include <stdlib.h>
#include "btree/bsearch_tree.h"
#include "btree/path.h"
#include "errors.h"
#include "stack_view.h"
#include "structures.h"
#include "test_macros.h"

// TODO: Implement rb_tree_check
/* BOOL rb_tree_check(bsearch_tree_t* tree) { */
/* 	when_null_ret(tree, -ERROR_INVALID_PARAM1); */
/* 	if (tree->root == NULL) */
/* 		return 0; */
/* 	unsigned i = 0; */
/**/
/* 	stack_view_t* forest = create_stack_view(sizeof(node_btree_t*)); */
/* 	when_null_ret(forest, -ERROR_ALLOCATION_FAILED); */
/**/
/* 	node_btree_t* node = (node_btree_t*)tree->root; */
/**/
/* 	stack_view_push(forest, node); */
/* 	while (empty_stack(forest) == FALSE) { */
/* 		stack_view_pop(forest, (void**)&node); */
/* 		BOOL red = node->red; */
/* 		if (node->rs != NULL) { */
/* 			stack_view_push(forest, node->rs); */
/* 			if (red == TRUE && node->rs->red == TRUE) */
/* 				return FALSE; */
/* 		} */
/* 		if (node->ls != NULL) { */
/* 			stack_view_push(forest, node->ls); */
/* 			if (red == TRUE && node->ls->red == TRUE) */
/* 				return FALSE; */
/* 		} */
/* 	} */
/* 	free_stack(forest); */
/* 	return i; */
/* } */

static unsigned get_child_index(node_btree_t* node) {
	return node->parent->rs == node;
}

node_btree_t** node_bsearch_tree_get_location(
	bsearch_tree_t* tree,
	node_btree_t* node);

static void insert_equilibrate_leaf_path(bsearch_tree_t* tree,
										 node_btree_t* node,
										 btree_path_t path) {
	node_btree_t **parents, **children;
	node_btree_t **grandparent_ptr, **parent_ptr;
	node_btree_t *grandparent, *parent, *uncle;

	while (node->parent != NULL) {
		// Case  (parent BLACK)
		if (node->parent->priv == RB_BLACK)
			return;	 // insertion complete
		// From now on parent is RED

		// Case 4 (parent is root and RED)
		if (node->parent->parent == NULL) {
			node->parent->priv = RB_BLACK;
			return;	 // insertion complete
		}

		grandparent_ptr = node_bsearch_tree_get_location(
			tree, (node_btree_t*)node->parent->parent);
		grandparent = (node_btree_t*)*grandparent_ptr;
		parents = &grandparent->ls;
		unsigned parent_index = get_child_index(node->parent);
		parent_ptr = (node_btree_t**)&parents[parent_index];
		parent = (node_btree_t*)*parent_ptr;
		uncle = parents[1 - parent_index];
		children = &parent->ls;

		// Case 5 and 6 (uncle is considered BLACK)
		if (uncle == NULL || uncle->priv == RB_BLACK) {
			// Case 5 (inner node)
			if (parent_index != get_child_index(node)) {
				if (parent_index == 1)
					bsearch_tree_rotate_right(parent_ptr);
				else
					bsearch_tree_rotate_left(parent_ptr);
				node = parent;
				// Continue to Case 6
				continue;
			}
			// Case 6 (parent RED and uncle BLACK and outer grandchild)
			if (parent_index == 0)
				bsearch_tree_rotate_right(grandparent_ptr);
			else
				bsearch_tree_rotate_left(grandparent_ptr);
			parent->priv = RB_BLACK;
			grandparent->priv = RB_RED;
			return;
		}
		// Case 2 (parent and uncle are RED)
		parent->priv = RB_BLACK;
		uncle->priv = RB_BLACK;
		grandparent->priv = RB_RED;
		node = grandparent;
	}
}

typedef node_btree_t* (
	*create_bsearch_leaf_fn_t)(void* value, node_btree_t* parent);

int bsearch_tree_insert_impl(bsearch_tree_t* tree,
							 void* value,
							 node_btree_t** found,
							 btree_path_t* path,
                             uintptr_t priv_init);

int rb_tree_insert(bsearch_tree_t* tree,
				   void* value,
				   node_btree_t** found) {
	btree_path_t path;
	node_btree_t* node;
	int ret =
		bsearch_tree_insert_impl(tree, value, &node, &path, RB_RED);
    if(found != NULL)
        *found = node;
    if (ret == -ERROR_KEY_ALREADY_EXISTS)
		return ret;
	insert_equilibrate_leaf_path(tree, node, path);
	return -ERROR_NO_ERROR;
}

node_btree_t* bsearch_tree_remove_impl(bsearch_tree_t* tree,
												  void* value,
												  btree_path_t* path);

static void remove_equilibrate_leaf_path(bsearch_tree_t* tree,
										 node_btree_t* node,
										 btree_path_t path) {
	int child_index;
	node_btree_t **siblings, **nephews;
	node_btree_t **parent_ptr, **brother_ptr;
	node_btree_t *parent, *brother, *close, *distant;
	// Only zero or one-child nodes can be removed
	// If a two node is to be remove we swap its value with the value of its
	// successor and try to remove its successor (see resolve_remove in
	// bsearch_tree.c)
	BOOL has_right = node->rs != NULL;
	BOOL has_left = node->ls != NULL;
	if (has_right && !has_left) {
		node->rs->priv = RB_BLACK;
		return;
	}
	if (has_left && !has_right) {
		node->ls->priv = RB_BLACK;
		return;
	}
	parent = node->parent;
	if (parent == NULL || node->priv == RB_RED)
		return;
	// node is not the root, has no child and is BLACK
	do {
		child_index = path_last_direction(path);
		parent_ptr = node_bsearch_tree_get_location(
			tree, (node_btree_t*)parent);
		siblings = &parent->ls;
		brother_ptr = (node_btree_t**)&siblings[1 - child_index];
		brother = (node_btree_t*)*brother_ptr;
		nephews = &brother->ls;
		close = nephews[child_index];
		distant = nephews[1 - child_index];
		// Case 3: Sibling is RED
		if (brother->priv == RB_RED) {
			if (child_index == 0)
				bsearch_tree_rotate_left(parent_ptr);
			else
				bsearch_tree_rotate_right(parent_ptr);
			parent->priv = RB_RED;
			brother->priv = RB_BLACK;
			continue;
		}
		// Sibling is BLACK
		BOOL close_is_red = close != NULL && close->priv == TRUE;
		BOOL distant_is_red = distant != NULL && distant->priv == TRUE;
		if (close_is_red || distant_is_red) {
			// Case 5: Close nephew is RED
			if (close_is_red == TRUE) {
				if (child_index == 0)
					bsearch_tree_rotate_right(brother_ptr);
				else
					bsearch_tree_rotate_left(brother_ptr);
				brother->priv = RB_RED;
				close->priv = RB_BLACK;
				distant = brother;
				brother = close;
			}
			// Case 6: Distant nephew is RED
			if (child_index == 0)
				bsearch_tree_rotate_left(parent_ptr);
			else
				bsearch_tree_rotate_right(parent_ptr);
			brother->priv = parent->priv;
			parent->priv = RB_BLACK;
			distant->priv = RB_BLACK;
			return;
		}
		// Both nephews are BLACK
		// Case 4: parent is RED
		if (parent->priv == RB_RED) {
			brother->priv = RB_RED;
			parent->priv = RB_BLACK;
			return;
		}
		// Case 2: parent, brother and both nephews are BLACK
		brother->priv = RB_RED;
		parent = parent->parent;
		path.length -= 1;
	} while (parent != NULL);
	return;
}

BOOL rb_tree_remove(bsearch_tree_t* tree, void* value) {
	btree_path_t path;
	node_btree_t* node =
		(node_btree_t*)bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	if (tree->root != NULL)
		remove_equilibrate_leaf_path(tree, node, path);
	free(node);
	return TRUE;
}

BOOL rb_tree_is_node_red(node_btree_t* node) {
    // An absence of node is equivalent to the presence of a black node
	if (node == NULL)
		return FALSE;
	return node->priv == RB_RED;
}
