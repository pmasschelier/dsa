#include "btree_ref/rb_tree_ref.h"
#include <stdint.h>
#include <stdlib.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/path.h"
#include "errors.h"
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

static unsigned get_father_index(node_rb_tree_ref_t* node) {
	return node->father->father->rs == node->father;
}

node_bsearch_tree_ref_t** node_bsearch_tree_get_location(
	bsearch_tree_ref_t* tree,
	node_bsearch_tree_ref_t* node);

static void insert_equilibrate_leaf_path(bsearch_tree_ref_t* tree,
										 node_rb_tree_ref_t* node) {
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
		unsigned father_index = get_father_index(node);
		father_ptr = (node_bsearch_tree_ref_t**)&parents[father_index];
		father = (node_rb_tree_ref_t*)*father_ptr;
		uncle = parents[1 - father_index];
		children = &father->ls;

		// Case 5 and 6 (uncle is considered BLACK)
		if (uncle == NULL || uncle->red == FALSE) {
			// Case 5 (inner node)
			if (children[1 - father_index] == node) {
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
	insert_equilibrate_leaf_path(tree, (node_rb_tree_ref_t*)node);
	return -ERROR_NO_ERROR;
}

node_bsearch_tree_ref_t* bsearch_tree_remove_impl(bsearch_tree_ref_t* tree,
												  void* value,
												  btree_path_t* path);

static void remove_equilibrate_leaf_path(bsearch_tree_ref_t* tree,
										 node_rb_tree_ref_t* node) {}

BOOL rb_tree_remove(bsearch_tree_ref_t* tree, void* value) {
	btree_path_t path;
	node_rb_tree_ref_t* node =
		(node_rb_tree_ref_t*)bsearch_tree_remove_impl(tree, value, &path);
	if (node == NULL)
		return FALSE;
	if (tree->root == NULL)
		return TRUE;
	remove_equilibrate_leaf_path(tree, node);
	free(node);
	return TRUE;
}

BOOL rb_tree_is_node_red(node_bsearch_tree_ref_t* node) {
	node_rb_tree_ref_t* rb_node = (node_rb_tree_ref_t*)node;
	return rb_node->red;
}
