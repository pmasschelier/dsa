#include <assert.h>
#include "btree_ref/rb_tree_ref.h"
#include "errors.h"
#include "list_ref/linked_list_ref.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Pass only if rb_tree_ref_case1 passes
KEY_TYPE values[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main(void) {
	bsearch_tree_ref_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[7], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[3], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[9], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[1], NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[0], NULL));
	assert(*get_node_ref(tree->root->ls->ls, int) == 0);
	assert(*get_node_ref(tree->root->ls, int) == 1);
	assert(*get_node_ref(tree->root->ls->rs, int) == 3);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);

	clean_bsearch_tree_ref(tree);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[7], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[3], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[9], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[5], NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[6], NULL));
	assert(*get_node_ref(tree->root->ls->ls, int) == 3);
	assert(*get_node_ref(tree->root->ls, int) == 5);
	assert(*get_node_ref(tree->root->ls->rs, int) == 6);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->rs) == TRUE);

	free_bsearch_tree_ref(tree);
	return 0;
}
