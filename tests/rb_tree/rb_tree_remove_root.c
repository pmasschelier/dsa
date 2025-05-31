#include <assert.h>
#include "btree/bsearch_tree.h"
#include "btree/rb_tree.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &a, NULL));
	assert(TRUE == rb_tree_remove(tree, &a));
	assert(NULL == tree->root);
	free_bsearch_tree(tree);
	return 0;
}
