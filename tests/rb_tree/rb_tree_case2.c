#include <assert.h>
#include "btree/rb_tree.h"
#include "errors.h"

#define KEY_TYPE int
DEFINE_COMPARE_SCALAR(int)

// Test order: Pass iif rb_tree_case1 passes
KEY_TYPE values[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main(void) {
	bsearch_tree_t* tree = create_bsearch_tree(sizeof(int), compare_int);
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[7], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[3], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[9], NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[1], NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls->ls) == TRUE);

	clean_bsearch_tree(tree);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[7], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[3], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[9], NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[11], NULL));
	assert(rb_tree_is_node_red(tree->root) == TRUE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs) == TRUE);

	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[8], NULL));
	assert(ERROR_NO_ERROR == rb_tree_insert(tree, &values[12], NULL));
	assert(rb_tree_is_node_red(tree->root) == FALSE);
	assert(rb_tree_is_node_red(tree->root->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs) == TRUE);
	assert(rb_tree_is_node_red(tree->root->rs->ls) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs) == FALSE);
	assert(rb_tree_is_node_red(tree->root->rs->rs->rs) == TRUE);

	free_bsearch_tree(tree);
	return 0;
}
