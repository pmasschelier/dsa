
#include <assert.h>
#include "btree/bsearch_tree.h"
#include "compare.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	bsearch_tree_insert(tree, &a, NULL);
	BOOL ret = bsearch_tree_remove(tree, &a);
	assert(ret == TRUE);
	assert(tree->root == NULL);
	free_bsearch_tree(tree);
	return 0;
}
