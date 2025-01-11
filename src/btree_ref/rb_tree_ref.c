#include "btree_ref/rb_tree_ref.h"
#include <stdint.h>
#include <stdlib.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/path.h"
#include "errors.h"
#include "stack_view.h"
#include "structures.h"
#include "test_macros.h"

typedef struct node_rb_tree_ref node_rb_tree_ref_t;

struct node_rb_tree_ref {
	void* p;					/**< Pointer to data */
	node_rb_tree_ref_t* ls;		/**< Pointer to its left son */
	node_rb_tree_ref_t* rs;		/**< Pointer to its right son */
	node_rb_tree_ref_t* father; /**< Pointer to its father */
	BOOL red;					/**< Is the node red ? */
};

// TODO: Implement rb_tree_check
/* BOOL rb_tree_check(bsearch_tree_ref_t* tree) { */
/* 	when_null_ret(tree, -ERROR_INVALID_PARAM1); */
/* 	if (tree->root == NULL) */
/* 		return 0; */
/* 	unsigned i = 0; */
/**/
/* 	stack_view_t* forest = create_stack_view(sizeof(node_btree_ref_t*)); */
/* 	when_null_ret(forest, -ERROR_ALLOCATION_FAILED); */
/**/
/* 	node_rb_tree_ref_t* node = (node_rb_tree_ref_t*)tree->root; */
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

static unsigned get_child_index(node_rb_tree_ref_t* node) {
	return node->father->rs == node;
}

node_bsearch_tree_ref_t** node_bsearch_tree_get_location(
	bsearch_tree_ref_t* tree,
	node_bsearch_tree_ref_t* node);

static void insert_equilibrate_leaf_path(bsearch_tree_ref_t* tree,
										 node_rb_tree_ref_t* node,
										 btree_path_t path) {
	node_rb_tree_ref_t **parents, **children;
	node_bsearch_tree_ref_t **grandfather_ptr, **father_ptr;
	node_rb_tree_ref_t *grandfather, *father, *uncle;

	while (node->father != NULL) {
		// Case  (father BLACK)
		if (node->father->red == FALSE)
			return;	 // insertion complete
		// From now on father is RED

		// Case 4 (father is root and RED)
		if (node->father->father == NULL) {
			node->father->red = FALSE;
			return;	 // insertion complete
		}

		grandfather_ptr = node_bsearch_tree_get_location(
			tree, (node_bsearch_tree_ref_t*)node->father->father);
		grandfather = (node_rb_tree_ref_t*)*grandfather_ptr;
		parents = &grandfather->ls;
		unsigned father_index = get_child_index(node->father);
		father_ptr = (node_bsearch_tree_ref_t**)&parents[father_index];
		father = (node_rb_tree_ref_t*)*father_ptr;
		uncle = parents[1 - father_index];
		children = &father->ls;

		// Case 5 and 6 (uncle is considered BLACK)
		if (uncle == NULL || uncle->red == FALSE) {
			// Case 5 (inner node)
			if (father_index != get_child_index(node)) {
				if (father_index == 1)
					bsearch_tree_rotate_right(father_ptr);
				else
					bsearch_tree_rotate_left(father_ptr);
				node = father;
				// Continue to Case 6
				continue;
			}
			// Case 6 (parent RED and uncle BLACK and outer grandchild)
			if (father_index == 0)
				bsearch_tree_rotate_right(grandfather_ptr);
			else
				bsearch_tree_rotate_left(grandfather_ptr);
			father->red = FALSE;
			grandfather->red = TRUE;
			return;
		}
		// Case 2 (father and uncle are RED)
		father->red = FALSE;
		uncle->red = FALSE;
		grandfather->red = TRUE;
		node = grandfather;
	}
}

static node_rb_tree_ref_t* create_rb_leaf(void* value,
										  node_rb_tree_ref_t* father) {
	node_rb_tree_ref_t* leaf = malloc(sizeof(node_rb_tree_ref_t));
	when_null_ret(leaf, NULL);
	leaf->p = value;
	leaf->ls = NULL;
	leaf->rs = NULL;
	leaf->red = TRUE;
	leaf->father = father;
	return leaf;
}

typedef node_bsearch_tree_ref_t* (
	*create_bsearch_leaf_fn_t)(void* value, node_bsearch_tree_ref_t* father);

int bsearch_tree_insert_impl(bsearch_tree_ref_t* tree,
							 void* value,
							 node_bsearch_tree_ref_t** found,
							 btree_path_t* path,
							 node_bsearch_tree_ref_t** node,
							 create_bsearch_leaf_fn_t create_leaf);

int rb_tree_insert(bsearch_tree_ref_t* tree,
				   void* value,
				   node_bsearch_tree_ref_t** found) {
	btree_path_t path;
	node_bsearch_tree_ref_t* node;
	int ret =
		bsearch_tree_insert_impl(tree, value, found, &path, &node,
								 (create_bsearch_leaf_fn_t)create_rb_leaf);
	if (ret == -ERROR_KEY_ALREADY_EXISTS)
		return ret;
	insert_equilibrate_leaf_path(tree, (node_rb_tree_ref_t*)node, path);
	return -ERROR_NO_ERROR;
}

node_bsearch_tree_ref_t* bsearch_tree_remove_impl(bsearch_tree_ref_t* tree,
												  void* value,
												  btree_path_t* path);

static void remove_equilibrate_leaf_path(bsearch_tree_ref_t* tree,
										 node_rb_tree_ref_t* node,
										 btree_path_t path) {
	int child_index;
	node_rb_tree_ref_t **siblings, **nephews;
	node_bsearch_tree_ref_t **father_ptr, **brother_ptr;
	node_rb_tree_ref_t *father, *brother, *close, *distant;
	// Only zero or one-child nodes can be removed
	// If a two node is to be remove we swap its value with the value of its
	// successor and try to remove its successor (see resolve_remove in
	// bsearch_tree_ref.c)
	BOOL has_right = node->rs != NULL;
	BOOL has_left = node->ls != NULL;
	if (has_right && !has_left) {
		node->rs->red = FALSE;
		return;
	}
	if (has_left && !has_right) {
		node->ls->red = FALSE;
		return;
	}
	father = node->father;
	if (father == NULL || node->red == TRUE)
		return;
	// node is not the root, has no child and is BLACK
	do {
		child_index = path_last_direction(path);
		father_ptr = node_bsearch_tree_get_location(
			tree, (node_bsearch_tree_ref_t*)father);
		siblings = &father->ls;
		brother_ptr = (node_bsearch_tree_ref_t**)&siblings[1 - child_index];
		brother = (node_rb_tree_ref_t*)*brother_ptr;
		nephews = &brother->ls;
		close = nephews[child_index];
		distant = nephews[1 - child_index];
		// Case 3: Sibling is RED
		if (brother->red == TRUE) {
			if (child_index == 0)
				bsearch_tree_rotate_left(father_ptr);
			else
				bsearch_tree_rotate_right(father_ptr);
			father->red = TRUE;
			brother->red = FALSE;
			continue;
		}
		// Sibling is BLACK
		BOOL close_is_red = close != NULL && close->red == TRUE;
		BOOL distant_is_red = distant != NULL && distant->red == TRUE;
		if (close_is_red || distant_is_red) {
			// Case 5: Close nephew is RED
			if (close_is_red == TRUE) {
				if (child_index == 0)
					bsearch_tree_rotate_right(brother_ptr);
				else
					bsearch_tree_rotate_left(brother_ptr);
				brother->red = TRUE;
				close->red = FALSE;
				distant = brother;
				brother = close;
			}
			// Case 6: Distant nephew is RED
			if (child_index == 0)
				bsearch_tree_rotate_left(father_ptr);
			else
				bsearch_tree_rotate_right(father_ptr);
			brother->red = father->red;
			father->red = FALSE;
			distant->red = FALSE;
			return;
		}
		// Both nephews are BLACK
		// Case 4: Father is RED
		if (father->red == TRUE) {
			brother->red = TRUE;
			father->red = FALSE;
			return;
		}
		// Case 2: father, brother and both nephews are BLACK
		brother->red = TRUE;
		father = father->father;
		path.length -= 1;
	} while (father != NULL);
	return;
}

BOOL rb_tree_remove(bsearch_tree_ref_t* tree, void* value) {
	btree_path_t path;
	node_rb_tree_ref_t* node =
		(node_rb_tree_ref_t*)bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	if (tree->root != NULL)
		remove_equilibrate_leaf_path(tree, node, path);
	free(node);
	return TRUE;
}

BOOL rb_tree_is_node_red(node_bsearch_tree_ref_t* node) {
	if (node == NULL)
		return FALSE;
	node_rb_tree_ref_t* rb_node = (node_rb_tree_ref_t*)node;
	return rb_node->red == TRUE;
}
