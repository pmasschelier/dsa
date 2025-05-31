#include <assert.h>
#include "btree/rb_tree.h"
#include "compare.h"
#include "list_ref/linked_list.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE father = 2;
KEY_TYPE value = 1;
KEY_TYPE brother = 4;
KEY_TYPE close = 3;

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	rb_tree_insert(tree, &father, NULL);
	rb_tree_insert(tree, &value, NULL);
	rb_tree_insert(tree, &brother, NULL);
	rb_tree_insert(tree, &close, NULL);
	BOOL ret = rb_tree_remove(tree, &value);
	assert(ret == TRUE);
	assert(*get_node_ref(tree->root, int) == close);
	assert(*get_node_ref(tree->root->ls, int) == father);
	assert(*get_node_ref(tree->root->rs, int) == brother);
	free_bsearch_tree(tree);
	return 0;
}
