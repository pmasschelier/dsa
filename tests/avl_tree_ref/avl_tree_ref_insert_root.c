#include <assert.h>
#include <stdlib.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "errors.h"
#include "list_ref/list_ref.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	avl_tree_ref_t* tree = create_avl_tree(sizeof(int), compare_int);
	int ret = avl_tree_insert(tree, ptr(TYPE_INT, a), NULL);
	assert(ret == -ERROR_NO_ERROR);
	assert(tree->root != NULL);
	assert(tree->root->subtree == 1);
	assert(tree->root->father == NULL);
	assert(tree->root->ls == NULL);
	assert(tree->root->rs == NULL);
	assert(*get_node_ref(tree->root, int) == a);
	free_avl_tree_ref(tree);
	return 0;
}
