#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Pass iif rb_tree_ref_case1 passes

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 7), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 3), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 9), NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 1), NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);

	clean_bsearch_tree_ref(tree);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 7), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 3), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 9), NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 11), NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 8), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 12), NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->rs->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs->rs) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
