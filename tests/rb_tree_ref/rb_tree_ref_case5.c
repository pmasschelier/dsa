#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"
#include "list_ref/linked_list_ref.h"
#include "ptr.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Pass only if rb_tree_ref_case1 passes

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 7), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 3), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 9), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 1), NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 2), NULL));
	assert(*get_node_ref(tree->root->ls->ls, int) == 1);
	assert(*get_node_ref(tree->root->ls, int) == 2);
	assert(*get_node_ref(tree->root->ls->rs, int) == 3);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);

	clean_bsearch_tree_ref(tree);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 7), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 3), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 9), NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 5), NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, ptr(TYPE_INT, 4), NULL));
	assert(*get_node_ref(tree->root->ls->ls, int) == 3);
	assert(*get_node_ref(tree->root->ls, int) == 4);
	assert(*get_node_ref(tree->root->ls->rs, int) == 5);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);
	free_bsearch_tree_ref(tree);
	return 0;
}
