#include <assert.h>
#include "btree/bsearch_tree.h"
#include "compare.h"

DEFINE_COMPARE_SCALAR(int)

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(NULL == bsearch_tree_min(tree));
	free_bsearch_tree(tree);
	return 0;
}
