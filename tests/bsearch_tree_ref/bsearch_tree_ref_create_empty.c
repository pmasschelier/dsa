#include <assert.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "compare.h"

DEFINE_COMPARE_SCALAR(int)

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(tree != NULL);
	assert(tree->root == NULL);
	assert(tree->compare == compare_int);
	assert(tree->size == sizeof(int));
	free_bsearch_tree_ref(tree);
	return 0;
}
