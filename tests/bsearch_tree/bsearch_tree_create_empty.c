#include <assert.h>
#include "btree/bsearch_tree.h"
#include "compare.h"

DEFINE_COMPARE_SCALAR(int)

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(tree != NULL);
	assert(tree->root == NULL);
	assert(tree->compare == compare_int);
	assert(tree->size_bytes == sizeof(int));
	free_bsearch_tree(tree);
	return 0;
}
