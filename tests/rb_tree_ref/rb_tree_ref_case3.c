#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Base test case

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 1), NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
