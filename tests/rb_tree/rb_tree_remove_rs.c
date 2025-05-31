
#include <assert.h>
#include "btree/rb_tree.h"
#include "compare.h"
#include "list_ref/linked_list.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 2;
KEY_TYPE b = 3;

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	rb_tree_insert(tree, &a, NULL);
	rb_tree_insert(tree, &b, NULL);
	BOOL ret = rb_tree_remove(tree, &a);
	assert(ret == TRUE);
	assert(*get_node_ref(tree->root, int) == b);
	free_bsearch_tree(tree);
	return 0;
}
