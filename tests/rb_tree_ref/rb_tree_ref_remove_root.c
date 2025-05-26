#include <assert.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &a, NULL));
	assert(TRUE == rb_tree_remove(tree, &a));
	assert(NULL == tree->root);
	free_bsearch_tree_ref(tree);
	return 0;
}
