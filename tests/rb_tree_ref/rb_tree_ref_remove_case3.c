#include <assert.h>
#include "btree_ref/bsearch_tree_ref.h"
#include "btree_ref/rb_tree_ref.h"
#include "compare.h"
#include "list_ref/linked_list.h"
#include "structures.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

KEY_TYPE father = 2;
KEY_TYPE value = 1;
KEY_TYPE brother = 4;
KEY_TYPE close = 3;
KEY_TYPE distant = 5;
KEY_TYPE distant_son = 6;

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	rb_tree_insert(tree, &father, NULL);
	rb_tree_insert(tree, &value, NULL);
	rb_tree_insert(tree, &brother, NULL);
	rb_tree_insert(tree, &close, NULL);
	rb_tree_insert(tree, &distant, NULL);
	rb_tree_insert(tree, &distant_son, NULL);
	assert(*get_node_ref(tree->root, int) == father);
	assert(FALSE == rb_tree_is_node_red(tree->root));
	assert(*get_node_ref(tree->root->ls, int) == value);
	assert(FALSE == rb_tree_is_node_red(tree->root->ls));
	assert(*get_node_ref(tree->root->rs, int) == brother);
	assert(TRUE == rb_tree_is_node_red(tree->root->rs));
	assert(*get_node_ref(tree->root->rs->ls, int) == close);
	assert(FALSE == rb_tree_is_node_red(tree->root->rs->ls));
	assert(*get_node_ref(tree->root->rs->rs, int) == distant);
	assert(FALSE == rb_tree_is_node_red(tree->root->rs->rs));
	assert(*get_node_ref(tree->root->rs->rs->rs, int) == distant_son);
	assert(TRUE == rb_tree_is_node_red(tree->root->rs->rs->rs));

	BOOL ret = rb_tree_remove(tree, &value);
	assert(ret == TRUE);
	assert(*get_node_ref(tree->root, int) == brother);
	assert(FALSE == rb_tree_is_node_red(tree->root));
	assert(*get_node_ref(tree->root->ls, int) == father);
	assert(FALSE == rb_tree_is_node_red(tree->root->ls));
	assert(*get_node_ref(tree->root->rs, int) == distant);
	assert(FALSE == rb_tree_is_node_red(tree->root->rs));
	assert(*get_node_ref(tree->root->rs->rs, int) == distant_son);
	assert(TRUE == rb_tree_is_node_red(tree->root->rs->rs));
	assert(*get_node_ref(tree->root->ls->rs, int) == close);
	assert(TRUE == rb_tree_is_node_red(tree->root->ls->rs));

	free_bsearch_tree_ref(tree);
	return 0;
}
