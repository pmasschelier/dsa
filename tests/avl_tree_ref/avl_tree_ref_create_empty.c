#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "btree_ref/btree_ref.h"
#include "compare.h"

DEFINE_COMPARE_SCALAR(int)

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	assert(tree != NULL);
	assert(tree->root == NULL);
	assert(tree->free_element == free);
	assert(tree->compare == compare_int);
	assert(tree->size == sizeof(int));
	free_avl_tree_ref(tree);
	return 0;
}
