#include <assert.h>
#include <stdlib.h>
#include "btree_ref/rb_tree_ref.h"
#include "compare.h"
#include "list_ref/list_ref.h"
#include "ptr.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE father = 2;
KEY_TYPE value = 1;
KEY_TYPE brother = 4;
KEY_TYPE distant = 5;

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	rb_tree_insert(tree, ptr(TYPE_INT, father), NULL);
	rb_tree_insert(tree, ptr(TYPE_INT, value), NULL);
	rb_tree_insert(tree, ptr(TYPE_INT, brother), NULL);
	rb_tree_insert(tree, ptr(TYPE_INT, distant), NULL);
	BOOL ret = rb_tree_remove(tree, &value);
	assert(ret == TRUE);
	assert(*get_node_ref(tree->root, int) == brother);
	assert(*get_node_ref(tree->root->ls, int) == father);
	assert(*get_node_ref(tree->root->rs, int) == distant);
	free_bsearch_tree_ref(tree);
	return 0;
}
