
#include <assert.h>
#include <stdlib.h>
#include "btree_ref/rb_tree_ref.h"
#include "compare.h"
#include "list_ref/list_ref.h"
#include "ptr.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE a = 2;
KEY_TYPE b = 3;

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	rb_tree_insert(tree, ptr(TYPE_INT, a), NULL);
	rb_tree_insert(tree, ptr(TYPE_INT, b), NULL);
	BOOL ret = rb_tree_remove(tree, &a);
	assert(ret == TRUE);
	assert(*get_node_ref(tree->root, int) == b);
	free_bsearch_tree_ref(tree);
	return 0;
}
