#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"
#include "ptr.h"

// Test order: Pass iif rb_tree_ref_case3 passes

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 1), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 0), NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	clean_bsearch_tree_ref(tree);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 1), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 2), NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
