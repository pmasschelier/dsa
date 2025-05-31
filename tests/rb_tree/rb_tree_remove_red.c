#include <assert.h>
#include "btree/rb_tree.h"
#include "errors.h"

// Test order: Pass iif rb_tree_case3 passes

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;
KEY_TYPE b = 0;

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &a, NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &b, NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	assert(TRUE == rb_tree_remove(tree, &b));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(tree->root->ls == NULL);

	free_bsearch_tree(tree);
	return 0;
}
