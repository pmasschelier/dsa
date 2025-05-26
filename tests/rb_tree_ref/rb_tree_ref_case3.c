#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Base test case
KEY_TYPE values[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[1], NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
