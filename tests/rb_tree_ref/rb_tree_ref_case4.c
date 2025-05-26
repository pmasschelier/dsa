#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"

// Test order: Pass iif rb_tree_ref_case3 passes

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE values[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[1], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[0], NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	clean_bsearch_tree_ref(tree);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[1], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[2], NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
