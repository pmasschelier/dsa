#include <assert.h>
#include "btree_ref/avl_tree_ref.h"
#include "compare.h"
#include "errors.h"
#include "list_ref/linked_list.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 1;

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	int ret = avl_tree_insert(tree, &a, NULL);
	assert(ret == -ERROR_NO_ERROR);
	assert(tree->root != NULL);
	assert(tree->root->parent == NULL);
	assert(tree->root->ls == NULL);
	assert(tree->root->rs == NULL);
	assert(*get_node_ref(tree->root, int) == a);
	free_bsearch_tree_ref(tree);
	return 0;
}
