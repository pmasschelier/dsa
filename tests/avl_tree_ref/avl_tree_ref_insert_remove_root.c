
#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "ptr.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	avl_tree_insert(tree, ptr(TYPE_INT, a), NULL);
	BOOL ret = avl_tree_remove(tree, &a);
	assert(ret == TRUE);
	assert(tree->root == NULL);
	free_avl_tree_ref(tree);
	return 0;
}
